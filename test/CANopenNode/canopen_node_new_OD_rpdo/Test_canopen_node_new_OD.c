#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_RPDO.h"
#include "CO_Config.h"
#include "CO_OD_interface.h"
#include "CO_driver.h"

extern CO_CANtx_t last_sent_buffer;
uint32_t sdo_test_callback_call_count = 0;

uint32_t     COB_IDUsedByRPDO = CO_CAN_ID_TPDO_1;
uint8_t      transmissionType = 0x1;
uint16_t     inhibitTime = 0x0102;
uint8_t      compatibilityEntry = 0x3;
uint16_t     eventTimer = 0x0405;
uint8_t      SYNCStartValue = 0x6;

int32_t callback_called_cnt = 0;

uint32_t     mappedObjects[8];
uint8_t tv1 = 0x55;
uint16_t tv2 = 0x1234;
uint32_t tv3 = 0xffffffff;
uint32_t tv4 = 0;

void setUp(void)
{
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        
        memset(mappedObjects, 0, sizeof(mappedObjects));
        sdo_test_callback_call_count = 0;
        transmissionType = 0x0;
        inhibitTime = 0x0;
        compatibilityEntry = 0x0;
        eventTimer = 0x0;
        SYNCStartValue = 0x0;

        callback_called_cnt = 0;
        
        tv1 = 0x55;
        tv2 = 0x1234;
        tv3 = 0xffffffff;
        tv4 = 0x45;
//        test_plogger_reset();
}

void tearDown(void)
{
        
}

void send_rpdo_messge(void *obj, uint8_t *msg_bytes, uint32_t size)
{
        if (size > 8)
        {
                return ;
        }
        
        CO_CANrxMsg_t msg;
        msg.ident = 0;
        msg.DLC = size;
        uint32_t i;
        for (i = 0; size > i; i++)
        {
                msg.data[i] = msg_bytes[i];
        }
        CO_PDO_receive(obj, &msg);
}

uint32_t rpdo_callback_test(void *d)
{
        callback_called_cnt += 1;
        return 0;
}

void *make_rpdo_od_entries(uint32_t number_of_maped_elements)
{
        int32_t err;
        void *OD;
        // Make a OD record
//        struct con_od_record_entry OD_1800_subelements[7];
        struct con_od_record_entry *OD_1400_subelements = malloc(sizeof(struct con_od_record_entry) * 2);

        INIT_RECORD_SUBENTRY(&OD_1400_subelements[0],
                             OD_TYPE_UINT32,
                             0x8D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &COB_IDUsedByRPDO, "", "");

        INIT_RECORD_SUBENTRY(&OD_1400_subelements[1],
                             OD_TYPE_UINT32,
                             0x0D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &transmissionType, "", "");

//        struct con_od_list_node_record OD_1400;
        struct con_od_list_node_record *OD_1400 = malloc(sizeof(struct con_od_list_node_record));
        INIT_OD_ENTRY_RECORD(OD_1400, 0x1400, &OD_1400_subelements[0], 2, "", "");

        OD = OD_1400;
        
        struct con_od_record_entry *OD_1600_subelements = malloc(sizeof(struct con_od_record_entry) * 8);

        uint32_t i;
        for (i = 0; 8 > i; i++)
        {
                INIT_RECORD_SUBENTRY(&OD_1600_subelements[i],
                                     OD_TYPE_UINT32,
                                     0x8D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                                     &mappedObjects[i], "", "");
        }

//        struct con_od_list_node_record OD_1A00;
        struct con_od_list_node_record *OD_1600 = malloc(sizeof(struct con_od_list_node_record));
        INIT_OD_ENTRY_RECORD(OD_1600, 0x1600, &OD_1600_subelements[0],
                             number_of_maped_elements, "", "");
        // Add OD elements together in a OD linked list
        err = con_od_add_element_to_od(&OD, OD_1600);
        TEST_ASSERT_EQUAL_UINT32(0, err);

        // add 0x2000 OD entry, so the map above will be valid

//        struct con_od_list_node_var test_var;
        struct con_od_list_node_var *test_var1 = malloc(sizeof(struct con_od_list_node_var)); 
        INIT_OD_ENTRY_VAR(test_var1, 0x2000, OD_TYPE_UINT8,
                          CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE,
                          &tv1, NULL, NULL);

        struct con_od_list_node_var *test_var2 = malloc(sizeof(struct con_od_list_node_var)); 
        INIT_OD_ENTRY_VAR(test_var2, 0x2001, OD_TYPE_UINT16,
                          CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE,
                          &tv2, NULL, NULL);

        struct con_od_list_node_var *test_var3 = malloc(sizeof(struct con_od_list_node_var)); 
        INIT_OD_ENTRY_VAR(test_var3, 0x2002, OD_TYPE_UINT32,
                          CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE,
                          &tv3, NULL, NULL);

        struct con_od_list_node_var_with_callback *test_var4 = malloc(sizeof(struct con_od_list_node_var_with_callback)); 
        INIT_OD_ENTRY_VAR_WITH_CALLBACK(test_var4, 0x2003, OD_TYPE_UINT8,
                                        CO_ODA_WRITEABLE | CO_ODA_RPDO_MAPABLE,
                                        &tv4, rpdo_callback_test, NULL, NULL);

        
        err = con_od_add_element_to_od(&OD, test_var1);
        err = con_od_add_element_to_od(&OD, test_var2);
        err = con_od_add_element_to_od(&OD, test_var3);
        err = con_od_add_element_to_od(&OD, test_var4);
        TEST_ASSERT_EQUAL_UINT32(0, err);
        return OD;
}

static CO_RPDO_t *init_rpdo(uint32_t node_id, void *OD)
{
        CO_ReturnError_t err;
        // malloc memory.
        // Dont bother freeing it, we have enough ram on a PC :)
        CO_RPDO_t *RPDO = malloc(sizeof(CO_RPDO_t));

//        CO_CANrx_t *rxArray = malloc(sizeof(CO_CANrx_t));
//        CO_CANtx_t *txArray = malloc(sizeof(CO_CANtx_t));;
//        CO_CANmodule_t *NOT_USED = malloc(sizeof(CO_CANmodule_t));
//
//        NOT_USED->rxArray = rxArray;
//        NOT_USED->txArray = txArray;
        
        err = CO_RPDO_init(
                RPDO,
                OD,
                node_id,
                0, // restriction flags
                0x1400,
                0x1600,
                (void *) 0x12345678);
        if (err)
        {
                return NULL;
        }
        else
        {
                return RPDO;
        }
}

void test_PDO_init(void)
{
//        int32_t err = 0;
        uint8_t node_id = 0x20;
        
        transmissionType = 0x1;
        inhibitTime = 0x0102;
        compatibilityEntry = 0x3;
        eventTimer = 0x0405;
        SYNCStartValue = 0x6;

        mappedObjects[0] = (0x2000 << 16) | (0x0 << 8) | 0x8;
//        void *OD = NULL;
        void *OD = make_rpdo_od_entries(1);
        TEST_ASSERT_NOT_NULL(OD);
        
        CO_RPDO_t *RPDO = init_rpdo(node_id, OD);
        TEST_ASSERT_NOT_NULL(RPDO);
        TEST_ASSERT_EQUAL_UINT8(node_id, RPDO->nodeId);
        TEST_ASSERT_EQUAL_UINT8(2, *RPDO->RPDOCommPar_ptrs.maxSubIndex);
        TEST_ASSERT_EQUAL_UINT32(COB_IDUsedByRPDO, *RPDO->RPDOCommPar_ptrs.COB_IDUsedByRPDO);
        TEST_ASSERT_EQUAL_UINT8(transmissionType, *RPDO->RPDOCommPar_ptrs.transmissionType);

        TEST_ASSERT_EQUAL_UINT8(1, *RPDO->RPDOMapPar_ptrs.numberOfMappedObjects);
//        TEST_ASSERT_EQUAL_UINT8(mappedObjects[0], *RPDO->RPDOMapPar_ptrs.mappedObjects[0]);
        TEST_ASSERT_EQUAL_UINT8(tv1, *(uint8_t *) RPDO->mapPointer[0]);
}

void test_tpdo_process(void)
{
        uint8_t node_id = 0x20;
        
        transmissionType = 0xff;
        inhibitTime = 0x0;
        compatibilityEntry = 0x3;
        eventTimer = 0x0;
        SYNCStartValue = 0x6;

        mappedObjects[0] = (0x2000 << 16) | (0x0 << 8) | 0x8;
        mappedObjects[1] = (0x2001 << 16) | (0x0 << 8) | 16;
        mappedObjects[2] = (0x2002 << 16) | (0x0 << 8) | 32;
        mappedObjects[3] = (0x2003 << 16) | (0x0 << 8) | 8;
        
        void *OD = make_rpdo_od_entries(4);
        TEST_ASSERT_NOT_NULL(OD);

        
        CO_RPDO_t *RPDO = init_rpdo(node_id, OD);
        TEST_ASSERT_NOT_NULL(RPDO);


        uint8_t tmp_array[8];
        tmp_array[0] = 10;

        tmp_array[1] = 9;
        tmp_array[2] = 87;

        tmp_array[3] = 1;
        tmp_array[4] = 2;
        tmp_array[5] = 3;
        tmp_array[6] = 4;

        tmp_array[7] = 5;
        
        send_rpdo_messge(RPDO, tmp_array, 8);
        
        CO_RPDO_process(RPDO, true);
        TEST_ASSERT_EQUAL_UINT8(10, tv1);

        TEST_ASSERT_EQUAL_UINT8(9, tv2 & 0xff);
        TEST_ASSERT_EQUAL_UINT8(87, tv2 >> 8);

        TEST_ASSERT_EQUAL_UINT8(1, tv3 & 0xff);
        TEST_ASSERT_EQUAL_UINT8(2, (tv3 >> 8)& 0xff);
        TEST_ASSERT_EQUAL_UINT8(3, (tv3 >> 16)& 0xff);
        TEST_ASSERT_EQUAL_UINT8(4, (tv3 >> 24)& 0xff);        

        TEST_ASSERT_EQUAL_UINT8(5, tv4 & 0xff);

        TEST_ASSERT_EQUAL_UINT32(1, callback_called_cnt);
        
//        TEST_ASSERT_EQUAL_UINT32(COB_IDUsedByTPDO+node_id, last_sent_buffer.ident);
//        TEST_ASSERT_EQUAL_UINT8(1, last_sent_buffer.DLC);
//        TEST_ASSERT_EQUAL_UINT8(a, last_sent_buffer.data[0]);
//
//
//        // if called again should not send tpdo again, because value didnt change
//        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
//        TPDO->sendRequest = CO_TPDOisCOS(TPDO);
//        CO_TPDO_process(TPDO, false, 10);
//        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0]);
//
//        // change the value so new tpdo msg should be sent
//        a += 1;
//        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
//        TPDO->sendRequest = CO_TPDOisCOS(TPDO);        
//        CO_TPDO_process(TPDO, false, 10);
//        TEST_ASSERT_EQUAL_UINT8(a, last_sent_buffer.data[0]);
//
}
