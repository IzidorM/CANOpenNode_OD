#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_driver.h"
#include "CO_HB.h"
#include "CO_Config.h"
#include "CO_OD_interface.h"

extern CO_CANtx_t last_sent_buffer;

void setUp(void)
{
        memset(&last_sent_buffer, 0xff, sizeof(CO_CANtx_t));
}

void tearDown(void)
{
        
}

static CO_HB_t *init_hb(uint32_t node_id, void *OD)
{
        CO_ReturnError_t err;
        // malloc memory for SDO.
        // Dont bother freeing it, we have enough ram on a PC :)
        CO_HB_t *HB = malloc(sizeof(CO_HB_t));

        err = CO_HB_init(
                HB,
                node_id);

        if (err)
        {
                return NULL;
        }
        else
        {
                return HB;
        }
}

void test_HB_init(void)
{
        uint8_t node_id = 0x20;
        struct con_od_list_node_var OD;
        //just init OD entry so next ptr is set to null. 
        INIT_OD_ENTRY_VAR(OD, 0x2000, OD_TYPE_UINT32, CO_ODA_WRITEABLE, &node_id, NULL, NULL);

        CO_HB_t *HB = init_hb(node_id, &OD);
        TEST_ASSERT_NOT_NULL(HB);
        TEST_ASSERT_EQUAL_UINT8(node_id, HB->nodeId);
}

void test_HB_process(void)
{
        uint8_t node_id = 0x20;
        struct con_od_list_node_var OD;
        //just init OD entry so next ptr is set to null. 
        INIT_OD_ENTRY_VAR(OD, 0x2000, OD_TYPE_UINT32, CO_ODA_WRITEABLE, &node_id, NULL, NULL);

        CO_HB_t *HB = init_hb(node_id, &OD);
        TEST_ASSERT_NOT_NULL(HB);

        int16_t HBtime = 1000;
        uint16_t time_passed = 40;
        uint16_t time_passed_sum = 40;
        uint16_t time_until_next_hb = 1000;

        uint8_t operating_state = 0; // 
        
        CO_HB_process(HB, operating_state, time_passed, HBtime, &time_until_next_hb);
        TEST_ASSERT_EQUAL_UINT16(time_passed_sum, HB->HBproducerTimer);
        TEST_ASSERT_EQUAL_UINT16(HBtime - time_passed_sum, time_until_next_hb);
        // hb msg was not send
        TEST_ASSERT_EQUAL_UINT32(last_sent_buffer.ident, 0xffffffff);


        time_passed = 100;
        time_passed_sum += time_passed;
        CO_HB_process(HB, operating_state, time_passed, HBtime, &time_until_next_hb);
        TEST_ASSERT_EQUAL_UINT16(time_passed_sum, HB->HBproducerTimer);
        TEST_ASSERT_EQUAL_UINT16(HBtime - time_passed_sum, time_until_next_hb);
        // hb msg was not send
        TEST_ASSERT_EQUAL_UINT32(last_sent_buffer.ident, 0xffffffff);

        // overshoot the time, expect a new hb msg...
        operating_state = 0x34; //
        time_passed = 1000;
        CO_HB_process(HB, operating_state, time_passed, HBtime, &time_until_next_hb);
        TEST_ASSERT_EQUAL_UINT16(0, HB->HBproducerTimer);
        TEST_ASSERT_EQUAL_UINT16(HBtime - time_passed_sum, time_until_next_hb);
        // hb msg should have right id and data[0]
        TEST_ASSERT_EQUAL_UINT32(last_sent_buffer.ident, 0x700 + node_id);
        TEST_ASSERT_EQUAL_UINT32(last_sent_buffer.data[0], operating_state);
}
