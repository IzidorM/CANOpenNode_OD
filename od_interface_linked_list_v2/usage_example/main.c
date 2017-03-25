#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include "canopen_od_interface.h"
#include "sensor_od.h"
#include "canopen_desc_gen.h"

int main(void)
{
        printf("Running OD doc generator\n");
        void *od_root = NULL;
        // call all OD init functions
        sensor_init_od(&od_root);

        uint32_t i;
        uint32_t entries_found = 0;
        for (i = 0x1000; 0xffff > i; i++)
        {
                //printf("searching for %i\n", i);
                void *n = CO_OD_find(od_root, i);
                if (n)
                {
                        con_generate_od_element_description(n);
                        entries_found += 1;
                }
        }
        printf("\nEntries found %i\n", entries_found);
}
