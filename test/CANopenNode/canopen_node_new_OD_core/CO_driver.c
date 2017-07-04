/*
 * CAN module object for generic microcontroller.
 *
 * This file is a template for other microcontrollers.
 *
 * @file        CO_driver.c
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

#include <string.h>
#include "CO_driver.h"

#define MAX_CAN_CALLBACKS 16

struct co_driver_msg_callback {
        bool enabled;
        uint32_t cob_id;
        int32_t (*callback)(void *, void *);
        void *callback_param;
};

struct co_driver_msg_callback msg_callbacks[MAX_CAN_CALLBACKS];

CO_CANtx_t last_sent_buffer;

int32_t co_driver_open(void *CAN)
{
        uint32_t i;
        for (i = 0; MAX_CAN_CALLBACKS > i; i++)
        {
                msg_callbacks[i].enabled = false;
        }
        return 0;
}

int32_t co_driver_close(void *CAN)
{
        return 0;
}

int32_t co_driver_send(void *CAN, CO_CANtx_t *msg)
{
        memcpy(&last_sent_buffer, msg, sizeof(CO_CANtx_t));
        return 0; //add error handling
}

int32_t co_driver_receive(void *CAN, CO_CANrx_t *msg)
{
        uint32_t i;
        for (i = 0; MAX_CAN_CALLBACKS > i; i++)
        {
                struct co_driver_msg_callback *tmp = &msg_callbacks[i];
                if (tmp->enabled)
                {
                        if ((msg->ident & 0x7ff) == tmp->cob_id)
                        {
                                if (tmp->callback)
                                {
                                        tmp->callback(tmp->callback_param, msg);
                                }
                        }
                }
        }
        return 0; //add error handling
}

void *co_driver_register_callback(uint16_t cob_id,
                                  int32_t (*callback)(void *, void *),
                                  void *callback_param)
{
        uint32_t i;
        for (i = 0; MAX_CAN_CALLBACKS > i; i++)
        {
                struct co_driver_msg_callback *tmp = &msg_callbacks[i];
                if (!tmp->enabled)
                {
                        tmp->cob_id = cob_id;
                        tmp->callback = callback;
                        tmp->callback_param = callback_param;

                        tmp->enabled = true;
                        break;
                }
        }

        return 0; //add error handling
}

void co_driver_modify_callback(void *cb_obj, uint16_t cob_id, void (*callback)(void *))
{
        
}


