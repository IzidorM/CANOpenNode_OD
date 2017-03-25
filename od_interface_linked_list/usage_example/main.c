#include <inttypes.h>
#include <stdlib.h>
#include "canopen_od_interface.h"

// We have NUM_OF_SENSORS of same sensor type present in our system.
// Sensor is defined as follows:
#define NUM_OF_SENSORS 2

// information about the sensor, read from sensor at init
struct sensor_info {
        uint32_t id;
        uint16_t rev;
};

// sensor measurement information, updated automaticaly by the driver
// when new measurement is available
struct sensor_measurement {
        uint32_t measurement_mm;
        uint8_t measurement_quality_ptc;
        uint8_t time_ms;
};

// main sensor data structure
struct sensor_type01 {
        uint8_t sensor_command; 
        struct sensor_info info;
        struct sensor_measurement m;
};

// 
struct sensor_type01 s1[NUM_OF_SENSORS];

// We want to access all the above data over the canopen
// Because we want to map same type of data from every sensor to the same OD
// element e.g. id from first sensor should be accessed on subindex 1,
// id from second sensor on subindex 2, we will use the record type. The array
// type couldn't be used, because elements are not stored sequentially in the memory.

// Allocate memory and init record type
INIT_OD_RECORD_STATIC(s1_command_record);


// Allocate memory for record subelements
struct con_od_record_subelement s1_cmd_record_subelemets[NUM_OF_SENSORS];

// Allocate space and init OD entry for the record
INIT_OD_ENTRY_STATIC(s1_command, 0x2000, OD_RECORD, &s1_command_record);

INIT_OD_RECORD_STATIC(s1_info_id_record);
INIT_OD_RECORD_STATIC(s1_info_rev_record);
struct con_od_record_subelement s1_info_id_rs[NUM_OF_SENSORS];
struct con_od_record_subelement s1_info_rev_rs[NUM_OF_SENSORS];

INIT_OD_ENTRY_STATIC(s1_info_id, 0x2001, OD_RECORD, &s1_info_id_record);
INIT_OD_ENTRY_STATIC(s1_info_rev, 0x2002, OD_RECORD, &s1_info_rev_record);

int main(void)
{
        uint32_t i;
        for (i = 0; NUM_OF_SENSORS > i; i++)
        {
                // fill command record
                INIT_OD_RECORD_ENTRY(
                        s1_cmd_record_subelemets[i],
                        OD_UINT8,
                        s1[i].sensor_command);

                con_od_add_element_to_record(&s1_command_record,
                                             &s1_cmd_record_subelemets[i]);
                // fill info record
                INIT_OD_RECORD_ENTRY(
                        s1_info_id_rs[i],
                        OD_UINT32,
                        s1[i].info.id);
                INIT_OD_RECORD_ENTRY(
                        s1_info_rev_rs[i],
                        OD_UINT16,
                        s1[i].info.rev);

                con_od_add_element_to_record(&s1_info_id_record,
                                             &s1_info_id_rs[i]);
                con_od_add_element_to_record(&s1_info_rev_record,
                                             &s1_info_rev_rs[i]);
        }
}
