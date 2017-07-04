#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_TPDO.h"
#include "CO_Config.h"
#include "CO_OD_interface.h"
#include "CO_driver.h"

extern CO_CANtx_t last_sent_buffer;
uint32_t sdo_test_callback_call_count = 0;

uint32_t     COB_IDUsedByTPDO = CO_CAN_ID_TPDO_1;
uint8_t      transmissionType = 0x1;
uint16_t     inhibitTime = 0x0102;
uint8_t      compatibilityEntry = 0x3;
uint16_t     eventTimer = 0x0405;
uint8_t      SYNCStartValue = 0x6;

uint32_t     mappedObjects[8];
uint8_t a = 0x55;

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

        a = 0x55;
//        test_plogger_reset();
}

void tearDown(void)
{
        
}

void * make_pdo_od_entries(void)
{
        int32_t err;
        void *OD;
        // Make a OD record
//        struct con_od_record_entry OD_1800_subelements[7];
        struct con_od_record_entry *OD_1800_subelements = malloc(sizeof(struct con_od_record_entry) * 7);
        INIT_RECORD_SUBENTRY(&OD_1800_subelements[0],
                             OD_TYPE_UINT32,
                             0x8D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &COB_IDUsedByTPDO, "", "");

        INIT_RECORD_SUBENTRY(&OD_1800_subelements[1],
                             OD_TYPE_UINT32,
                             0x0D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &transmissionType, "", "");

        INIT_RECORD_SUBENTRY(&OD_1800_subelements[2],
                             OD_TYPE_UINT32,
                             0x8D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &inhibitTime, "", "");

        INIT_RECORD_SUBENTRY(&OD_1800_subelements[3],
                             OD_TYPE_UINT32,
                             0x0D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &compatibilityEntry, "", "");

        INIT_RECORD_SUBENTRY(&OD_1800_subelements[4],
                             OD_TYPE_UINT32,
                             0x8D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                             &eventTimer, "", "");

        INIT_RECORD_SUBENTRY(&OD_1800_subelements[5],
                             OD_TYPE_UINT32,
                             0x0D,
                             &SYNCStartValue, "", "");
        
//        struct con_od_list_node_record OD_1800;
        struct con_od_list_node_record *OD_1800 = malloc(sizeof(struct con_od_list_node_record));
        INIT_OD_ENTRY_RECORD(OD_1800, 0x1800, &OD_1800_subelements[0], 6, "", "");

        OD = OD_1800;
        
//        struct con_od_record_entry OD_1A00_subelements[8];
        struct con_od_record_entry *OD_1A00_subelements = malloc(sizeof(struct con_od_record_entry) * 8);

        uint32_t i;
        for (i = 0; 8 > i; i++)
        {
                INIT_RECORD_SUBENTRY(&OD_1A00_subelements[i],
                                     OD_TYPE_UINT32,
                                     0x8D, //CO_ODA_WRITEABLE | CO_ODA_READABLE,
                                     &mappedObjects[i], "", "");
        }

//        struct con_od_list_node_record OD_1A00;
        struct con_od_list_node_record *OD_1A00 = malloc(sizeof(struct con_od_list_node_record));
        INIT_OD_ENTRY_RECORD(OD_1A00, 0x1A00, &OD_1A00_subelements[0], 1, "", "");
        // Add OD elements together in a OD linked list
        err = con_od_add_element_to_od(&OD, OD_1A00);
        TEST_ASSERT_EQUAL_UINT32(0, err);

        // add 0x2000 OD entry, so the map above will be valid

//        struct con_od_list_node_var test_var;
        struct con_od_list_node_var *test_var = malloc(sizeof(struct con_od_list_node_var)); 
        INIT_OD_ENTRY_VAR(test_var, 0x2000, OD_TYPE_UINT8,
                          CO_ODA_READABLE | CO_ODA_TPDO_MAPABLE | CO_ODA_TPDO_DETECT_COS,
                          &a, NULL, NULL);

        err = con_od_add_element_to_od(&OD, test_var);
        TEST_ASSERT_EQUAL_UINT32(0, err);
        return OD;
}

static CO_TPDO_t *init_pdo(uint32_t node_id, void *OD)
{
        CO_ReturnError_t err;
        // malloc memory.
        // Dont bother freeing it, we have enough ram on a PC :)
        CO_TPDO_t *TPDO = malloc(sizeof(CO_TPDO_t));
        CO_CANrx_t *rxArray = malloc(sizeof(CO_CANrx_t));
        CO_CANtx_t *txArray = malloc(sizeof(CO_CANtx_t));;
        CO_CANmodule_t *NOT_USED = malloc(sizeof(CO_CANmodule_t));

        NOT_USED->rxArray = rxArray;
        NOT_USED->txArray = txArray;

        err = CO_TPDO_init(
                TPDO,
                OD,
                node_id,
                0, // restriction flags
                0x1800,
                0x1A00,
                NOT_USED);
        if (err)
        {
                return NULL;
        }
        else
        {
                return TPDO;
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

        void *OD = make_pdo_od_entries();
        TEST_ASSERT_NOT_NULL(OD);
        
        CO_TPDO_t *TPDO = init_pdo(node_id, OD);
        TEST_ASSERT_NOT_NULL(TPDO);
//        TEST_ASSERT_EQUAL_PTR(&OD, PDO->OD);
        TEST_ASSERT_EQUAL_UINT8(node_id, TPDO->nodeId);        
        TEST_ASSERT_EQUAL_UINT8(6, *TPDO->TPDOCommPar_ptrs.maxSubIndex);
        TEST_ASSERT_EQUAL_UINT32(COB_IDUsedByTPDO, *TPDO->TPDOCommPar_ptrs.COB_IDUsedByTPDO);
        TEST_ASSERT_EQUAL_UINT8(transmissionType, *TPDO->TPDOCommPar_ptrs.transmissionType);
        TEST_ASSERT_EQUAL_UINT16(inhibitTime, *TPDO->TPDOCommPar_ptrs.inhibitTime);
        TEST_ASSERT_EQUAL_UINT16(eventTimer, *TPDO->TPDOCommPar_ptrs.eventTimer);
        TEST_ASSERT_EQUAL_UINT16(SYNCStartValue, *TPDO->TPDOCommPar_ptrs.SYNCStartValue);

        TEST_ASSERT_EQUAL_UINT8(1, *TPDO->TPDOMapPar_ptrs.numberOfMappedObjects);
        TEST_ASSERT_EQUAL_UINT8(mappedObjects[0], *TPDO->TPDOMapPar_ptrs.mappedObjects[0]);
        TEST_ASSERT_EQUAL_UINT8(a, *(uint8_t *) TPDO->mapPointer[0]);

        
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

        void *OD = make_pdo_od_entries();
        TEST_ASSERT_NOT_NULL(OD);

        
        CO_TPDO_t *TPDO = init_pdo(node_id, OD);
        TEST_ASSERT_NOT_NULL(TPDO);

//        CO_TPDOsend(TPDO);
        CO_TPDO_process(TPDO, false, 10);
        TEST_ASSERT_EQUAL_UINT32(COB_IDUsedByTPDO+node_id, last_sent_buffer.ident);
        TEST_ASSERT_EQUAL_UINT8(1, last_sent_buffer.DLC);
        TEST_ASSERT_EQUAL_UINT8(a, last_sent_buffer.data[0]);


        // if called again should not send tpdo again, because value didnt change
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        TPDO->sendRequest = CO_TPDOisCOS(TPDO);
        CO_TPDO_process(TPDO, false, 10);
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0]);

        // change the value so new tpdo msg should be sent
        a += 1;
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        TPDO->sendRequest = CO_TPDOisCOS(TPDO);        
        CO_TPDO_process(TPDO, false, 10);
        TEST_ASSERT_EQUAL_UINT8(a, last_sent_buffer.data[0]);

}
