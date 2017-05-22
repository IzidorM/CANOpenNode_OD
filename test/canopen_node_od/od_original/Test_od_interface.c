#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "canopen_od_interface.h"

#include "unity.h"

void setUp(void)
{
//        test_plogger_reset();
}

void tearDown(void)
{
}


void test_simple_type(void)
{
//        int32_t r;
        uint32_t a;
        uint8_t b;
        
        CO_OD_entry_t od_array[] = {
                {0x1000, 0x00, 0x85,  4, (void*)&a},
                {0x1001, 0x00, 0x36,  1, (void*)&b},
        };

        struct CO_OD od = {sizeof(od_array) / sizeof(CO_OD_entry_t), od_array};

        CO_OD_entry_t *t;
        t = CO_OD_find(&od, 0x0);
        TEST_ASSERT_NULL(t);

        t = CO_OD_find(&od, 0x1000);
        TEST_ASSERT_NOT_NULL(t);

        t = CO_OD_find(&od, 0x1001);
        TEST_ASSERT_NOT_NULL(t);

        TEST_ASSERT_EQUAL_PTR(&od_array[1], t);
        // test size function for the simple data types
        uint16_t l = CO_OD_getLength(&od_array[0], 0);
        TEST_ASSERT_EQUAL_UINT16(4, l);

        l = CO_OD_getLength(&od_array[1], 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);
        
}


void test_record_type(void)
{
        uint32_t a;
        uint8_t b;

        OD_TPDOCommunicationParameter_t record = {0x6, 0x180L, 0xFF, 0x64, 0x0, 0x0, 0x0};
        const CO_OD_entryRecord_t OD_record1800[7] = {
           {(void*)&record.maxSubIndex, 0x05,  1},
           {(void*)&record.COB_IDUsedByTPDO, 0x8D,  4},
           {(void*)&record.transmissionType, 0x0D,  1},
           {(void*)&record.inhibitTime, 0x8D,  2},
           {(void*)&record.compatibilityEntry, 0x0D,  1},
           {(void*)&record.eventTimer, 0x8D,  2},
           {(void*)&record.SYNCStartValue, 0x0D,  1}};
        
        CO_OD_entry_t od_array[] = {
                {0x1000, 0x00, 0x85,  4, (void*) &a},
                {0x1001, 0x00, 0x36,  1, (void*) &b},
                {0x1800, 0x06, 0x00,  0, (void*) &OD_record1800},
        };
        struct CO_OD od = {sizeof(od_array) / sizeof(CO_OD_entry_t), od_array};

        CO_OD_entry_t *t;
        t = CO_OD_find(&od, 0x1800);
        TEST_ASSERT_NOT_NULL(t);
        TEST_ASSERT_EQUAL_PTR(&od_array[2], t);

        uint16_t l = CO_OD_getLength(t, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(t, 1);
        TEST_ASSERT_EQUAL_UINT16(4, l);
}


//void test_array_type(void)
//{
//
//}
