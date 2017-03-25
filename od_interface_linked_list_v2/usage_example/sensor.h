#ifndef SENSOR_H
#define SENSOR_H

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

uint32_t *sensor_get_error_counter(void);
struct sensor_type01 *sensor_get_obj_array(void);
uint32_t sensor_get_obj_array_size(void);

#endif
