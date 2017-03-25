#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include "canopen_od_interface.h"

// generate OD entry description
void con_generate_od_element_description(void *n)
{
        if (NULL == n)
        {
                return; //TODO: Add error
        }

        uint16_t index = CO_OD_getIndex(n);
        char *name = CO_OD_getEntryName(n);
        char *description = CO_OD_getEntryDescription(n);

//        char *name = CO_OD_getEntryName(n, 0);
//        char *description = CO_OD_getEntryDescription(n, 0);
        uint8_t object_type = CO_OD_getObjectType(n);
        

        printf("\n\n\rIndex: 0x%x\n\r", index);
        if (name)
        {
                printf("Name: %s\n\r", name);
        }
        if (description)
        {
                printf("Description: %s\n\r", description);
        }



        if (OD_OBJECT_TYPE_VAR == object_type)
        {
                uint8_t attributes = CO_OD_getAttribute(n, 0);
                uint8_t data_type = CO_OD_getDataType(n, 0);
                void *default_value = CO_OD_getDataPointer(n,0);

                printf("Object type (VAR): 0x%x\n\r", object_type);                
                printf("Attributes: 0x%x\n\r", attributes);
                if (OD_TYPE_UINT8 == data_type)
                {
                        printf("Data type (uint8): 0x%x\n\r", data_type);
                        printf("Default value: 0x%x\n\r", *((uint8_t *) default_value));
                }
                else if (OD_TYPE_UINT16 == data_type)
                {
                        printf("Data type (uint16): 0x%x\n\r", data_type);
                        printf("Default value: 0x%x\n\r", *((uint16_t *) default_value));
                }
                else if (OD_TYPE_UINT32 == data_type)
                {
                        printf("Data type (uint32): 0x%x\n\r", data_type);
                        printf("Default value: 0x%x\n\r", *((uint32_t *) default_value));
                }
                else if (OD_TYPE_INT8 == data_type)
                {
                        printf("Data type (int8): 0x%x\n\r", data_type);
                        printf("Default value: 0x%x\n\r", *((int8_t *) default_value));
                }
                else if (OD_TYPE_INT16 == data_type)
                {
                        printf("Data type (int16): 0x%x\n\r", data_type);
                        printf("Default value: 0x%x\n\r", *((int16_t *) default_value));
                }
                else if (OD_TYPE_INT32 == data_type)
                {
                        printf("Data type (int32): 0x%x\n\r", data_type);
                        printf("Default value: 0x%x\n\r", *((int32_t *) default_value));
                }
                
        }
        else if (OD_OBJECT_TYPE_ARRAY == object_type)
        {
                uint8_t attributes = CO_OD_getAttribute(n, 0);
                uint8_t data_type = CO_OD_getDataType(n, 1);
                uint8_t array_size = *(uint8_t*) CO_OD_getDataPointer(n, 0);
                
                printf("Object type (ARRAY): 0x%x\n\r", object_type);
                printf("Array size: 0x%x\n\r", array_size);
                printf("Attributes: 0x%x\n\r", attributes);
                if (OD_TYPE_UINT8 == data_type)
                {
                        printf("Data type (uint8): 0x%x\n\r", data_type);
                }
                else if (OD_TYPE_UINT16 == data_type)
                {
                        printf("Data type (uint16): 0x%x\n\r", data_type);
                }
                else if (OD_TYPE_UINT32 == data_type)
                {
                        printf("Data type (uint32): 0x%x\n\r", data_type);
                }
                else if (OD_TYPE_INT8 == data_type)
                {
                        printf("Data type (int8): 0x%x\n\r", data_type);
                }
                else if (OD_TYPE_INT16 == data_type)
                {
                        printf("Data type (int16): 0x%x\n\r", data_type);
                }
                else if (OD_TYPE_INT32 == data_type)
                {
                        printf("Data type (int32): 0x%x\n\r", data_type);
                }

        }
        else if (OD_OBJECT_TYPE_RECORD == object_type)
        {
                printf("Object type (RECORD): 0x%x\n\r", object_type);

                uint8_t record_size = (*(uint8_t*) CO_OD_getDataPointer(n, 0));
                printf("Num of subelements: %i\n\r", record_size); 

                uint32_t i;
                for (i = 0; record_size > i; i++)
                {

                        uint8_t attributes = CO_OD_getAttribute(n, i);
                        uint8_t data_type = CO_OD_getDataType(n, i);
                        void *default_value = CO_OD_getDataPointer(n,i);
                        char *sr_name = CO_OD_getEntrySubindexName(n, i);
                        char *sr_desc = CO_OD_getEntrySubindexDescription(n, i);
                        if (NULL == sr_name)
                        {
                                sr_name = "Name missing";
                        }
                        printf("\n\rRecord subelement %i: %s \n\r", i, sr_name);
                        if (sr_desc)
                        {
                                printf("Subelement description: %s \n\r", sr_desc);
                        }
                        
                        printf("Attributes: 0x%x\n\r", attributes);
                        if (OD_TYPE_UINT8 == data_type)
                        {
                                printf("Data type (uint8): 0x%x\n\r", data_type);
                                printf("Default value: 0x%x\n\r", *((uint8_t *) default_value));
                        }
                        else if (OD_TYPE_UINT16 == data_type)
                        {
                                printf("Data type (uint16): 0x%x\n\r", data_type);
                                printf("Default value: 0x%x\n\r", *((uint16_t *) default_value));
                        }
                        else if (OD_TYPE_UINT32 == data_type)
                        {
                                printf("Data type (uint32): 0x%x\n\r", data_type);
                                printf("Default value: 0x%x\n\r", *((uint32_t *) default_value));
                        }
                        else if (OD_TYPE_INT8 == data_type)
                        {
                                printf("Data type (int8): 0x%x\n\r", data_type);
                                printf("Default value: 0x%x\n\r", *((int8_t *) default_value));
                        }
                        else if (OD_TYPE_INT16 == data_type)
                        {
                                printf("Data type (int16): 0x%x\n\r", data_type);
                                printf("Default value: 0x%x\n\r", *((int16_t *) default_value));
                        }
                        else if (OD_TYPE_INT32 == data_type)
                        {
                                printf("Data type (int32): 0x%x\n\r", data_type);
                                printf("Default value: 0x%x\n\r", *((int32_t *) default_value));
                        }
                }
        }
        else
        {
                printf("Unknown data type\n\r");
        }

}


