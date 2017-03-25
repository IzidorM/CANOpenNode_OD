#include <inttypes.h>
#include <stdlib.h>

#include "sensor_config.h"
#include "sensor.h"
#include "canopen_od_interface.h"


struct con_od_list_node_var od_sensor_error_counter;

// every sensor variable mapped to OD will be a RECORD, where variable from 
// sensor N is located at sub-index N

// Mapping commands entry
struct con_od_record_entry od_sensor_cmd_record_entries[NUM_OF_SENSORS];
INIT_OD_ENTRY_RECORD_STATIC(od_sensor_cmd, 0x2001, od_sensor_cmd_record_entries,
                            NUM_OF_SENSORS+1,
                            "Sensor command",
                            "\
Writting sensor command id to Nth sub-index will issue the command to Nth sensor...");

// Mapping info id entry
struct con_od_record_entry od_sensor_info_id_record_entries[NUM_OF_SENSORS];
INIT_OD_ENTRY_RECORD_STATIC(od_sensor_info_id, 0x2002, od_sensor_info_id_record_entries,
                            NUM_OF_SENSORS+1,
                            "Sensor info id",
                            "\
Nth sensor info id is mapped to Nth sub-index on this record...");

// Mapping info revision entry
struct con_od_record_entry od_sensor_info_rev_record_entries[NUM_OF_SENSORS];
INIT_OD_ENTRY_RECORD_STATIC(od_sensor_info_rev, 0x2003, od_sensor_info_rev_record_entries,
                            NUM_OF_SENSORS+1,
                            "Sensor info revision",
                            "\
Nth sensor info revision is mapped to Nth sub-index on this record...");



// sensor specific OD init
int sensor_init_od(void *root)
{
        // get actual sensor data object used by the sensor driver
        struct sensor_type01 *sensors = sensor_get_obj_array();

        // Add sensor OD entries to global OD
        con_od_add_element_to_od(root, &od_sensor_cmd);
        con_od_add_element_to_od(root, &od_sensor_info_id);
        con_od_add_element_to_od(root, &od_sensor_info_rev);


        // init sensors error counter OD variable
        INIT_OD_ENTRY_VAR(od_sensor_error_counter,
                          0x2000,
                          OD_TYPE_INT32,
                          CO_ODA_READABLE | CO_ODA_TPDO_MAPABLE,
                          sensor_get_error_counter(),
                          "sensor error counter",
                          "Sum of all errors reported from all sensors");
        
        // adding error counter OD variable to OD
        con_od_add_element_to_od(root, &od_sensor_error_counter);


        // Use loop to init RECORD subelements
        uint32_t i;
        for (i = 0; NUM_OF_SENSORS > i; i++)
        {
                // for every sensor map its command variable to
                //od sensor cmd record subentry
                INIT_RECORD_SUBENTRY(od_sensor_cmd_record_entries[i],
                                     OD_TYPE_UINT8,
                                     CO_ODA_WRITEABLE, //attributes
                                     &sensors[i].sensor_command,
                                     NULL, NULL); // TODO: How to add auto-gen strings?

                // map info id fields
                INIT_RECORD_SUBENTRY(od_sensor_info_id_record_entries[i],
                                     OD_TYPE_UINT32,
                                     CO_ODA_READABLE, //attributes
                                     &sensors[i].info.id,
                                     NULL, NULL); // TODO: How to add auto-gen strings?

                // map info revision fields                
                INIT_RECORD_SUBENTRY(od_sensor_info_rev_record_entries[i],
                                     OD_TYPE_UINT16,
                                     CO_ODA_READABLE, //attributes
                                     &sensors[i].info.rev,
                                     NULL, NULL); // TODO: How to add auto-gen strings?
        }
        return 0;
}
