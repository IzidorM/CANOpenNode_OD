#ifndef CANOPEN_OD_INTERFACE_H
#define CANOPEN_OD_INTERFACE_H

#include <inttypes.h>

enum con_od_data_type {
        OD_UINT8,
        OD_UINT16,
        OD_UINT32,

        OD_INT8,
        OD_INT16,
        OD_INT32,

        OD_REAL32,
        OD_REAL64,

        OD_ARRAY,

        OD_RECORD,
        // different record implementations possibility
        OD_RECORD_LIST,  // record subelements are in linked list
        OD_RECORD_ARRAY, // record subelements are in an array
        // add types
};


/**
 * Object Dictionary attributes. Bit masks for attribute in CO_OD_entry_t.
 */
typedef enum{
    CO_ODA_MEM_ROM          = 0x0001U,  /**< Variable is located in ROM memory */
    CO_ODA_MEM_RAM          = 0x0002U,  /**< Variable is located in RAM memory */
    CO_ODA_MEM_EEPROM       = 0x0003U,  /**< Variable is located in EEPROM memory */
    CO_ODA_READABLE         = 0x0004U,  /**< SDO server may read from the variable */
    CO_ODA_WRITEABLE        = 0x0008U,  /**< SDO server may write to the variable */
    CO_ODA_RPDO_MAPABLE     = 0x0010U,  /**< Variable is mappable for RPDO */
    CO_ODA_TPDO_MAPABLE     = 0x0020U,  /**< Variable is mappable for TPDO */
    CO_ODA_TPDO_DETECT_COS  = 0x0040U,  /**< If variable is mapped to any PDO, then
                                             PDO is automatically send, if variable
                                             changes its value */
    CO_ODA_MB_VALUE         = 0x0080U   /**< True when variable is a multibyte value */
} CO_SDO_OD_attributes_t;


struct con_od_entry_description {
        char * entry_name;
        char * entry_description;
};

struct con_od_data {
        uint8_t element_code;
        uint8_t attributes;
        void *element_data;
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif

};

// array type
struct con_od_array {
        uint8_t number_of_elements;
        struct con_od_data data;
};

// record type
struct con_od_record_subelement {
        struct con_od_record_subelement *next_subelement;
        struct con_od_data data;
};

struct con_od_record {
        struct con_od_record_subelement *first_subelement;
        uint8_t number_of_elements;
};

struct con_od_list_node_generic {
        struct con_od_list_node *next_od_element;
        uint16_t index;
};

struct con_od_list_node {
        struct con_od_list_node_generic head;
        uint32_t flags; //access rights, ...
        struct con_od_data data;
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};


// NEW interface 
//struct con_od_entry_description {
//        char * entry_name;
//        char * entry_description;
//};
//
//struct con_od_data {
//        uint8_t element_code;
//        uint8_t attributes;
//        void *element_data;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//
//};
//
//// array type
//struct con_od_array {
//        uint8_t element_code;
//        uint8_t attributes;
//        uint8_t number_of_elements;
//        uint8_t subelement_code;
//        void *data;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//};
//
//// record type
////struct con_od_record_subelement {
////        struct con_od_record_subelement *next_subelement;
////        struct con_od_record_subelement subelements[];
////        struct con_od_data data;
////};
//
//struct con_od_record {
//        uint8_t element_code;
////        struct con_od_record_subelement *first_subelement;
//        uint8_t number_of_elements;
//        struct con_od_data *subelements;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//
//};
//
////enum con_od_entry_type {
////        CON_OD_ENTRY_TYPE_BASIC,
////        CON_OD_ENTRY_TYPE_EXTENDED,
////};
//
//struct con_od_list_node_generic {
//        struct con_od_list_node *next_od_element;
//        uint16_t index;
//        uint8_t element_code;
//        uint8_t attributes;
//};
//
//struct con_od_list_node {
//        struct con_od_list_node_generic head;
//        void *data;
//};

// NEW NEW interface
//struct con_od_list_node_generic {
//        struct con_od_list_node *next_od_element;
//        uint16_t index;
//        uint8_t element_code;
//        uint8_t attributes;
//};
//
//struct con_od_record_entry {
//        uint8_t element_code;
//        uint8_t attributes;
//        void *element_data;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//};
//
//struct con_od_list_node_record {
//        struct con_od_list_node_generic head;
//        uint8_t number_of_elements;
//        struct con_od_record_entry *subelements;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//};
//
//struct con_od_list_node_array {
//        struct con_od_list_node_generic head;
//        uint8_t number_of_elements;
//        uint8_t array_element_code;
//        void *data;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//};
//
//struct con_od_list_node_var {
//        struct con_od_list_node_generic head;
//        void *element_data;
//#ifdef USE_OD_DESCRIPTION
//        struct con_od_entry_description description;
//#endif
//};

//struct con_od_list_node_with_callback {
//        struct con_od_list_node_generic head;
//        uint32_t flags; //access rights, ...
//        void (*callback) (void);
//        struct con_od_data data;
//#ifdef USE_OD_DESCRIPTION
//        char * description;
//#endif
//};

#define CON_OD_ELEMENT struct con_od_list_node
#define CON_OD struct con_od_list_node

CON_OD_ELEMENT *CO_OD_find(
        CON_OD *n,
        uint16_t const index);

uint16_t CO_OD_getLength(CON_OD_ELEMENT const *t,
                         uint8_t const sub_index);

void *CO_OD_getDataPointer(CON_OD_ELEMENT const *t,
                           uint8_t const sub_index);

uint8_t CO_OD_getFlagsPointer(CON_OD_ELEMENT const *t);

uint16_t CO_OD_getIndex(CON_OD_ELEMENT const *t);

char* CO_OD_getEntryName(CON_OD_ELEMENT const *t);
char* CO_OD_getEntryDescription(CON_OD_ELEMENT const *t);

int32_t con_od_add_element_to_od(CON_OD *root,
                                 struct con_od_list_node *element);

int32_t con_od_add_element_to_record(
        struct con_od_record *root,
        struct con_od_record_subelement *element);

        
#ifdef USE_OD_DESCRIPTION
#define INIT_OD_ENTRY(node, od_index, type, _attr, data_ptr, _description_name, _description) \
        do {                                          \
                node.head.next_od_element = NULL;     \
                node.head.index = od_index;           \
                node.data.element_code = type;        \
                node.data.attributes = _attr;        \
                node.data.element_data = data_ptr;    \
                node.description.entry_name = _description_name; \
                node.description.entry_description = _description; \
        } while(0)                                          

#define INIT_OD_ENTRY_STATIC(node, od_index, type, data_ptr, _description_name, _description)\
        struct con_od_list_node node = {                                \
                .head = {                                               \
                        .next_od_element = NULL,                        \
                        .index = od_index,                              \
        },                                                              \
                .data = {                                               \
                        .element_code = type,                           \
                        .element_data = data_ptr,                       \
        },                                                              \
}

#define INIT_OD_RECORD_ENTRY(od_entry, _type, _data) \
        do { \
        od_entry.next_subelement = NULL; \
        od_entry.data.element_code = _type; \
        od_entry.data.element_data = &_data; \
        } while(0)

#define INIT_OD_RECORD_ENTRY_STATIC(od_entry, _type, _data)     \
        static struct con_od_record_subelement od_entry = { \
                .next_subelement = NULL, \
                .data = { \
                .element_code = _type, \
                .element_data = &_data \
                } \
        } \


#define INIT_OD_ARRAY(od_array, array_data, array_size, type) \
        do { \
        od_array.number_of_elements = array_size; \
        od_array.data.element_code = type; \
        od_array.data.element_data = &array_data; \
        } while(0)

// TODO: check the spec about missing elements 
#define INIT_OD_RECORD(_record) \
        do { \
        _record.first_subelement = NULL; \
        _record.number_of_elements = 1;  \
        } while(0)

#define INIT_OD_RECORD_STATIC(_record) \
        static struct con_od_record _record = { \
                .first_subelement = NULL,                          \
                .number_of_elements = 1,                           \
        }


#else
#define INIT_OD_ENTRY(node, od_index, type, data_ptr, _description_name, _description) \
        do {                                          \
                node.head.next_od_element = NULL;     \
                node.head.index = od_index;           \
                node.data.element_code = type;        \
                node.data.element_data = data_ptr;    \
        } while(0)                                          

#define INIT_OD_ENTRY_STATIC(node, od_index, type, data_ptr, _description_name, _description)\
        struct con_od_list_node node = {                                \
                .head = {                                               \
                        .next_od_element = NULL,                        \
                        .index = od_index,                              \
        },                                                              \
                .data = {                                               \
                        .element_code = type,                           \
                        .element_data = data_ptr,                       \
        },                                                              \
        }

#define INIT_OD_RECORD_ENTRY(od_entry, _type, _data) \
        do { \
        od_entry.next_subelement = NULL; \
        od_entry.data.element_code = _type; \
        od_entry.data.element_data = &_data; \
        } while(0)

#define INIT_OD_RECORD_ENTRY_STATIC(od_entry, _type, _data)     \
        static struct con_od_record_subelement od_entry = { \
                .next_subelement = NULL, \
                .data = { \
                .element_code = _type, \
                .element_data = &_data \
                } \
        } \


#define INIT_OD_ARRAY(od_array, array_data, array_size, type) \
        do { \
        od_array.number_of_elements = array_size; \
        od_array.data.element_code = type; \
        od_array.data.element_data = &array_data; \
        } while(0)

// TODO: check the spec about missing elements 
#define INIT_OD_RECORD(_record) \
        do { \
        _record.first_subelement = NULL; \
        _record.number_of_elements = 1;  \
        } while(0)

#define INIT_OD_RECORD_STATIC(_record) \
        static struct con_od_record _record = { \
                .first_subelement = NULL,                          \
                .number_of_elements = 1,                           \
        }
#endif
                  
#endif
