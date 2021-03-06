#ifndef CANOPEN_OD_INTERFACE_H
#define CANOPEN_OD_INTERFACE_H

#include <inttypes.h>
#include <stdbool.h>

enum con_od_entry_id {
        OD_VAR,
        OD_VAR_WITH_CALLBACK,
        OD_DOMAIN,
        OD_ARRAY,

        OD_RECORD,
        // different record implementations possibility
        OD_RECORD_LIST,  // record subelements are in linked list
        OD_RECORD_ARRAY, // record subelements are in an array
        // add types
};

// OD object codes as defined in CiA 301, Table 42 Object dictionary object definitions
enum con_od_object_code {
        OD_OBJECT_TYPE_NULL = 0,
        OD_OBJECT_TYPE_DOMAIN = 0x2,
        OD_OBJECT_TYPE_DEFTYPE = 0x5,
        OD_OBJECT_TYPE_DEFSTRUCT = 0x6,
        OD_OBJECT_TYPE_VAR = 0x7,
        OD_OBJECT_TYPE_ARRAY = 0x8,
        OD_OBJECT_TYPE_RECORD = 0x9,
};

// OD object codes as defined in CiA 301, Table 44 Object dictionary data types
enum con_od_data_type {
        OD_TYPE_BOOL = 0x1,
        OD_TYPE_INT8 = 0x2,
        OD_TYPE_INT16 = 0x3,
        OD_TYPE_INT32 = 0x4,
        OD_TYPE_UINT8 = 0x5,
        OD_TYPE_UINT16 = 0x6,
        OD_TYPE_UINT32 = 0x7,
        // TODO: Add the rest of the defined data types
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

/**
 * Object contains all information about the object being transferred by SDO server.
 *
 * Object is used as an argument to @ref CO_SDO_OD_function. It is also
 * part of the CO_SDO_t object.
 */
typedef struct{
    /** Informative parameter. It may point to object, which is connected
    with this OD entry. It can be used inside @ref CO_SDO_OD_function, ONLY
    if it was registered by CO_OD_configure() function before. */
    void               *object;
    /** SDO data buffer contains data, which are exchanged in SDO transfer.
    @ref CO_SDO_OD_function may verify or manipulate that data before (after)
    they are written to (read from) Object dictionary. Data have the same
    endianes as processor. Pointer must NOT be changed. (Data up to length
    can be changed.) */
    uint8_t            *data;
    /** Pointer to location in object dictionary, where data are stored.
    (informative reference to old data, read only). Data have the same
    endianes as processor. If data type is Domain, this variable is null. */
    const void         *ODdataStorage;
    /** Length of data in the above buffer. Read only, except for domain. If
    data type is domain see @ref CO_SDO_OD_function for special rules by upload. */
    uint16_t            dataLength;
    /** Attribute of object in Object dictionary (informative, must NOT be changed). */
    uint16_t            attribute;
    /** Pointer to the #CO_SDO_OD_flags_t byte. */
//    uint8_t            *pFlags;
    /** Index of object in Object dictionary (informative, must NOT be changed). */
    uint16_t            index;
    /** Subindex of object in Object dictionary (informative, must NOT be changed). */
    uint8_t             subIndex;
    /** True, if SDO upload is in progress, false if SDO download is in progress. */
    bool              reading;
    /** Used by domain data type. Indicates the first segment. Variable is informative. */
    bool              firstSegment;
    /** Used by domain data type. If false by download, then application will
    receive more segments during SDO communication cycle. If uploading,
    application may set variable to false, so SDO server will call
    @ref CO_SDO_OD_function again for filling the next data. */
    bool              lastSegment;
    /** Used by domain data type. By upload @ref CO_SDO_OD_function may write total
    data length, so this information will be send in SDO upload initiate phase. It
    is not necessary to specify this variable. By download this variable contains
    total data size, if size is indicated in SDO download initiate phase */
    uint32_t            dataLengthTotal;
    /** Used by domain data type. In case of multiple segments, this indicates the offset
    into the buffer this segment starts at. */
    uint32_t            offset;
}CO_ODF_arg_t;


struct con_od_entry_description {
        char * entry_name;
        char * entry_description;
};

struct con_od_list_node_generic {
        void *next_od_element;
        uint16_t index;
        uint8_t element_id;
        uint8_t attributes;
};

struct con_od_record_entry {
        uint8_t element_data_type;
        uint8_t attributes;
        void *data;
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};

struct con_od_list_node_record {
        struct con_od_list_node_generic head;
        uint8_t number_of_elements;
        struct con_od_record_entry *subelements;
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};

struct con_od_list_node_array {
        struct con_od_list_node_generic head;
        uint8_t number_of_elements;
        uint8_t array_elements_attributes;
        uint8_t array_elements_data_type;
        void *data;
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};

struct con_od_list_node_var {
        struct con_od_list_node_generic head;
        uint8_t element_data_type;
        void *element_data;
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};

struct con_od_list_node_var_with_callback {
        struct con_od_list_node_generic head;
        uint8_t element_data_type;
        void *element_data;
        uint32_t (*od_callback)(void*);
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};

struct con_od_list_node_domain {
        struct con_od_list_node_generic head;
        uint32_t (*od_callback)(void*);
#ifdef USE_OD_DESCRIPTION
        struct con_od_entry_description description;
#endif
};


#define CON_OD_ELEMENT void *
#define CON_OD void *

void *CO_OD_find(void *n, uint16_t const index);

uint16_t CO_OD_getLength(void const *n, uint8_t const sub_index);

void *CO_OD_getDataPointer(void const *n, uint8_t const sub_index);

uint8_t CO_OD_getFlagsPointer(CON_OD_ELEMENT const *t);

//uint16_t CO_OD_getIndex(void *t);
uint16_t CO_OD_getAttribute(void const *n, uint8_t const sub_index);

char* CO_OD_getEntryName(void *n);
char* CO_OD_getEntrySubindexName(void *n, uint16_t sub_index);
char* CO_OD_getEntryDescription(void *n);
char* CO_OD_getEntrySubindexDescription(void *n, uint16_t sub_index);

uint8_t CO_OD_getObjectType(void *n);
uint16_t CO_OD_getDataType(void *n, uint16_t sub_index);

int32_t con_od_add_element_to_od(void *root, void * new_od_element);

uint8_t CO_OD_getMaxSubindex(const void* n);

//void *CO_OD_getCallback(void *OD, const void* n);
uint32_t (*CO_OD_getCallback(const void* n))(void*);

        
#ifdef USE_OD_DESCRIPTION
#define INIT_OD_ENTRY_VAR(_node, _od_index, _type, _attr, _data_ptr, _description_name, _description) \
        do {                                                            \
                (_node)->head.next_od_element = NULL;                   \
                (_node)->head.index = _od_index;                           \
                (_node)->head.element_id = OD_VAR;                         \
                (_node)->head.attributes = _attr;                          \
                (_node)->element_data_type = _type;                        \
                (_node)->element_data = _data_ptr;                         \
                (_node)->description.entry_name = _description_name;       \
                (_node)->description.entry_description = _description;     \
        } while(0)                                          

#define INIT_OD_ENTRY_VAR_WITH_CALLBACK(_node, _od_index, _type, _attr, _data_ptr, _cb, _description_name, _description) \
        do {                                                            \
                (_node)->head.next_od_element = NULL;                      \
                (_node)->head.index = _od_index;                           \
                (_node)->head.element_id = OD_VAR_WITH_CALLBACK;           \
                (_node)->head.attributes = _attr;                          \
                (_node)->element_data_type = _type;                        \
                (_node)->element_data = _data_ptr;                         \
                (_node)->od_callback = _cb;                         \
                (_node)->description.entry_name = _description_name;       \
                (_node)->description.entry_description = _description;     \
        } while(0)                                          



#define INIT_OD_ENTRY_VAR_STATIC(_node, _od_index, _type, _attr, _data_ptr, _description_name, _description) \
        struct con_od_list_node_var _node = {                           \
                .head = {                                               \
                        .next_od_element = NULL,                        \
                        .index = _od_index,                             \
                        .element_id = OD_VAR,                           \
                        .attributes = _attr,                            \
                },                                                      \
                .element_data_type = _type,                             \
                .element_data = _data_ptr,                              \
                .description = {                                        \
                        .entry_name = _description_name,                \
                        .entry_description = _description,              \
                }                                                       \
        }



#define INIT_OD_ENTRY_RECORD(_node, _od_index, _subelements_data_ptr, _num_of_subelem, _description_name, _description) \
        do {                                                            \
                (_node)->head.next_od_element = NULL;                      \
                (_node)->head.index = _od_index;                           \
                (_node)->head.element_id = OD_RECORD;                      \
                (_node)->head.attributes = CO_ODA_READABLE;                              \
                (_node)->number_of_elements = _num_of_subelem;             \
                (_node)->subelements = _subelements_data_ptr;              \
                (_node)->description.entry_name = _description_name;       \
                (_node)->description.entry_description = _description;     \
        } while(0)


#define INIT_OD_ENTRY_RECORD_STATIC(_node, _od_index, _subelements_data_ptr, _num_of_subelem, _description_name, _description) \
        struct con_od_list_node_record _node = {                        \
                .head = {                                               \
                        .next_od_element = NULL,                        \
                        .index = _od_index,                             \
                        .element_id = OD_RECORD,                        \
                        .attributes = 0,                                \
                },                                                      \
                .number_of_elements = _num_of_subelem,                  \
                .subelements = _subelements_data_ptr,                   \
                .description = {                                        \
                        .entry_name = _description_name,                \
                        .entry_description = _description,              \
                }                                                       \
        }


#define INIT_RECORD_SUBENTRY(_node, _type, _attr, _data_ptr, _description_name, _description) \
        do {                                                            \
                (_node)->element_data_type = _type;                        \
                (_node)->attributes = _attr;                               \
                (_node)->data = _data_ptr;                                 \
                (_node)->description.entry_name = _description_name;       \
                (_node)->description.entry_description = _description;     \
        } while(0)

#if 0
#define INIT_RECORD_SUBENTRY_STATIC(_node, _type, _attr, _data_ptr, _description_name, _description) \
        struct con_od_record_entry _node = {                            \
                .element_data_type = _type,                             \
                .attributes = _attr,                                    \
                .data = _data_ptr,                                      \
                .description = {                                        \
                        .entry_name = _description_name,                \
                        .entry_description = _description,              \
                }                                                       \
        }
#endif

#define INIT_OD_ENTRY_ARRAY(_node, _od_index, _attr_sub_element, _num_of_subelem,  _subelements_type, _attr, _array_data_ptr, _description_name, _description) \
        do {                                                            \
                (_node)->head.next_od_element = NULL;                      \
                (_node)->head.index = _od_index;                           \
                (_node)->head.element_id = OD_ARRAY;                       \
                (_node)->head.attributes = _attr;                          \
                (_node)->number_of_elements = _num_of_subelem;             \
                (_node)->array_elements_data_type = _subelements_type;     \
                (_node)->array_elements_attributes = _attr_sub_element;    \
                (_node)->data = _array_data_ptr;                           \
                (_node)->description.entry_name = _description_name;       \
                (_node)->description.entry_description = _description;     \
        } while(0)

#define INIT_OD_ENTRY_DOMAIN(_node, _od_index, _attr, _cb, _description_name, _description) \
        do {                                                            \
                (_node.head.next_od_element = NULL;                      \
                (_node.head.index = _od_index;                           \
                (_node.head.element_id = OD_DOMAIN;                      \
                (_node.head.attributes = _attr;                          \
                (_node.od_callback = _cb;                                \
                (_node.description.entry_name = _description_name;       \
                (_node.description.entry_description = _description;     \
        } while(0)                                          


#else
#define INIT_OD_ENTRY_VAR(_node, _od_index, _type, _attr, _data_ptr, _description_name, _description) \
        do {                                                            \
                (_node)->head.next_od_element = NULL;                      \
                (_node)->head.index = _od_index;                           \
                (_node)->head.element_id = OD_VAR;                         \
                (_node)->head.attributes = _attr;                          \
                (_node)->element_data_type = _type;                        \
                (_node)->element_data = _data_ptr;                         \
        } while(0)                                          


#define INIT_OD_ENTRY_VAR_STATIC(_node, _od_index, _type, _attr, _data_ptr, _description_name, _description) \
        struct con_od_list_node_var _node = {                           \
                .head = {                                               \
                        .next_od_element = NULL,                        \
                        .index = _od_index,                             \
                        .element_id = OD_VAR,                           \
                        .attributes = _attr,                            \
                },                                                      \
                .element_data_type = _type,                             \
                .element_data = _data_ptr,                              \
        }


#define INIT_OD_ENTRY_RECORD(_node, _od_index, _subelements_data_ptr, _num_of_subelem, _description_name, _description) \
        do {                                                            \
                (_node)->head.next_od_element = NULL;                      \
                (_node)->head.index = _od_index;                           \
                (_node)->head.element_id = OD_RECORD;                      \
                (_node)->head.attributes = 0;                              \
                (_node)->number_of_elements = _num_of_subelem;             \
                (_node)->subelements = _subelements_data_ptr;              \
        } while(0)


#define INIT_OD_ENTRY_RECORD_STATIC(_node, _od_index, _subelements_data_ptr, _num_of_subelem, _description_name, _description) \
        struct con_od_list_node_record _node = {                        \
                .head = {                                               \
                        .next_od_element = NULL,                        \
                        .index = _od_index,                             \
                        .element_id = OD_RECORD,                        \
                        .attributes = 0,                                \
                },                                                      \
                .number_of_elements = _num_of_subelem,                  \
                .subelements = _subelements_data_ptr,                   \
        }


#define INIT_RECORD_SUBENTRY(_node, _type, _attr, _data_ptr, _description_name, _description) \
        do {                                                            \
                _node.element_data_type = _type;                        \
                _node.attributes = _attr;                               \
                _node.data = _data_ptr;                                 \
        } while(0)


#define INIT_OD_ENTRY_ARRAY(_node, _od_index, _attr_sub_element, _num_of_subelem,  _subelements_type, _attr, _array_data_ptr, _description_name, _description) \
        do {                                                            \
                _node.head.next_od_element = NULL;                      \
                _node.head.index = _od_index;                           \
                _node.head.element_id = OD_ARRAY;                       \
                _node.head.attributes = _attr;                          \
                _node.number_of_elements = _num_of_subelem;             \
                _node.array_elements_data_type = _subelements_type;     \
                _node.array_elements_attributes = _attr_sub_element;    \
                _node.data = _array_data_ptr;                           \
        } while(0)

#endif
                  
#endif
