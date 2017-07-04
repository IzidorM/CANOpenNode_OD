/**
 * Default CANopen identifiers.
 *
 * Default CANopen identifiers for CANopen communication objects. Same as
 * 11-bit addresses of CAN messages. These are default identifiers and
 * can be changed in CANopen. Especially PDO identifiers are confgured
 * in PDO linking phase of the CANopen network configuration.
 */
typedef enum{
     CO_CAN_ID_NMT_SERVICE       = 0x000,   /**< 0x000, Network management */
     CO_CAN_ID_SYNC              = 0x080,   /**< 0x080, Synchronous message */
     CO_CAN_ID_EMERGENCY         = 0x080,   /**< 0x080, Emergency messages (+nodeID) */
     CO_CAN_ID_TIME_STAMP        = 0x100,   /**< 0x100, Time stamp message */
     CO_CAN_ID_TPDO_1            = 0x180,   /**< 0x180, Default TPDO1 (+nodeID) */
     CO_CAN_ID_RPDO_1            = 0x200,   /**< 0x200, Default RPDO1 (+nodeID) */
     CO_CAN_ID_TPDO_2            = 0x280,   /**< 0x280, Default TPDO2 (+nodeID) */
     CO_CAN_ID_RPDO_2            = 0x300,   /**< 0x300, Default RPDO2 (+nodeID) */
     CO_CAN_ID_TPDO_3            = 0x380,   /**< 0x380, Default TPDO3 (+nodeID) */
     CO_CAN_ID_RPDO_3            = 0x400,   /**< 0x400, Default RPDO3 (+nodeID) */
     CO_CAN_ID_TPDO_4            = 0x480,   /**< 0x480, Default TPDO4 (+nodeID) */
     CO_CAN_ID_RPDO_4            = 0x500,   /**< 0x500, Default RPDO5 (+nodeID) */
     CO_CAN_ID_TSDO              = 0x580,   /**< 0x580, SDO response from server (+nodeID) */
     CO_CAN_ID_RSDO              = 0x600,   /**< 0x600, SDO request from client (+nodeID) */
     CO_CAN_ID_HEARTBEAT         = 0x700,    /**< 0x700, Heartbeat message */
     CO_CAN_ID_BOOTUP            = 0x700    /**< 0x700 bootup msg (same as hb) */
}CO_Default_CAN_ID_t;
