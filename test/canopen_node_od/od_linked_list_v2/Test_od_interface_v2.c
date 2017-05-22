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

        uint8_t a;
        uint32_t c;
        uint16_t b;
        
        struct con_od_list_node_var v1;
        INIT_OD_ENTRY_VAR(v1, 0x1234, OD_TYPE_UINT8, 0x1, &a, NULL, NULL);
        
        struct con_od_list_node_var v2;
        INIT_OD_ENTRY_VAR(v2, 0x4321, OD_TYPE_UINT32, 0x4, &c, NULL, NULL);

        INIT_OD_ENTRY_VAR_STATIC(v3, 0x1, OD_TYPE_UINT16, 0x8, &b, NULL, NULL);

        con_od_add_element_to_od(&v1, &v2);
        con_od_add_element_to_od(&v1, &v3);
        
        struct con_od_list_node *t;

        // test find function for the simple data types
        t = CO_OD_find(&v1, 0x3);
        TEST_ASSERT_NULL(t);

        t = CO_OD_find(&v1, 0x4321);
        TEST_ASSERT_NOT_NULL(t);

        t = CO_OD_find(&v1, 0x1234);
        TEST_ASSERT_NOT_NULL(t);


        // test get index
        TEST_ASSERT_EQUAL_UINT16(v1.head.index, CO_OD_getIndex(&v1));
        TEST_ASSERT_EQUAL_UINT16(v3.head.index, CO_OD_getIndex(&v3));

        // test get attributes
        TEST_ASSERT_EQUAL_UINT16(v1.head.attributes, CO_OD_getAttribute(&v1, 0));
        TEST_ASSERT_EQUAL_UINT16(v2.head.attributes, CO_OD_getAttribute(&v2, 0));
        TEST_ASSERT_EQUAL_UINT16(v3.head.attributes, CO_OD_getAttribute(&v3, 0));

        TEST_ASSERT_EQUAL_UINT16(OD_OBJECT_TYPE_VAR, CO_OD_getObjectType(&v1));

        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT8,
                                 CO_OD_getDataType(&v1, 0));

        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT32,
                                 CO_OD_getDataType(&v2, 0));

        
        // test size function for the simple data types
        uint16_t l = CO_OD_getLength(&v1, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(&v2, 0);
        TEST_ASSERT_EQUAL_UINT16(4, l);

        l = CO_OD_getLength(&v3, 0);
        TEST_ASSERT_EQUAL_UINT16(2, l);

        
        void *d1 = CO_OD_getDataPointer(&v1, 0xff);
        TEST_ASSERT_EQUAL_PTR(&a, d1);

        void *d2 = CO_OD_getDataPointer(&v2, 0xff);
        TEST_ASSERT_EQUAL_PTR(&c, d2);

        void *d3 = CO_OD_getDataPointer(&v3, 0xff);
        TEST_ASSERT_EQUAL_PTR(&b, d3);
}

void test_array_type(void)
{
        uint8_t test_array[5];

        struct con_od_list_node_array a8;
        INIT_OD_ENTRY_ARRAY(a8, 0x1234, 0x3,
                            sizeof(test_array),
                            OD_TYPE_UINT8, 0x1, &test_array, NULL, NULL);

        uint16_t l = CO_OD_getLength(&a8, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);


        // test get attributes
        TEST_ASSERT_EQUAL_UINT16(a8.head.attributes, CO_OD_getAttribute(&a8, 0));
        TEST_ASSERT_EQUAL_UINT16(a8.array_elements_attributes , CO_OD_getAttribute(&a8, 1));
        TEST_ASSERT_EQUAL_UINT16(a8.array_elements_attributes, CO_OD_getAttribute(&a8, 2));
        TEST_ASSERT_EQUAL_UINT16(0, CO_OD_getAttribute(&a8, sizeof(a8) + 1));

        l = CO_OD_getLength(&a8, 1);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        // test data
        void *d = CO_OD_getDataPointer(&a8, 0x0);
        TEST_ASSERT_EQUAL_PTR(&a8.number_of_elements , d);

        d = CO_OD_getDataPointer(&a8, 0x1);
        TEST_ASSERT_EQUAL_PTR(test_array, d);

        d = CO_OD_getDataPointer(&a8, 0x2);
        TEST_ASSERT_EQUAL_PTR(&test_array[1], d);

        d = CO_OD_getDataPointer(&a8, 0x3);
        TEST_ASSERT_EQUAL_PTR(&test_array[2], d);

        TEST_ASSERT_EQUAL_UINT16(OD_OBJECT_TYPE_ARRAY, CO_OD_getObjectType(&a8));

        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT8,
                                 CO_OD_getDataType(&a8, 1));
        
        uint16_t test_array16[5];
        struct con_od_list_node_array a16;
        INIT_OD_ENTRY_ARRAY(a16, 0x1234, 0x0,
                            sizeof(test_array16) / 2,
                            OD_TYPE_UINT16, 0x1, &test_array16, NULL, NULL);

        
        l = CO_OD_getLength(&a16, 1);
        TEST_ASSERT_EQUAL_UINT16(2, l);

        // out of 
        l = CO_OD_getLength(&a16, (sizeof(test_array16)/2) + 1);
        TEST_ASSERT_EQUAL_UINT16(0, l);

        d = CO_OD_getDataPointer(&a16, 0x1);
        TEST_ASSERT_EQUAL_PTR(&test_array16[0], d);

        d = CO_OD_getDataPointer(&a16, 0x2);
        TEST_ASSERT_EQUAL_PTR(&test_array16[1], d);

        d = CO_OD_getDataPointer(&a16, 0x3);
        TEST_ASSERT_EQUAL_PTR(&test_array16[2], d);

        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT16,
                                 CO_OD_getDataType(&a16, 1));

}

void test_record_type(void)
{
//        struct con_od_list_node od;
        struct pdo_communication {
                uint32_t cob_id;
                uint8_t transmission_type;
                uint16_t inhibit_time;
                uint8_t reserved;
                uint16_t event_time;
        };

        struct example_record_data {
                uint8_t a;
                uint16_t b;
                uint32_t c;
        };

        struct pdo_communication pcr = {1,2,3,4,5};
        struct example_record_data e = {0xf0, 0xff00, 0xff00ff00};

        // make OD record entry and fill it up with pcr struct elements
        struct con_od_list_node_record od_pdo_communication_record;
        struct con_od_list_node_record *rt = &od_pdo_communication_record;
        struct con_od_record_entry od_pdo_communication_record_subelements[5];
        struct con_od_record_entry *rst = &od_pdo_communication_record_subelements[0];
        
        INIT_OD_ENTRY_RECORD(od_pdo_communication_record,
                             0x1800,
                             od_pdo_communication_record_subelements,
                             6,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[0],
                             OD_TYPE_UINT32,
                             0x0,
                             &pcr.cob_id,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[1],
                             OD_TYPE_UINT8,
                             0x0,
                             &pcr.transmission_type,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[2],
                             OD_TYPE_UINT16,
                             0x0,
                             &pcr.inhibit_time,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[3],
                             OD_TYPE_UINT8,
                             0x0,
                             &pcr.reserved,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[4],
                             OD_TYPE_UINT16,
                             0x0,
                             &pcr.event_time,
                             NULL, NULL);


        // make OD record entry and fill it up with example struct elements

        struct con_od_record_entry ers[3];
        INIT_OD_ENTRY_RECORD_STATIC(er, 0x1, ers, 4,
                                    "example entry", "Example description");

        INIT_RECORD_SUBENTRY(ers[0],
                             OD_TYPE_UINT8,
                             0x0,
                             &e.a,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(ers[1],
                             OD_TYPE_UINT16,
                             0x0,
                             &e.b,
                             NULL, NULL);

        INIT_RECORD_SUBENTRY(ers[2],
                             OD_TYPE_UINT32,
                             0x0,
                             &e.c,
                             NULL, NULL);
        
        
        uint16_t attr = CO_OD_getAttribute(rt, 0);
        TEST_ASSERT_EQUAL_UINT16(od_pdo_communication_record.head.attributes, attr);

        attr = CO_OD_getAttribute(&er, 0);
        TEST_ASSERT_EQUAL_UINT16(er.head.attributes, attr);
        
        attr = CO_OD_getAttribute(rt, 1);
        TEST_ASSERT_EQUAL_UINT16(rst[0].attributes, attr);

        attr = CO_OD_getAttribute(rt, 4);
        TEST_ASSERT_EQUAL_UINT16(rst[3].attributes, attr);

        attr = CO_OD_getAttribute(rt, rt->number_of_elements);

        TEST_ASSERT_EQUAL_UINT16(rst[rt->number_of_elements - 1].attributes, attr);

        attr = CO_OD_getAttribute(rt, rt->number_of_elements + 1);
        TEST_ASSERT_EQUAL_UINT16(0, attr);


// object type
        TEST_ASSERT_EQUAL_UINT8(OD_OBJECT_TYPE_RECORD,
                                 CO_OD_getObjectType(rt));

        TEST_ASSERT_EQUAL_UINT8(OD_OBJECT_TYPE_RECORD,
                                 CO_OD_getObjectType(&er));


// data type
        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT8,
                                 CO_OD_getDataType(rt, 0));

        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT32,
                                 CO_OD_getDataType(rt, 1));

        
        TEST_ASSERT_EQUAL_UINT16(OD_TYPE_UINT16,
                                 CO_OD_getDataType(rt, 5));

        TEST_ASSERT_EQUAL_UINT32(OD_TYPE_UINT32,
                                 CO_OD_getDataType(&er, 3));

        

        
        // test size function for the simple data types
        uint16_t l = CO_OD_getLength(rt, 0);
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(rt, 1); //cob_id length
        TEST_ASSERT_EQUAL_UINT16(4, l);

        l = CO_OD_getLength(rt, 2); //
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(rt, 3); //
        TEST_ASSERT_EQUAL_UINT16(2, l);

        l = CO_OD_getLength(rt, 4); //
        TEST_ASSERT_EQUAL_UINT16(1, l);

        l = CO_OD_getLength(rt, 5); //
        TEST_ASSERT_EQUAL_UINT16(2, l);

        l = CO_OD_getLength(rt, 6); //out of range
        TEST_ASSERT_EQUAL_UINT16(0, l);

//        // test data
        void *d = CO_OD_getDataPointer(rt, 0x1);
        TEST_ASSERT_EQUAL_PTR(&pcr.cob_id, d);

        d = CO_OD_getDataPointer(rt, 0x2);
        TEST_ASSERT_EQUAL_PTR(&pcr.transmission_type, d);

        d = CO_OD_getDataPointer(rt, 0x3);
        TEST_ASSERT_EQUAL_PTR(&pcr.inhibit_time, d);

        d = CO_OD_getDataPointer(&er, 0x3);
        TEST_ASSERT_EQUAL_PTR(&e.c, d);

}


