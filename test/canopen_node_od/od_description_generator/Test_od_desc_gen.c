#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "canopen_od_interface.h"
#include "canopen_desc_gen.h"
#include "unity.h"

void setUp(void)
{
//        test_plogger_reset();
}

void tearDown(void)
{
        
}

void test_test(void)
{
//        int32_t r;

        uint8_t a = 1;
        uint32_t c = 2;
        uint16_t b = 3;
        uint8_t test_array[5];
        struct pdo_communication {
                uint32_t cob_id;
                uint8_t transmission_type;
                uint16_t inhibit_time;
                uint8_t reserved;
                uint16_t event_time;
        };

        struct pdo_communication pcr = {1,2,3,4,5};


        struct con_od_list_node_var v1;
        INIT_OD_ENTRY_VAR(v1, 0x1234, OD_TYPE_UINT8, 0x1, &a,
                          "V1"," This is V1 description...");

        struct con_od_list_node_var v2;
        INIT_OD_ENTRY_VAR(v2, 0x4321, OD_TYPE_UINT32, 0x4, &c, NULL, NULL);

        struct con_od_list_node_var v3;
        INIT_OD_ENTRY_VAR(v3, 0x1, OD_TYPE_UINT16, 0x8, &b, NULL, NULL);

        struct con_od_list_node_array a8;
        INIT_OD_ENTRY_ARRAY(a8, 0x1234, 0x3,
                            sizeof(test_array),
                            OD_TYPE_UINT8, 0x1, &test_array,
                            "array a8",
                             "Test array description...");


        struct con_od_list_node_record od_pdo_communication_record;
        struct con_od_list_node_record *rt = &od_pdo_communication_record;
        struct con_od_record_entry od_pdo_communication_record_subelements[5];
        
        INIT_OD_ENTRY_RECORD(od_pdo_communication_record,
                             0x1800,
                             od_pdo_communication_record_subelements,
                             6,
                             "pdo communication record",
                             " This is description of the pdo comm record...");

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[0],
                             OD_TYPE_UINT32,
                             0x0,
                             &pcr.cob_id,
                             "Cob id",
                             "Cob id is...");

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[1],
                             OD_TYPE_UINT8,
                             0x0,
                             &pcr.transmission_type,
                             "Transmission type",
                             "Transmission time id is...");

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[2],
                             OD_TYPE_UINT16,
                             0x0,
                             &pcr.inhibit_time,
                             "Inhibit_time",
                             "Inhibit time is...");

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[3],
                             OD_TYPE_UINT8,
                             0x0,
                             &pcr.reserved,
                             "Reserved",
                             "Not in use");

        INIT_RECORD_SUBENTRY(od_pdo_communication_record_subelements[4],
                             OD_TYPE_UINT16,
                             0x0,
                             &pcr.event_time,
                             "Event time",
                             "Event time is...");


        
        con_od_add_element_to_od(&v1, &v2);
        con_od_add_element_to_od(&v1, &v3);

        con_generate_od_element_description(&v1);
        con_generate_od_element_description(&v2);
        con_generate_od_element_description(&a8);
        con_generate_od_element_description(rt);
}
