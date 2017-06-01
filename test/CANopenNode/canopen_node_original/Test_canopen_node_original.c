#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_OD.h"
#include "CO_SDO.h"
#include "CO_Config.h"


extern CO_CANtx_t last_sent_buffer;
uint32_t sdo_test_callback_call_count = 0;

void setUp(void)
{
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        sdo_test_callback_call_count = 0;
//        test_plogger_reset();
}

void tearDown(void)
{
        
}

enum sdo_read_write {
        SDO_READ,
        SDO_WRITE,
};

void make_msg_for_sdo_expedited_transfere(CO_SDO_t *SDO,
                                          enum sdo_read_write rw,
                                          uint16_t index,
                                          uint8_t subindex,
                                          uint32_t data,
                                          uint32_t data_size)
{
        CO_CANrxMsg_t msg;
        uint32_t ccs = 2;
        if (SDO_WRITE == rw)
        {
                ccs = 1;
        }
        msg.ident = SDO->nodeId;
        msg.DLC = 8; // SDO msgs are always 8 bytes ???
        msg.data[0] = ccs << 5 | (4 - data_size) << 2 | 0x3;
        msg.data[1] = index & 0xff;
        msg.data[2] = index >> 8;
        msg.data[3] = subindex;

        uint32_t i;
        uint8_t *pd = (uint8_t *) &data;
        for (i = 4; 8 > i; i++)
        {
                msg.data[i] = pd[i - 4];
        }
        CO_SDO_receive(SDO, &msg);
}


static uint32_t sdo_test_callback(void *arg)
{
//        CO_ODF_arg_t *ODF_arg = arg;
        sdo_test_callback_call_count += 1;
        return 0;
}

void test_SDO_expedited_transfer(void)
{
        uint32_t node_id = 0x20;
        CO_ReturnError_t err;
        CO_SDO_t SDO;
        CO_CANmodule_t NOT_USED;

        struct CO_OD *OD = CO_OD_interface_init();
        
        err = CO_SDO_init(
                &SDO,
                0x600 + node_id,
                0x580 + node_id,
                OD_H1200_SDO_SERVER_PARAM,
                OD,
                node_id,
                &NOT_USED, // in testing sdo Rx function will be called directly
                0,
                &NOT_USED,
                1);

        TEST_ASSERT_EQUAL_UINT32(0, err);

        // register a callback

        CO_OD_configure(OD, 0x2000,
                        sdo_test_callback,
                        NULL);
        // TEST normal successfully expedited transfer
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_WRITE,
                                             0x2000,
                                             0,
                                             0xf0,
                                             4);

        err = CO_SDO_process(
                &SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT32(1, sdo_test_callback_call_count);

        // TEST normal successfully expedited transfer again
        // to check if first transfer brakes something globaly
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_WRITE,
                                             0x2000,
                                             0,
                                             0xff,
                                             4);

        err = CO_SDO_process(
                &SDO,
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
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_WRITE,
                                             0xffff,
                                             0,
                                             0xff,
                                             4);
        err = CO_SDO_process(
                &SDO,
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
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_WRITE,
                                             0x2001,
                                             0,
                                             0x0,
                                             2);
        err = CO_SDO_process(
                &SDO,
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
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_READ,
                                             0x1200,
                                             0,
                                             0x0,
                                             1);
        err = CO_SDO_process(
                &SDO,
                true,
                100,
                1000,
                NULL);
        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT8(2, last_sent_buffer.data[0] >> 5); // scs=3
        TEST_ASSERT_EQUAL_UINT8(3, (last_sent_buffer.data[0] >> 2) & 3); // n
        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] & 0x3); 
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); //index lsb
        TEST_ASSERT_EQUAL_UINT8(0x12, last_sent_buffer.data[2] ); //index msg
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // subindex
        TEST_ASSERT_EQUAL_UINT8(0x2, last_sent_buffer.data[4] ); //lsb

        // subindex 1: COB_IDClientToServer
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_READ,
                                             0x1200,
                                             1,
                                             0x0,
                                             1);
        err = CO_SDO_process(
                &SDO,
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
        TEST_ASSERT_EQUAL_UINT8(0x1, last_sent_buffer.data[3] ); // subindex
        TEST_ASSERT_EQUAL_UINT8(node_id, last_sent_buffer.data[4] ); //lsb cob_id
        TEST_ASSERT_EQUAL_UINT8(0x6, last_sent_buffer.data[5] ); //msg cob_id

        // subindex 2: COB_IDServerToClient
        make_msg_for_sdo_expedited_transfere(&SDO,
                                             SDO_READ,
                                             0x1200,
                                             2,
                                             0x0,
                                             1);
        err = CO_SDO_process(
                &SDO,
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

        
}
