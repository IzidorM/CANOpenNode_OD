#ifndef CANOPEN_OD_INTERFACE_H
#define CANOPEN_OD_INTERFACE_H

#include <inttypes.h>

typedef struct{
        uint8_t      maxSubIndex;
        uint32_t     COB_IDUsedByTPDO;
        uint8_t       transmissionType;
        uint16_t     inhibitTime;
        uint8_t      compatibilityEntry;
        uint16_t     eventTimer;
        uint8_t      SYNCStartValue;
}              OD_TPDOCommunicationParameter_t;


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
 * Common DS301 object dictionary entries.
 */
typedef enum{
    OD_H1000_DEV_TYPE             = 0x1000U,/**< Device type */
    OD_H1001_ERR_REG              = 0x1001U,/**< Error register */
    OD_H1002_MANUF_STATUS_REG     = 0x1002U,/**< Manufacturer status register */
    OD_H1003_PREDEF_ERR_FIELD     = 0x1003U,/**< Predefined error field */
    OD_H1004_RSV                  = 0x1004U,/**< Reserved */
    OD_H1005_COBID_SYNC           = 0x1005U,/**< Sync message cob-id */
    OD_H1006_COMM_CYCL_PERIOD     = 0x1006U,/**< Communication cycle period */
    OD_H1007_SYNC_WINDOW_LEN      = 0x1007U,/**< Sync windows length */
    OD_H1008_MANUF_DEV_NAME       = 0x1008U,/**< Manufacturer device name */
    OD_H1009_MANUF_HW_VERSION     = 0x1009U,/**< Manufacturer hardware version */
    OD_H100A_MANUF_SW_VERSION     = 0x100AU,/**< Manufacturer software version */
    OD_H100B_RSV                  = 0x100BU,/**< Reserved */
    OD_H100C_GUARD_TIME           = 0x100CU,/**< Guard time */
    OD_H100D_LIFETIME_FACTOR      = 0x100DU,/**< Life time factor */
    OD_H100E_RSV                  = 0x100EU,/**< Reserved */
    OD_H100F_RSV                  = 0x100FU,/**< Reserved */
    OD_H1010_STORE_PARAM_FUNC     = 0x1010U,/**< Store parameter in persistent memory function */
    OD_H1011_REST_PARAM_FUNC      = 0x1011U,/**< Restore default parameter function */
    OD_H1012_COBID_TIME           = 0x1012U,/**< Timestamp message cob-id */
    OD_H1013_HIGH_RES_TIMESTAMP   = 0x1013U,/**< High resolution timestamp */
    OD_H1014_COBID_EMERGENCY      = 0x1014U,/**< Emergency message cob-id */
    OD_H1015_INHIBIT_TIME_MSG     = 0x1015U,/**< Inhibit time message */
    OD_H1016_CONSUMER_HB_TIME     = 0x1016U,/**< Consumer heartbeat time */
    OD_H1017_PRODUCER_HB_TIME     = 0x1017U,/**< Producer heartbeat time */
    OD_H1018_IDENTITY_OBJECT      = 0x1018U,/**< Identity object */
    OD_H1019_SYNC_CNT_OVERFLOW    = 0x1019U,/**< Sync counter overflow value */
    OD_H1020_VERIFY_CONFIG        = 0x1020U,/**< Verify configuration */
    OD_H1021_STORE_EDS            = 0x1021U,/**< Store EDS */
    OD_H1022_STORE_FORMAT         = 0x1022U,/**< Store format */
    OD_H1023_OS_CMD               = 0x1023U,/**< OS command */
    OD_H1024_OS_CMD_MODE          = 0x1024U,/**< OS command mode */
    OD_H1025_OS_DBG_INTERFACE     = 0x1025U,/**< OS debug interface */
    OD_H1026_OS_PROMPT            = 0x1026U,/**< OS prompt */
    OD_H1027_MODULE_LIST          = 0x1027U,/**< Module list */
    OD_H1028_EMCY_CONSUMER        = 0x1028U,/**< Emergency consumer object */
    OD_H1029_ERR_BEHAVIOR         = 0x1029U,/**< Error behaviour */
    OD_H1200_SDO_SERVER_PARAM     = 0x1200U,/**< SDO server parameters */
    OD_H1280_SDO_CLIENT_PARAM     = 0x1280U,/**< SDO client parameters */
    OD_H1400_RXPDO_1_PARAM        = 0x1400U,/**< RXPDO communication parameter */
    OD_H1401_RXPDO_2_PARAM        = 0x1401U,/**< RXPDO communication parameter */
    OD_H1402_RXPDO_3_PARAM        = 0x1402U,/**< RXPDO communication parameter */
    OD_H1403_RXPDO_4_PARAM        = 0x1403U,/**< RXPDO communication parameter */
    OD_H1600_RXPDO_1_MAPPING      = 0x1600U,/**< RXPDO mapping parameters */
    OD_H1601_RXPDO_2_MAPPING      = 0x1601U,/**< RXPDO mapping parameters */
    OD_H1602_RXPDO_3_MAPPING      = 0x1602U,/**< RXPDO mapping parameters */
    OD_H1603_RXPDO_4_MAPPING      = 0x1603U,/**< RXPDO mapping parameters */
    OD_H1800_TXPDO_1_PARAM        = 0x1800U,/**< TXPDO communication parameter */
    OD_H1801_TXPDO_2_PARAM        = 0x1801U,/**< TXPDO communication parameter */
    OD_H1802_TXPDO_3_PARAM        = 0x1802U,/**< TXPDO communication parameter */
    OD_H1803_TXPDO_4_PARAM        = 0x1803U,/**< TXPDO communication parameter */
    OD_H1A00_TXPDO_1_MAPPING      = 0x1A00U,/**< TXPDO mapping parameters */
    OD_H1A01_TXPDO_2_MAPPING      = 0x1A01U,/**< TXPDO mapping parameters */
    OD_H1A02_TXPDO_3_MAPPING      = 0x1A02U,/**< TXPDO mapping parameters */
    OD_H1A03_TXPDO_4_MAPPING      = 0x1A03U /**< TXPDO mapping parameters */
}CO_ObjDicId_t;

/**
 * Bit masks for flags associated with variable from @ref CO_SDO_objectDictionary.
 *
 * This additional functionality of any variable in @ref CO_SDO_objectDictionary can be
 * enabled by function CO_OD_configure(). Location of the flag byte can be
 * get from function CO_OD_getFlagsPointer().
 */
typedef enum{
    /** Variable was written by RPDO. Flag can be cleared by application */
    CO_ODFL_RPDO_WRITTEN        = 0x01U,
    /** Variable is mapped to TPDO */
    CO_ODFL_TPDO_MAPPED         = 0x02U,
    /** Change of state bit, initially copy of attribute from CO_OD_entry_t.
    If set and variable is mapped to TPDO, TPDO will be automatically send,
    if variable changed */
    CO_ODFL_TPDO_COS_ENABLE     = 0x04U,
    /** PDO send bit, can be set by application. If variable is mapped into
    TPDO, TPDO will be send and bit will be cleared. */
    CO_ODFL_TPDO_SEND           = 0x08U,
    /** Variable was accessed by SDO download */
    CO_ODFL_SDO_DOWNLOADED      = 0x10U,
    /** Variable was accessed by SDO upload */
    CO_ODFL_SDO_UPLOADED        = 0x20U,
    /** Reserved */
    CO_ODFL_BIT_6               = 0x40U,
    /** Reserved */
    CO_ODFL_BIT_7               = 0x80U
}CO_SDO_OD_flags_t;


/**
 * Object for one entry with specific index in @ref CO_SDO_objectDictionary.
 */
typedef struct {
    /** The index of Object from 0x1000 to 0xFFFF */
    uint16_t            index;
    /** Number of (sub-objects - 1). If Object Type is variable, then
    maxSubIndex is 0, otherwise maxSubIndex is equal or greater than 1. */
    uint8_t             maxSubIndex;
    /** If Object Type is record, attribute is set to zero. Attribute for
    each member is then set in special array with members of type
    CO_OD_entryRecord_t. If Object Type is Array, attribute is common for
    all array members. See #CO_SDO_OD_attributes_t. */
    uint16_t            attribute;
    /** If Object Type is Variable, length is the length of variable in bytes.
    If Object Type is Array, length is the length of one array member.
    If Object Type is Record, length is zero. Length for each member is
    set in special array with members of type CO_OD_entryRecord_t.
    If Object Type is Domain, length is zero. Length is specified
    by application in @ref CO_SDO_OD_function. */
    uint16_t            length;
    /** If Object Type is Variable, pData is pointer to data.
    If Object Type is Array, pData is pointer to data. Data doesn't
    include Sub-Object 0.
    If object type is Record, pData is pointer to special array
    with members of type CO_OD_entryRecord_t.
    If object type is Domain, pData is null. */
    void               *pData;
} CO_OD_entry_t;

struct CO_OD {
        uint32_t od_size;
        CO_OD_entry_t *od;
};

/**
 * Object for record type entry in @ref CO_SDO_objectDictionary.
 *
 * See CO_OD_entry_t.
 */
typedef struct{
    /** See #CO_SDO_OD_attributes_t */
    void               *pData;
    /** Length of variable in bytes. If object type is Domain, length is zero */
    uint16_t            attribute;
    /** Pointer to data. If object type is Domain, pData is null */
    uint16_t            length;
} CO_OD_entryRecord_t;

/**
 * Object is used as array inside CO_SDO_t, parallel to @ref CO_SDO_objectDictionary.
 *
 * Object is generated by function CO_OD_configure(). It is then used as
 * extension to Object dictionary entry at specific index.
 */
typedef struct{
    /** Pointer to @ref CO_SDO_OD_function */
//    CO_SDO_abortCode_t (*pODFunc)(CO_ODF_arg_t *ODF_arg);
    /** Pointer to object, which will be passed to @ref CO_SDO_OD_function */
    void               *object;
    /** Pointer to #CO_SDO_OD_flags_t. If object type is array or record, this
    variable points to array with length equal to number of subindexes. */
    uint8_t            *flags;
} CO_OD_extension_t;

#define CON_OD_ELEMENT CO_OD_entry_t
#define CON_OD struct CO_OD 

void *CO_OD_find(void *n, uint16_t index);
uint16_t CO_OD_getLength(void *n, uint8_t subIndex);

#endif
