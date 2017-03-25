#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "canopen_od_interface.h"
#include "canopen_od_eds.h"
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

        uint8_t a;
        uint32_t c;
        uint16_t b;
        
        struct con_od_list_node n1;
        INIT_OD_ENTRY(n1, 0x1234, OD_UINT8, 0x0, &a, NULL, NULL);
        
        struct con_od_list_node n2;
        INIT_OD_ENTRY(n2, 0x4321, OD_UINT32, 0x0, &c, NULL, NULL);
        struct con_od_list_node n3;
        INIT_OD_ENTRY(n3, 0x1, OD_UINT16, 0x0, &b, NULL, NULL);

        con_od_add_element_to_od(&n1, &n2);
        con_od_add_element_to_od(&n1, &n3);
        
        struct con_od_list_node *t;

        // test find function for the simple data types
        t = CO_OD_find(&n1, 0x3);
        TEST_ASSERT_NULL(t);

        t = CO_OD_find(&n1, 0x4321);
        TEST_ASSERT_NOT_NULL(t);

        t = CO_OD_find(&n1, 0x1234);
        TEST_ASSERT_NOT_NULL(t);


        // test size function for the simple data types
        uint16_t l = CO_OD_getLength(&n1, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(&n2, 0);
        TEST_ASSERT_EQUAL_UINT16(4, l);

        void *d1 = CO_OD_getDataPointer(&n1, 0xff);
        TEST_ASSERT_EQUAL_PTR(&a, d1);

        void *d2 = CO_OD_getDataPointer(&n2, 0xff);
        TEST_ASSERT_EQUAL_PTR(&c, d2);

}

void test_array_type(void)
{
        uint8_t test_array[5];

        struct con_od_array a8 = {
                .number_of_elements = sizeof(test_array),
                .data = {
                        .element_code = OD_UINT8,
                        .element_data = &test_array,
                }
        };

        struct con_od_list_node n;
        n.head.next_od_element = NULL;
        n.head.index = 0x1234;
        n.data.element_code = OD_ARRAY,
        n.data.element_data = &a8;
        
        uint16_t l = CO_OD_getLength(&n, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(&n, 1);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        // test data
        void *d = CO_OD_getDataPointer(&n, 0x0);
        TEST_ASSERT_EQUAL_PTR(NULL, d);

        d = CO_OD_getDataPointer(&n, 0x1);
        TEST_ASSERT_EQUAL_PTR(test_array, d);

        d = CO_OD_getDataPointer(&n, 0x2);
        TEST_ASSERT_EQUAL_PTR(&test_array[1], d);

        d = CO_OD_getDataPointer(&n, 0x3);
        TEST_ASSERT_EQUAL_PTR(&test_array[2], d);

        
        uint16_t test_array16[5];
        struct con_od_array a16;
        INIT_OD_ARRAY(a16, test_array16, sizeof(test_array16) / 2, OD_UINT16);

        INIT_OD_ENTRY(n, 0x1234, OD_ARRAY, 0x0, &a16, NULL, NULL);

        l = CO_OD_getLength(&n, 1);
        TEST_ASSERT_EQUAL_UINT16(2, l);

        // out of 
        l = CO_OD_getLength(&n, (sizeof(test_array16)/2) + 1);
        TEST_ASSERT_EQUAL_UINT16(0, l);

        d = CO_OD_getDataPointer(&n, 0x1);
        TEST_ASSERT_EQUAL_PTR(&test_array16[0], d);

        d = CO_OD_getDataPointer(&n, 0x2);
        TEST_ASSERT_EQUAL_PTR(&test_array16[1], d);

        d = CO_OD_getDataPointer(&n, 0x3);
        TEST_ASSERT_EQUAL_PTR(&test_array16[2], d);

}

uint8_t test = 0;
INIT_OD_RECORD_STATIC(test_record);
INIT_OD_RECORD_ENTRY_STATIC(test_subelement, OD_UINT8, test);

void test_record_type(void)
{
//        struct con_od_list_node od;
        struct pdo_communication_record {
                uint8_t num_of_entries;
                uint32_t cob_id;
                uint8_t transmission_type;
                uint16_t inhibit_time;
                uint8_t reserved;
                uint16_t event_time;
        };

        struct pdo_communication_record pcr = {0,1,2,3,4,5};

//        struct con_od_record_subelement event_time;
//        INIT_OD_RECORD_ENTRY(event_time, OD_UINT16, pcr.event_time);
////        event_time.head.next_subelement = NULL;
////        event_time.data.element_code = OD_UINT16;
////        event_time.data.element_data = &pcr.event_time;
//        
//        struct con_od_record_subelement reserved;
//        INIT_OD_RECORD_ENTRY(reserved, OD_UINT8, pcr.reserved);
//
//        struct con_od_record_subelement inhibit_time;
//        inhibit_time.next_subelement = &reserved;
//        inhibit_time.data.element_code = OD_UINT16;
//        inhibit_time.data.element_data = &pcr.inhibit_time;
//
//        struct con_od_record_subelement transmission_type;
//        transmission_type.next_subelement = &inhibit_time;
//        transmission_type.data.element_code = OD_UINT8;
//        transmission_type.data.element_data = &pcr.transmission_type;
//        
        con_od_add_element_to_record(&test_record, &test_subelement);



        struct con_od_record pdo_communication_param;
//        pdo_communication_param.first_subelement = NULL;
//        pdo_communication_param.number_of_elements = 5;
        INIT_OD_RECORD(pdo_communication_param);
        
        struct con_od_record_subelement cob_id;
        cob_id.next_subelement = NULL;
        cob_id.data.element_code = OD_UINT32;
        cob_id.data.element_data = &pcr.cob_id;
//        struct con_od_record_subelement cob_id;
//        INIT_OD_RECORD_ENTRY(cob_id, OD_UINT32, pcr.cob_id);

        struct con_od_record_subelement transmission_type;
        INIT_OD_RECORD_ENTRY(transmission_type, OD_UINT8, pcr.transmission_type);

        struct con_od_record_subelement inhibit_time;
        INIT_OD_RECORD_ENTRY(inhibit_time, OD_UINT16, pcr.inhibit_time);

        struct con_od_record_subelement reserved;
        INIT_OD_RECORD_ENTRY(reserved, OD_UINT8, pcr.reserved);
        
        struct con_od_record_subelement event_time;
        INIT_OD_RECORD_ENTRY(event_time, OD_UINT16, pcr.event_time);

        con_od_add_element_to_record(&pdo_communication_param, &cob_id);
        con_od_add_element_to_record(&pdo_communication_param, &transmission_type);
        con_od_add_element_to_record(&pdo_communication_param, &inhibit_time);
        con_od_add_element_to_record(&pdo_communication_param, &reserved);
        con_od_add_element_to_record(&pdo_communication_param, &event_time);
        
        
        struct con_od_list_node n;
        n.head.index = 0x123;
        n.flags = 0x0;
        n.data.element_code = OD_RECORD;
        n.data.element_data = &pdo_communication_param;

        // test size function for the simple data types
        uint16_t l = CO_OD_getLength(&n, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(&n, 1); //cob_id length
        TEST_ASSERT_EQUAL_UINT16(4, l);

        l = CO_OD_getLength(&n, 2); //
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(&n, 3); //
        TEST_ASSERT_EQUAL_UINT16(2, l);

        l = CO_OD_getLength(&n, 4); //
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(&n, 5); //
        TEST_ASSERT_EQUAL_UINT16(2, l);

        l = CO_OD_getLength(&n, 6); //out of range
        TEST_ASSERT_EQUAL_UINT16(0, l);


        // test data
        void *d = CO_OD_getDataPointer(&n, 0x1);
        TEST_ASSERT_EQUAL_PTR(&pcr.cob_id, d);

        d = CO_OD_getDataPointer(&n, 0x2);
        TEST_ASSERT_EQUAL_PTR(&pcr.transmission_type, d);

        d = CO_OD_getDataPointer(&n, 0x3);
        TEST_ASSERT_EQUAL_PTR(&pcr.inhibit_time, d);
        
}

void test_eds(void)
{
        // test basic variable
        uint32_t a = 0x00123400;

        struct con_od_list_node var_test;
        INIT_OD_ENTRY(var_test, 0x4321, OD_UINT32, 0x7, &a,
                      "var_test",
                      "var test is an 32b variable used by...");

        con_generate_od_element_description(&var_test);
        
        // test array
        uint16_t test_array16[5];
        struct con_od_array a16;
        INIT_OD_ARRAY(a16, test_array16, sizeof(test_array16) / 2, OD_UINT16);

        struct con_od_list_node array_test;
        INIT_OD_ENTRY(array_test, 0x1234, OD_ARRAY, 0x0, &a16, NULL, NULL);

        con_generate_od_element_description(&array_test);        


}

