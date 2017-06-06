#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_SDO.h"
#include "CO_Config.h"
#include "CO_OD_interface.h"

extern CO_CANtx_t last_sent_buffer;
uint32_t sdo_test_callback_call_count = 0;
uint32_t domain_test_callback_cnt = 0;

// normal dl sdo transfer global vars
uint8_t *sdo_dl_data_ptr = NULL;
uint32_t sdo_dl_data_size = 0;
uint32_t sdo_dl_data_index = 0;
uint32_t sdo_dl_can_id = 0;
CO_SDO_t *SDO_dl = NULL;

void sdo_dl_reset(void)
{
        sdo_dl_data_ptr = NULL;
        sdo_dl_data_size = 0;
        sdo_dl_data_index = 0;
        sdo_dl_can_id = 0;
        SDO_dl = NULL;
}

void setUp(void)
{
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        sdo_test_callback_call_count = 0;
        domain_test_callback_cnt = 0;

        sdo_dl_reset();
//        test_plogger_reset();
}

void tearDown(void)
{
        
}

enum sdo_read_write {
        SDO_READ,
        SDO_WRITE,
};

enum sdo_transfere_types {
        SDO_NORMAL_TRANSFERE,
        SDO_EXPEDITED_TRANSFERE,
};

enum sdo_size_indicators {
        SDO_SIZE_NOT_INCLUDED,
        SDO_SIZE_INCLUDED,
};

int32_t sdo_dl_initiate(CO_CANrxMsg_t *msg, uint8_t node_id,
                        uint16_t index, uint8_t subindex,
                        uint8_t transfere_type, uint8_t size_indicator,
                        uint8_t *data, uint32_t data_size)
{
        uint32_t i;

        if (SDO_NORMAL_TRANSFERE == transfere_type &&
            SDO_SIZE_NOT_INCLUDED == size_indicator)
        {
                return -1; // reserved for future used
        }
        else if (SDO_EXPEDITED_TRANSFERE == transfere_type &&
            SDO_SIZE_NOT_INCLUDED == size_indicator)
        {
                // length of the data not known
        }
        else if (SDO_EXPEDITED_TRANSFERE == transfere_type &&
            SDO_SIZE_INCLUDED == size_indicator)
        {
                // d contains 4-n bytes to be downloaded
                if (data_size > 4)
                {
                        return -1;
                }
                const uint8_t ccs = 1;
                msg->data[0] = ccs << 5 |
                        (4 - data_size) << 2 |
                        (transfere_type & 0x1) << 1 |
                        (size_indicator & 0x1);

                uint8_t *pd = data;
                for (i = 4; 8 > i; i++)
                {
                        msg->data[i] = pd[i - 4];
                }

        }
        else if (SDO_NORMAL_TRANSFERE == transfere_type &&
            SDO_SIZE_INCLUDED == size_indicator)
        {
                // d contains num of bytes to be downloaded

                const uint8_t ccs = 1;
                msg->data[0] = ccs << 5 |
                        (transfere_type & 0x1) << 1 |
                        (size_indicator & 0x1);


                uint8_t *pd = (uint8_t *) &data_size;
                for (i = 4; 8 > i; i++)
                {
                        msg->data[i] = pd[i - 4];
                }
        }
        else
        {
                return -1;
        }

        
        msg->ident = node_id;
        msg->DLC = 8; // SDO msgs are always 8 bytes ???

        msg->data[1] = index & 0xff;
        msg->data[2] = index >> 8;
        msg->data[3] = subindex;
        return 0;
}


int32_t sdo_dl_segment(CO_CANrxMsg_t *msg, uint8_t node_id,
                       uint8_t toggle_flag, uint8_t last_segment,
                       uint8_t *data, uint32_t data_size)
{
        uint32_t i;

        if (7 < data_size)
        {
                return -1;
        }

        const uint8_t ccs = 0;
        msg->data[0] = ccs << 5 |
                (toggle_flag & 0x1) << 4 |
                (7 - data_size) << 1 | last_segment;

        uint8_t *pd = (uint8_t *) data;
        for (i = 1; 8 > i; i++)
        {
                msg->data[i] = pd[i - 1];
        }
        
        msg->ident = node_id;
        msg->DLC = 8; // SDO msgs are always 8 bytes ???

        return 0;
}

int32_t sdo_dl_send_next_segment(uint8_t sdo_toggle_flag)
{
        CO_CANrxMsg_t msg;
        uint8_t last_segment;
        uint8_t *data;
        uint32_t data_size;

        if (sdo_dl_data_size > (sdo_dl_data_index + 7))
        {
                last_segment = 0;
                data = &sdo_dl_data_ptr[sdo_dl_data_index];
                data_size = 7;

        }
        else if (sdo_dl_data_size == (sdo_dl_data_index + 7))
        {
                last_segment = 1;
                data = &sdo_dl_data_ptr[sdo_dl_data_index];
                data_size = 7;
        }
        else
        {
                last_segment = 1;
                data = &sdo_dl_data_ptr[sdo_dl_data_index];
                data_size = sdo_dl_data_size - sdo_dl_data_index;
        }

        sdo_dl_data_index += data_size;
        int32_t r = sdo_dl_segment(&msg, sdo_dl_can_id,
                                   sdo_toggle_flag, last_segment,
                                   data, data_size);

        if (0 == r)
        {
                CO_SDO_receive(SDO_dl, &msg);
        }

        return r;
}


int32_t sdo_dl_normal_init(CO_SDO_t *SDO,
                           enum sdo_read_write rw,
                           uint16_t index,
                           uint8_t subindex,
                           uint8_t *data,
                           uint32_t data_size)
{
        int32_t r;
        CO_CANrxMsg_t msg;

        r = sdo_dl_initiate(&msg, SDO->nodeId, index, subindex,
                            SDO_NORMAL_TRANSFERE,
                            SDO_SIZE_INCLUDED,
                            data, data_size);


        sdo_dl_data_ptr = data;
        sdo_dl_data_size = data_size;
        sdo_dl_data_index = 0;
        sdo_dl_can_id = SDO->nodeId;
        SDO_dl = SDO;

        if (0 == r)
        {
                CO_SDO_receive(SDO, &msg);
        }
        
        return r;
}

int32_t sdo_ul_initiate(CO_CANrxMsg_t *msg, uint8_t node_id,
                        uint16_t index, uint8_t subindex)
{
        uint32_t i;

        const uint8_t ccs = 2;
        
        msg->ident = node_id;
        msg->DLC = 8; // SDO msgs are always 8 bytes ???
        msg->data[0] = ccs << 5;
        msg->data[1] = index & 0xff;
        msg->data[2] = index >> 8;
        msg->data[3] = subindex;
        for (i = 4; 8 > i; i++)
        {
                msg->data[i] = 0;
        }

        return 0;
}


// This is a function simulating the master node starting a sdo expedited transfer
// It will insert the sdo message to the "driver rx buffer"
int32_t make_msg_for_sdo_expedited_transfere(CO_SDO_t *SDO,
                                          enum sdo_read_write rw,
                                          uint16_t index,
                                          uint8_t subindex,
                                          uint32_t data,
                                          uint32_t data_size)
{
        CO_CANrxMsg_t msg;
        int32_t r;

        if (SDO_WRITE == rw)
        {
                r = sdo_dl_initiate(&msg, SDO->nodeId,
                                    index, subindex,
                                    SDO_EXPEDITED_TRANSFERE, SDO_SIZE_INCLUDED,
                                    (uint8_t *) &data, data_size);
        }
        else if (SDO_READ == rw)
        {
                r = sdo_ul_initiate(&msg, SDO->nodeId, index, subindex);
        }
        else
        {
                r = -1;
        }

        if (0 == r)
        {
                CO_SDO_receive(SDO, &msg);
        }
        return r;
}

static uint32_t sdo_test_callback(void *arg)
{
//        CO_ODF_arg_t *ODF_arg = arg;
        sdo_test_callback_call_count += 1;
        return 0;
}

static CO_SDO_t *init_sdo(uint32_t node_id, void *OD)
{
        CO_ReturnError_t err;
        // malloc memory for SDO.
        // Dont bother freeing it, we have enough ram on a PC :)
        CO_SDO_t *SDO = malloc(sizeof(CO_SDO_t));
        CO_CANrx_t *rxArray = malloc(sizeof(CO_CANrx_t));
        CO_CANtx_t *txArray = malloc(sizeof(CO_CANtx_t));;
        CO_CANmodule_t *NOT_USED = malloc(sizeof(CO_CANmodule_t));

        NOT_USED->rxArray = rxArray;
        NOT_USED->txArray = txArray;
        
        err = CO_SDO_init(
                SDO,
                0x600 + node_id,
                0x580 + node_id,
                0x1200, //OD_H1200_SDO_SERVER_PARAM,
                OD,
                node_id,
                NOT_USED, // in testing sdo Rx function will be called directly
                0,
                NOT_USED,
                0);
        if (err)
        {
                return NULL;
        }
        else
        {
                return SDO;
        }
}

void test_SDO_init(void)
{
        uint8_t node_id = 0x20;
        struct con_od_list_node_var OD;
        CO_SDO_t *SDO = init_sdo(node_id, &OD);
        TEST_ASSERT_NOT_NULL(SDO);
        TEST_ASSERT_EQUAL_PTR(&OD, SDO->OD);
        TEST_ASSERT_EQUAL_UINT8(node_id, SDO->nodeId);
        TEST_ASSERT_EQUAL_UINT8(node_id, SDO->nodeId);
        TEST_ASSERT_EQUAL_UINT8(CO_SDO_ST_IDLE, SDO->state);
        TEST_ASSERT_EQUAL_UINT8(false, SDO->CANrxNew);
        TEST_ASSERT_EQUAL_PTR(NULL, SDO->pFunctSignal);

// TODO: Test or rewrite ...
//    /* configure SDO server CAN reception */
//    CO_CANrxBufferInit(
//            CANdevRx,               /* CAN device */
//            CANdevRxIdx,            /* rx buffer index */
//            COB_IDClientToServer,   /* CAN identifier */
//            0x7FF,                  /* mask */
//            0,                      /* rtr */
//            (void*)SDO,             /* object passed to receive function */
//            CO_SDO_receive);        /* this function will process received message */
//
//    /* configure SDO server CAN transmission */
//    SDO->CANdevTx = CANdevTx;
//    SDO->CANtxBuff = CO_CANtxBufferInit(
//            CANdevTx,               /* CAN device */
//            CANdevTxIdx,            /* index of specific buffer inside CAN module */
//            COB_IDServerToClient,   /* CAN identifier */
//            0,                      /* rtr */
//            8,                      /* number of data bytes */
//            0);                     /* synchronous message flag bit */
//
//
}

void test_SDO_expedited_transfer(void)
{
        uint8_t node_id = 0x20;
        CO_ReturnError_t err;

        // Make Object dictionary for testing
        uint32_t COB_IDClientToServer = 0x600 + node_id;
        uint32_t COB_IDServerToClient = 0x580 + node_id;
        int8_t a;
        int16_t b;

        struct con_od_list_node_var OD;
        INIT_OD_ENTRY_VAR(OD, 0x2000, OD_TYPE_UINT8, CO_ODA_WRITEABLE, &a, NULL, NULL);

        struct con_od_list_node_var OD_b;
        INIT_OD_ENTRY_VAR(OD_b, 0x2001, OD_TYPE_UINT8, CO_ODA_READABLE, &b, NULL, NULL);

        struct con_od_list_node_var_with_callback OD_a_cb;
        INIT_OD_ENTRY_VAR_WITH_CALLBACK(OD_a_cb, 0x2002, OD_TYPE_UINT8,
                                        CO_ODA_WRITEABLE, &a, sdo_test_callback, NULL, NULL);

        // Make a OD record
        struct con_od_record_entry OD_1200_subelements[2];
        INIT_RECORD_SUBENTRY(OD_1200_subelements[0],
                             OD_TYPE_UINT32,
                             CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &COB_IDClientToServer, "", "");

        INIT_RECORD_SUBENTRY(OD_1200_subelements[1],
                             OD_TYPE_UINT32,
                             CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &COB_IDServerToClient, "", "");

        struct con_od_list_node_record OD_1200;
        INIT_OD_ENTRY_RECORD(OD_1200, 0x1200, &OD_1200_subelements[0], 2, "", "");


        // Add OD elements together in a OD linked list
        err = con_od_add_element_to_od(&OD, &OD_b);
        TEST_ASSERT_EQUAL_UINT32(0, err);

        err = con_od_add_element_to_od(&OD, &OD_1200);
        TEST_ASSERT_EQUAL_UINT32(0, err);

        err = con_od_add_element_to_od(&OD, &OD_a_cb);
        TEST_ASSERT_EQUAL_UINT32(0, err);


        // Init SDO
        CO_SDO_t *SDO = init_sdo(node_id, &OD);        
        TEST_ASSERT_NOT_NULL(SDO);


        // TEST normal successfully expedited transfer
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_WRITE,
                                             0x2002,
                                             0,
                                             0xf0,
                                             1);

        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT32(1, sdo_test_callback_call_count);

        // TEST normal successfully expedited transfer again
        // to check if first transfer brakes something globaly
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_WRITE,
                                             0x2000,
                                             0,
                                             0xff,
                                             1);

        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] >> 5); // scs=3
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0] & 0xf); // not used, always 0
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x20, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // subindex


        // TEST expedited transfer where OD element is missing
        // We expect to get a SDO ABORT msg in the drivers tx buffer
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_WRITE,
                                             0xffff,
                                             0,
                                             0xff,
                                             4);
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);
        TEST_ASSERT_EQUAL_INT8(-1, err); //abort occurred
        // check abort message
        TEST_ASSERT_EQUAL_UINT8(4, last_sent_buffer.data[0] >> 5); // scs=4 (abort)
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0] & 0xf); // not used, always 0
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0xff, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0xff, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // subindex
         // error code 0x06020000 => object doesnt exist in OD
        TEST_ASSERT_EQUAL_UINT32(CO_SDO_AB_NOT_EXIST, *(uint32_t *) &last_sent_buffer.data[4]);


        // TEST download expedited transfer where OD element is only readable
        // We expect to get a SDO ABORT msg in the drivers tx buffre
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_WRITE,
                                             0x2001,
                                             0,
                                             0x0,
                                             2);
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);
        TEST_ASSERT_EQUAL_INT8(-1, err); //abort occurred

        // check abort message
        TEST_ASSERT_EQUAL_UINT8(4, last_sent_buffer.data[0] >> 5); // scs=4 (abort)
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0] & 0xf); // not used, always 0
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0x01, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x20, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // subindex
         // error code 0x06020000 => object doesnt exist in OD
        TEST_ASSERT_EQUAL_UINT32(CO_SDO_AB_READONLY, *(uint32_t *) &last_sent_buffer.data[4]);



        // test record download
        // subindex 0: number of entries
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_READ,
                                             0x1200,
                                             0,
                                             0x0,
                                             1);
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);
        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT8(2, last_sent_buffer.data[0] >> 5); 
        TEST_ASSERT_EQUAL_UINT8(3, (last_sent_buffer.data[0] >> 2) & 3); // n
        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] & 0x3); 
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x12, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // subindex
        TEST_ASSERT_EQUAL_UINT8(0x2, last_sent_buffer.data[4] ); //lsb

        // subindex 1: COB_IDClientToServer
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_READ,
                                             0x1200,
                                             1,
                                             0x0,
                                             1);
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);
        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT8(2, last_sent_buffer.data[0] >> 5); 
        TEST_ASSERT_EQUAL_UINT8(0, (last_sent_buffer.data[0] >> 2) & 3); // n
        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] & 0x3); 
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x12, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x1, last_sent_buffer.data[3] ); // subindex
        // TODO: add node_id
        TEST_ASSERT_EQUAL_UINT8(node_id, last_sent_buffer.data[4] ); //lsb cob_id
        TEST_ASSERT_EQUAL_UINT8(0x6, last_sent_buffer.data[5] ); //msg cob_id

        // subindex 2: COB_IDServerToClient
        make_msg_for_sdo_expedited_transfere(SDO,
                                             SDO_READ,
                                             0x1200,
                                             2,
                                             0x0,
                                             1);
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);
        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT8(2, last_sent_buffer.data[0] >> 5); // scs=3
        TEST_ASSERT_EQUAL_UINT8(0, (last_sent_buffer.data[0] >> 2) & 3); // n
        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] & 0x3); 
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x12, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x2, last_sent_buffer.data[3] ); // subindex
        TEST_ASSERT_EQUAL_UINT8(0x80 + node_id, last_sent_buffer.data[4] ); //lsb cob_id
        TEST_ASSERT_EQUAL_UINT8(0x5, last_sent_buffer.data[5] ); //msg cob_id
        return ;
        
}

static uint32_t domain_test_callback(void *arg)
{
//        volatile CO_ODF_arg_t *ODF_arg = arg;
        domain_test_callback_cnt += 1;
        return 0;
}

// Test segmented transfer by reading a domain type
void test_SDO_segmented_transfer(void)
{
        uint8_t node_id = 0x20;
        CO_ReturnError_t err;

        // Make Object dictionary for testing

        struct con_od_list_node_domain OD;
        INIT_OD_ENTRY_DOMAIN(OD, 0x2000, CO_ODA_WRITEABLE, domain_test_callback, NULL, NULL);


        // Init SDO
        CO_SDO_t *SDO = init_sdo(node_id, &OD);        
        TEST_ASSERT_NOT_NULL(SDO);

        uint8_t data[] = {1,2,3,4,5,6,7,8};
        // TEST normal successfully SDO transfer
        
        int32_t r = sdo_dl_normal_init(SDO,
                                       SDO_WRITE,
                                       0x2000,
                                       0,
                                       data,
                                       sizeof(data));

        TEST_ASSERT_EQUAL_UINT32(0, r);
        
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(1, err);// sdo sever is in transfere state

        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] >> 5); // scs=3
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0] & 0xf); // not used, always 0
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x20, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // subindex


        
        r =  sdo_dl_send_next_segment(0);
        TEST_ASSERT_EQUAL_UINT32(0, r);
        
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(1, err);// sdo sever is in transfere state

        TEST_ASSERT_EQUAL_UINT8(1, last_sent_buffer.data[0] >> 5); // scs=1
        TEST_ASSERT_EQUAL_UINT8(0, (last_sent_buffer.data[0] >> 4) & 0x1); // toggle_bit

        r =  sdo_dl_send_next_segment(1);
        TEST_ASSERT_EQUAL_UINT32(0, r);
        
        err = CO_SDO_process(
                SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(0, err);// sdo sever is in transfere state

        TEST_ASSERT_EQUAL_UINT8(1, last_sent_buffer.data[0] >> 5); // scs=1
        TEST_ASSERT_EQUAL_UINT8(1, (last_sent_buffer.data[0] >> 4) & 0x1); // toggle_bit

        TEST_ASSERT_EQUAL_UINT32(1, domain_test_callback_cnt);

        
}
