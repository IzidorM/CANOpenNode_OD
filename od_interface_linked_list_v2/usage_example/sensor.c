#include <inttypes.h>
#include <stdlib.h>

#include "sensor_config.h"
#include "sensor.h"


static uint32_t sensors_error_counter = 0;

struct sensor_type01 sensor[NUM_OF_SENSORS];

uint32_t *sensor_get_error_counter(void)
{
        return &sensors_error_counter;
}

struct sensor_type01 *sensor_get_obj_array(void)
{
        return sensor;
}

uint32_t sensor_get_obj_array_size(void)
{
        return NUM_OF_SENSORS;
}

// sensor driver code ...
