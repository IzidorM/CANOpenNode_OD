/*
 * CANopen Object Dictionary.
 */

#include <stdint.h>
#include "CO_OD.h"
#include "CO_SDO.h"
#include "CO_OD_interface.h"

/*******************************************************************************
   OBJECT DICTIONARY - CANopen defined
*******************************************************************************/

/*1001*/
uint8_t errorRegister = 0x0;
/*1002 */
uint32_t manufacturerStatusRegister = 0x0L;
/*1003 */
uint32_t preDefinedErrorField[8] = {0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L};
/*1010 */
uint32_t storeParameters[1] = {0x3L};
/*1011 */
uint32_t restoreDefaultParameters[1] = {0x1L};

// constants
/*1000 */
uint32_t deviceType = 0x0L;
/*1005 */
uint32_t COB_ID_SYNCMessage = 0x80L;
/*1006 */
uint32_t communicationCyclePeriod = 0x0L;
/*1007 */
uint32_t synchronousWindowLength = 0x0L;
/*1008 */
char manufacturerDeviceName[11] = {'C', 'A', 'N', 'o', 'p', 'e', 'n', 'N', 'o', 'd', 'e'};
/*1009 */
char manufacturerHardwareVersion[4] = {'3', '.', '0', '0'};
/*100A */
char manufacturerSoftwareVersion[4] = {'3', '.', '0', '0'};
/*1014 */
uint32_t COB_ID_EMCY = 0x80L;
/*1015 */
uint16_t inhibitTimeEMCY = 0x64;
/*1016 */
uint32_t consumerHeartbeatTime[4] = {0x0L, 0x0L, 0x0L, 0x0L};
/*1017 */
uint16_t producerHeartbeatTime = 500;
/*1018 */
OD_identity_t identity ={0x4, 0x0L, 0x0L, 0x0L, 0x0L};
/*1019 */
uint8_t synchronousCounterOverflowValue = 0;
/*1029 */
uint8_t errorBehavior[6] = {0x0, 0x0, 0x1, 0x0, 0x0, 0x0};
/*1200[1] */
OD_SDOServerParameter_t SDOServerParameter[1] = {{0x2, 0x600L, 0x580L}};
/*1280[1] */
OD_SDOClientParameter_t SDOClientParameter[1] = {{0x3, 0x600L, 0x580L, 0}};
/*1400[4] */
OD_RPDOCommunicationParameter_t RPDOCommunicationParameter[4] = {
        {0x2, 0x200L, 0xFF},
        {0x2, 0x300L, 0xFE},
        {0x2, 0x400L, 0xFE},
        {0x2, 0x500L, 0xFE}};

/*1600[4] */
OD_RPDOMappingParameter_t RPDOMappingParameter[4] = {
//        {
//                0x1,
//                ((0x2000) << 16) + 0x0120,
//                0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L
//        },
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
        //{0x2, 0x62000108L, 0x62000208L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L}};

/*1800[4] */
OD_TPDOCommunicationParameter_t TPDOCommunicationParameter[4] = {
        {0x6, 0x180L, 0xFF, 0x0, 0x0, 0x0, 0x0},
        {0x6, 0x280L, 0xFE, 0x0, 0x0, 0x0, 0x0},
        {0x6, 0x380L, 0xFE, 0x0, 0x0, 0x0, 0x0},
        {0x6, 0x480L, 0xFE, 0x0, 0x0, 0x0, 0x0}};

/*1A00*/
OD_TPDOMappingParameter_t TPDOMappingParameter[4] = {
//        {0x3,
//         (((UNSIGNED32)(VL53L0X_BASE_ADDR + VL53L0X_DATA_OFFSET + 0x13L)) << 16) + 0x0110,
//         (((UNSIGNED32)(VL53L0X_BASE_ADDR + VL53L0X_DATA_OFFSET + 0x13L)) << 16) + 0x0210,
//         (((UNSIGNED32)(VL53L0X_BASE_ADDR + VL53L0X_DATA_OFFSET + 0x13L)) << 16) + 0x0310,
//         0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
//        {0x0, 0x25000020L, 0x25010010L, 0x25020008L, 0x25030008L, 0x0L, 0x0L, 0x0L, 0x0L},
//        {0x2, 0x60000108L, 0x60000208L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L}, 
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L}, 
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L},
        {0x0, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L, 0x0L}};

/*1F80 */
//uint32_t NMTStartup = CO_NMT_PRE_OPERATIONAL;
uint32_t NMTStartup = 0; //CO_NMT_PRE_OPERATIONAL;


// TODO: Do we need this?
unsigned char errorStatusBits[10];

/*******************************************************************************
   STRUCTURES FOR RECORD TYPE OBJECTS
*******************************************************************************/
///*0x1018*/ static const CO_OD_entryRecord_t OD_record1018[5] = {
//           {(void*)&identity.maxSubIndex, 0x05,  1},
//           {(void*)&identity.vendorID, 0x85,  4},
//           {(void*)&identity.productCode, 0x85,  4},
//           {(void*)&identity.revisionNumber, 0x85,  4},
//           {(void*)&identity.serialNumber, 0x85,  4}};

/*0x1200*/ static const CO_OD_entryRecord_t OD_record1200[3] = {
           {(void*)&SDOServerParameter[0].maxSubIndex, 0x05,  1},
           {(void*)&SDOServerParameter[0].COB_IDClientToServer, 0x85,  4},
           {(void*)&SDOServerParameter[0].COB_IDServerToClient, 0x85,  4}};

/*0x1400*/ static const CO_OD_entryRecord_t OD_record1400[3] = {
           {(void*)&RPDOCommunicationParameter[0].maxSubIndex, 0x05,  1},
           {(void*)&RPDOCommunicationParameter[0].COB_IDUsedByRPDO, 0x8D,  4},
           {(void*)&RPDOCommunicationParameter[0].transmissionType, 0x0D,  1}};

/*0x1401*/ static const CO_OD_entryRecord_t OD_record1401[3] = {
           {(void*)&RPDOCommunicationParameter[1].maxSubIndex, 0x05,  1},
           {(void*)&RPDOCommunicationParameter[1].COB_IDUsedByRPDO, 0x8D,  4},
           {(void*)&RPDOCommunicationParameter[1].transmissionType, 0x0D,  1}};

/*0x1402*/ static const CO_OD_entryRecord_t OD_record1402[3] = {
           {(void*)&RPDOCommunicationParameter[2].maxSubIndex, 0x05,  1},
           {(void*)&RPDOCommunicationParameter[2].COB_IDUsedByRPDO, 0x8D,  4},
           {(void*)&RPDOCommunicationParameter[2].transmissionType, 0x0D,  1}};

/*0x1403*/ static const CO_OD_entryRecord_t OD_record1403[3] = {
           {(void*)&RPDOCommunicationParameter[3].maxSubIndex, 0x05,  1},
           {(void*)&RPDOCommunicationParameter[3].COB_IDUsedByRPDO, 0x8D,  4},
           {(void*)&RPDOCommunicationParameter[3].transmissionType, 0x0D,  1}};

/*0x1600*/ static const CO_OD_entryRecord_t OD_record1600[9] = {
           {(void*)&RPDOMappingParameter[0].numberOfMappedObjects, 0x0D,  1},
           {(void*)&RPDOMappingParameter[0].mappedObject1, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject2, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject3, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject4, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject5, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject6, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject7, 0x8D,  4},
           {(void*)&RPDOMappingParameter[0].mappedObject8, 0x8D,  4}};

/*0x1601*/ static const CO_OD_entryRecord_t OD_record1601[9] = {
           {(void*)&RPDOMappingParameter[1].numberOfMappedObjects, 0x0D,  1},
           {(void*)&RPDOMappingParameter[1].mappedObject1, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject2, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject3, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject4, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject5, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject6, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject7, 0x8D,  4},
           {(void*)&RPDOMappingParameter[1].mappedObject8, 0x8D,  4}};

/*0x1602*/ static const CO_OD_entryRecord_t OD_record1602[9] = {
           {(void*)&RPDOMappingParameter[2].numberOfMappedObjects, 0x0D,  1},
           {(void*)&RPDOMappingParameter[2].mappedObject1, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject2, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject3, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject4, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject5, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject6, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject7, 0x8D,  4},
           {(void*)&RPDOMappingParameter[2].mappedObject8, 0x8D,  4}};

/*0x1603*/ static const CO_OD_entryRecord_t OD_record1603[9] = {
           {(void*)&RPDOMappingParameter[3].numberOfMappedObjects, 0x0D,  1},
           {(void*)&RPDOMappingParameter[3].mappedObject1, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject2, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject3, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject4, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject5, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject6, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject7, 0x8D,  4},
           {(void*)&RPDOMappingParameter[3].mappedObject8, 0x8D,  4}};

/*0x1800*/ static const CO_OD_entryRecord_t OD_record1800[7] = {
           {(void*)&TPDOCommunicationParameter[0].maxSubIndex, 0x05,  1},
           {(void*)&TPDOCommunicationParameter[0].COB_IDUsedByTPDO, 0x8F,  4},
           {(void*)&TPDOCommunicationParameter[0].transmissionType, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[0].inhibitTime, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[0].compatibilityEntry, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[0].eventTimer, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[0].SYNCStartValue, 0x0D,  1}};

/*0x1801*/ static const CO_OD_entryRecord_t OD_record1801[7] = {
           {(void*)&TPDOCommunicationParameter[1].maxSubIndex, 0x05,  1},
           {(void*)&TPDOCommunicationParameter[1].COB_IDUsedByTPDO, 0x8F,  4},
           {(void*)&TPDOCommunicationParameter[1].transmissionType, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[1].inhibitTime, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[1].compatibilityEntry, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[1].eventTimer, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[1].SYNCStartValue, 0x0D,  1}};
/*0x1802*/ static const CO_OD_entryRecord_t OD_record1802[7] = {
           {(void*)&TPDOCommunicationParameter[2].maxSubIndex, 0x05,  1},
           {(void*)&TPDOCommunicationParameter[2].COB_IDUsedByTPDO, 0x8D,  4},
           {(void*)&TPDOCommunicationParameter[2].transmissionType, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[2].inhibitTime, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[2].compatibilityEntry, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[2].eventTimer, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[2].SYNCStartValue, 0x0D,  1}};

/*0x1803*/ static const CO_OD_entryRecord_t OD_record1803[7] = {
           {(void*)&TPDOCommunicationParameter[3].maxSubIndex, 0x05,  1},
           {(void*)&TPDOCommunicationParameter[3].COB_IDUsedByTPDO, 0x8D,  4},
           {(void*)&TPDOCommunicationParameter[3].transmissionType, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[3].inhibitTime, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[3].compatibilityEntry, 0x0D,  1},
           {(void*)&TPDOCommunicationParameter[3].eventTimer, 0x8D,  2},
           {(void*)&TPDOCommunicationParameter[3].SYNCStartValue, 0x0D,  1}};

/*0x1A00*/ static const CO_OD_entryRecord_t OD_record1A00[9] = {
           {(void*)&TPDOMappingParameter[0].numberOfMappedObjects, 0x0D,  1},
           {(void*)&TPDOMappingParameter[0].mappedObject1, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject2, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject3, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject4, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject5, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject6, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject7, 0x8D,  4},
           {(void*)&TPDOMappingParameter[0].mappedObject8, 0x8D,  4}};

/*0x1A01*/ static const CO_OD_entryRecord_t OD_record1A01[9] = {
           {(void*)&TPDOMappingParameter[1].numberOfMappedObjects, 0xff,  1},
           {(void*)&TPDOMappingParameter[1].mappedObject1, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject2, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject3, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject4, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject5, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject6, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject7, 0x8F,  4},
           {(void*)&TPDOMappingParameter[1].mappedObject8, 0x8F,  4}};

/*0x1A02*/ static const CO_OD_entryRecord_t OD_record1A02[9] = {
           {(void*)&TPDOMappingParameter[2].numberOfMappedObjects, 0x0D,  1},
           {(void*)&TPDOMappingParameter[2].mappedObject1, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject2, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject3, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject4, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject5, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject6, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject7, 0x8D,  4},
           {(void*)&TPDOMappingParameter[2].mappedObject8, 0x8D,  4}};

/*0x1A03*/ static const CO_OD_entryRecord_t OD_record1A03[9] = {
           {(void*)&TPDOMappingParameter[3].numberOfMappedObjects, 0x0D,  1},
           {(void*)&TPDOMappingParameter[3].mappedObject1, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject2, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject3, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject4, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject5, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject6, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject7, 0x8D,  4},
           {(void*)&TPDOMappingParameter[3].mappedObject8, 0x8D,  4}};



uint32_t test = 0; //0x2000

//{addr,subaddr,mask,size,ptr_to_data}
static CO_OD_entry_t CO_OD[] = {
        {
                0x1000,
                0x00,
                0x85,
                sizeof(deviceType),
                (void*)&deviceType
        },
        {
                0x1001,
                0x00,
                CO_ODA_MEM_RAM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_RPDO_MAPABLE |
                CO_ODA_TPDO_MAPABLE,
                sizeof(errorRegister),
                (void*)&errorRegister
        },
        {
                0x1002,
                0x00,
                CO_ODA_MEM_RAM |
                CO_ODA_READABLE |
                CO_ODA_RPDO_MAPABLE |
                CO_ODA_TPDO_MAPABLE |
                CO_ODA_MB_VALUE,
                sizeof(manufacturerStatusRegister),
                (void*)&manufacturerStatusRegister
        },
        {
                0x1003,
                0x08,
                CO_ODA_MEM_RAM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(preDefinedErrorField[0]),
                (void*)&preDefinedErrorField[0]
        },
        {
                0x1005,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(COB_ID_SYNCMessage),
                (void*)&COB_ID_SYNCMessage
        },
        {
                0x1006,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(communicationCyclePeriod),
                (void*)&communicationCyclePeriod
        },
        {
                0x1007,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(synchronousWindowLength),
                (void*)&synchronousWindowLength
        },
        {
                0x1008,
                0x00,
                0x05,
                11,
                (void*)&manufacturerDeviceName[0]
        },
        {
                0x1009,
                0x00,
                0x05,
                sizeof(manufacturerHardwareVersion[0]),
                (void*)&manufacturerHardwareVersion[0]
        },
        {
                0x100A,
                0x00,
                0x05,
                sizeof(manufacturerSoftwareVersion[0]),
                (void*)&manufacturerSoftwareVersion[0]
        },
        {
                0x1010,
                0x01,
                0x8E,
                sizeof(storeParameters[0]),
                (void*)&storeParameters[0]
        },
        {
                0x1011,
                0x01,
                0x8E,
                sizeof(restoreDefaultParameters[0]),
                (void*)&restoreDefaultParameters[0]
        },
        {
                0x1014,
                0x00,
                0x85,
                sizeof(COB_ID_EMCY),
                (void*)&COB_ID_EMCY
        },
        {
                0x1015,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(inhibitTimeEMCY),
                (void*)&inhibitTimeEMCY
        },
        {
                0x1016,
                0x04,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(consumerHeartbeatTime[0]),
                (void*)&consumerHeartbeatTime[0]
        },
        {
                0x1017,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                sizeof(producerHeartbeatTime),
                (void*)&producerHeartbeatTime
        },
        {
                0x1018,
                0x04,
                0x00,
                0,
                (void*)&identity
        },
        {
                0x1019,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE,
                sizeof(synchronousCounterOverflowValue),
                (void*)&synchronousCounterOverflowValue
        },
        {
                0x1029,
                0x06,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE,
                sizeof(errorBehavior[0]),
                (void*)&errorBehavior[0]
        },
        {
                0x1200,
                0x02,
                0x00,
                0,
                (void*)&OD_record1200
        },
        {
                0x1400,
                0x02,
                0x00,
                0,
                (void*)&OD_record1400
        },
        {
                0x1401,
                0x02,
                0x00,
                0,
                (void*)&OD_record1401
        },
        {
                0x1402,
                0x02,
                0x00,
                0,
                (void*)&OD_record1402
        },
        {
                0x1403,
                0x02,
                0x00,
                0,
                (void*)&OD_record1403
        },
        {
                0x1600,
                0x08,
                0x00,
                0,
                (void*)&OD_record1600
        },
        {
                0x1601,
                0x08,
                0x00,
                0,
                (void*)&OD_record1601
        },
        {
                0x1602,
                0x08,
                0x00,
                0,
                (void*)&OD_record1602
        },
        {
                0x1603,
                0x08,
                0x00,
                0,
                (void*)&OD_record1603
        },
        {
                0x1800,
                0x06,
                0x00,
                0,
                (void*)&OD_record1800
        },
        {
                0x1801,
                0x06,
                0x00,
                0,
                (void*)&OD_record1801
        },
        {
                0x1802,
                0x06,
                0x00,
                0,
                (void*)&OD_record1802
        },
        {
                0x1803,
                0x06,
                0x00,
                0,
                (void*)&OD_record1803
        },
        {
                0x1A00,
                0x08,
                0x00,
                0,
                (void*)&OD_record1A00
        },
        {
                0x1A01,
                0x08,
                0x00,
                0,
                (void*)&OD_record1A01
        },
        {
                0x1A02,
                0x08,
                0x00,
                0,
                (void*)&OD_record1A02
        },
        {
                0x1A03,
                0x08,
                0x00,
                0,
                (void*)&OD_record1A03
        },
        {
                0x1F80,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                4,
                (void*)&NMTStartup
        },
        {
                0x2000,
                0x00,
                CO_ODA_MEM_ROM |
                CO_ODA_READABLE |
                CO_ODA_WRITEABLE |
                CO_ODA_MB_VALUE,
                4,
                (void*)&test,
        },
        
};

static CO_OD_extension_t COO_SDO_ODExtensions[sizeof(CO_OD) / sizeof(CO_OD_entry_t)];

uint32_t od_get_od_size(void)
{
        return sizeof(CO_OD) / sizeof(CO_OD_entry_t);
}

//uint32_t od_get_od_ext_size(void)
//{
//        return od_get_od_size();
//}

const CO_OD_entry_t *od_get_od_ptr(void)
{
        return &CO_OD[0];
}

CO_OD_extension_t *od_get_od_ext_ptr(void)
{
        return &COO_SDO_ODExtensions[0];
}
