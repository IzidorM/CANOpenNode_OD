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
#include "CO_core.h"

extern CO_CANtx_t last_sent_buffer;

int32_t callback_called_cnt = 0;



void setUp(void)
{
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        
        callback_called_cnt = 0;
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
        CO_RPDO_receive(obj, &msg);
}

uint32_t rpdo_callback_test(void *d)
{
        callback_called_cnt += 1;
        return 0;
}


void test_PDO_init(void)
{
        int32_t err = 0;
        void *can_driver = 0x1234;
        struct CO_core *c = CO_init(can_driver);


        TEST_ASSERT_NOT_NULL(c);
        
//        CO_RPDO_t *RPDO = init_rpdo(node_id, OD);
//        TEST_ASSERT_NOT_NULL(RPDO);
//        TEST_ASSERT_EQUAL_UINT8(node_id, RPDO->nodeId);
//        TEST_ASSERT_EQUAL_UINT8(2, *RPDO->RPDOCommPar_ptrs.maxSubIndex);
//        TEST_ASSERT_EQUAL_UINT32(COB_IDUsedByRPDO, *RPDO->RPDOCommPar_ptrs.COB_IDUsedByRPDO);
//        TEST_ASSERT_EQUAL_UINT8(transmissionType, *RPDO->RPDOCommPar_ptrs.transmissionType);
//
//        TEST_ASSERT_EQUAL_UINT8(1, *RPDO->RPDOMapPar_ptrs.numberOfMappedObjects);
////        TEST_ASSERT_EQUAL_UINT8(mappedObjects[0], *RPDO->RPDOMapPar_ptrs.mappedObjects[0]);
//        TEST_ASSERT_EQUAL_UINT8(tv1, *(uint8_t *) RPDO->mapPointer[0]);
}

void test_tpdo_process(void)
{
//        uint8_t node_id = 0x20;
//        
//        transmissionType = 0xff;
//        inhibitTime = 0x0;
//        compatibilityEntry = 0x3;
//        eventTimer = 0x0;
//        SYNCStartValue = 0x6;
//
//        mappedObjects[0] = (0x2000 << 16) | (0x0 << 8) | 0x8;
//        mappedObjects[1] = (0x2001 << 16) | (0x0 << 8) | 16;
//        mappedObjects[2] = (0x2002 << 16) | (0x0 << 8) | 32;
//        mappedObjects[3] = (0x2003 << 16) | (0x0 << 8) | 8;
//        
//        void *OD = make_rpdo_od_entries(4);
//        TEST_ASSERT_NOT_NULL(OD);
//
//        
//        CO_RPDO_t *RPDO = init_rpdo(node_id, OD);
//        TEST_ASSERT_NOT_NULL(RPDO);
//
//
//        uint8_t tmp_array[8];
//        tmp_array[0] = 10;
//
//        tmp_array[1] = 9;
//        tmp_array[2] = 87;
//
//        tmp_array[3] = 1;
//        tmp_array[4] = 2;
//        tmp_array[5] = 3;
//        tmp_array[6] = 4;
//
//        tmp_array[7] = 5;
//        
//        send_rpdo_messge(RPDO, tmp_array, 8);
//        
//        CO_RPDO_process(RPDO, true);
//        TEST_ASSERT_EQUAL_UINT8(10, tv1);
//
//        TEST_ASSERT_EQUAL_UINT8(9, tv2 & 0xff);
//        TEST_ASSERT_EQUAL_UINT8(87, tv2 >> 8);
//
//        TEST_ASSERT_EQUAL_UINT8(1, tv3 & 0xff);
//        TEST_ASSERT_EQUAL_UINT8(2, (tv3 >> 8)& 0xff);
//        TEST_ASSERT_EQUAL_UINT8(3, (tv3 >> 16)& 0xff);
//        TEST_ASSERT_EQUAL_UINT8(4, (tv3 >> 24)& 0xff);        
//
//        TEST_ASSERT_EQUAL_UINT8(5, tv4 & 0xff);
//
//        TEST_ASSERT_EQUAL_UINT32(1, callback_called_cnt);
}
