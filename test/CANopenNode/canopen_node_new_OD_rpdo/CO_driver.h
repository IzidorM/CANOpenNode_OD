/**
 * CAN module object for generic microcontroller.
 *
 * This file is a template for other microcontrollers.
 *
 * @file        CO_driver.h
 * @ingroup     CO_driver
 * @author      Janez Paternoster
 * @copyright   2004 - 2015 Janez Paternoster
 *
 * This file is part of CANopenNode, an opensource CANopen Stack.
 * Project home page is <https://github.com/CANopenNode/CANopenNode>.
 * For more information on CANopen see <http://www.can-cia.org/>.
 *
 * CANopenNode is free and open source software: you can redistribute
 * it and/or modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * Following clarification and special exception to the GNU General Public
 * License is included to the distribution terms of CANopenNode:
 *
 * Linking this library statically or dynamically with other modules is
 * making a combined work based on this library. Thus, the terms and
 * conditions of the GNU General Public License cover the whole combination.
 *
 * As a special exception, the copyright holders of this library give
 * you permission to link this library with independent modules to
 * produce an executable, regardless of the license terms of these
 * independent modules, and to copy and distribute the resulting
 * executable under terms of your choice, provided that you also meet,
 * for each linked independent module, the terms and conditions of the
 * license of that module. An independent module is a module which is
 * not derived from or based on this library. If you modify this
 * library, you may extend this exception to your version of the
 * library, but you are not obliged to do so. If you do not wish
 * to do so, delete this exception statement from your version.
 */


#ifndef CO_DRIVER_H
#define CO_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include processor header file */
#include <stddef.h>         /* for 'NULL' */
#include <stdint.h>         /* for 'int8_t' to 'uint64_t' */
#include <stdbool.h>        /* for 'true', 'false' */

/**
 * Return values of some CANopen functions. If function was executed
 * successfully it returns 0 otherwise it returns <0.
 */
typedef enum{
    CO_ERROR_NO                 = 0,    /**< Operation completed successfully */
    CO_ERROR_ILLEGAL_ARGUMENT   = -1,   /**< Error in function arguments */
    CO_ERROR_OUT_OF_MEMORY      = -2,   /**< Memory allocation failed */
    CO_ERROR_TIMEOUT            = -3,   /**< Function timeout */
    CO_ERROR_ILLEGAL_BAUDRATE   = -4,   /**< Illegal baudrate passed to function CO_CANmodule_init() */
    CO_ERROR_RX_OVERFLOW        = -5,   /**< Previous message was not processed yet */
    CO_ERROR_RX_PDO_OVERFLOW    = -6,   /**< previous PDO was not processed yet */
    CO_ERROR_RX_MSG_LENGTH      = -7,   /**< Wrong receive message length */
    CO_ERROR_RX_PDO_LENGTH      = -8,   /**< Wrong receive PDO length */
    CO_ERROR_TX_OVERFLOW        = -9,   /**< Previous message is still waiting, buffer full */
    CO_ERROR_TX_PDO_WINDOW      = -10,  /**< Synchronous TPDO is outside window */
    CO_ERROR_TX_UNCONFIGURED    = -11,  /**< Transmit buffer was not confugured properly */
    CO_ERROR_PARAMETERS         = -12,  /**< Error in function function parameters */
    CO_ERROR_DATA_CORRUPT       = -13,  /**< Stored data are corrupt */
    CO_ERROR_CRC                = -14   /**< CRC does not match */
}CO_ReturnError_t;

/**
 * CAN receive message structure as aligned in CAN module. It is different in
 * different microcontrollers. It usually contains other variables.
 */
typedef struct{
    /** CAN identifier. It must be read through CO_CANrxMsg_readIdent() function. */
    uint32_t            ident;
    uint8_t             DLC ;           /**< Length of CAN message */
    uint8_t             data[8];        /**< 8 data bytes */
}CO_CANrxMsg_t;


/**
 * Received message object
 */
typedef struct{
    uint16_t            ident;          /**< Standard CAN Identifier (bits 0..10) + RTR (bit 11) */
    uint16_t            mask;           /**< Standard Identifier mask with same alignment as ident */
    void               *object;         /**< From CO_CANrxBufferInit() */
    void              (*pFunct)(void *object, const CO_CANrxMsg_t *message);  /**< From CO_CANrxBufferInit() */
}CO_CANrx_t;


/**
 * Transmit message object.
 */
typedef struct{
    uint32_t            ident;          /**< CAN identifier as aligned in CAN module */
    uint8_t             DLC ;           /**< Length of CAN message. (DLC may also be part of ident) */
    uint8_t             data[8];        /**< 8 data bytes */
    volatile bool     bufferFull;     /**< True if previous message is still in buffer */
    /** Synchronous PDO messages has this flag set. It prevents them to be sent outside the synchronous window */
    volatile bool     syncFlag;
}CO_CANtx_t;

/**
 * Endianes.
 *
 * Depending on processor or compiler architecture, one of the two macros must
 * be defined: CO_LITTLE_ENDIAN or CO_BIG_ENDIAN. CANopen itself is little endian.
 */
#define CO_LITTLE_ENDIAN


/**
 * Send CAN message.
 *
 * @param CANmodule Ptr to can driver structure.
 * @param buffer Pointer to transmit buffer, returned by CO_CANtxBufferInit().
 * Data bytes must be written in buffer before function call.
 *
 * @return #CO_ReturnError_t: CO_ERROR_NO, CO_ERROR_TX_OVERFLOW or
 * CO_ERROR_TX_PDO_WINDOW (Synchronous TPDO is outside window).
 */

int32_t co_driver_open(void *CAN);
int32_t co_driver_send(void *CAN, CO_CANtx_t *buffer);

void *co_driver_register_callback(uint16_t cob_id,
                                  void (*callback)(void *),
                                  void *callback_param);

        
#endif
