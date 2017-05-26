#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_OD.h"
#include "CO_SDO.h"
#include "CO_Config.h"


extern CO_CANtx_t last_sent_buffer;

void setUp(void)
{
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
//        test_plogger_reset();
}

void tearDown(void)
{
        
}

void make_msg_for_sdo_expedited_transfere(CO_SDO_t *SDO,
                                          uint16_t index,
                                          uint8_t subindex,
                                          uint32_t data,
                                          uint32_t data_size)
{
        CO_CANrxMsg_t msg;
        msg.ident = SDO->nodeId;
        msg.DLC = 8; // SDO msgs are always 8 bytes ???
        msg.data[0] = 1 << 5 | (4 - data_size) << 2 | 0x3;
        msg.data[1] = index & 0xff;
        msg.data[2] = index >> 8;
        msg.data[3] = subindex;

        uint32_t i;
        uint8_t *pd = (uint8_t *) &data;
        for (i = 4; 8 > i; i++)
        {
                msg.data[i] = pd[i - 4];
        }
        CO_SDO_receive(SDO, &msg);
}

void test_SDO_expedited_transfer(void)
{
        uint32_t node_id = 0x20;
        CO_ReturnError_t err;
        CO_SDO_t SDO;
        CO_CANmodule_t NOT_USED;

        struct CO_OD OD = {
                od_get_od_size(),
                od_get_od_ptr(),
                od_get_od_ext_ptr(),
        };

        err = CO_SDO_init(
                &SDO,
                0x600 + node_id,
                0x580 + node_id,
                OD_H1200_SDO_SERVER_PARAM,
                NULL,
                &OD,
                node_id,
                &NOT_USED, // in testing sdo Rx function will be called directly
                0,
                &NOT_USED,
                1);

        TEST_ASSERT_EQUAL_UINT32(0, err);

        make_msg_for_sdo_expedited_transfere(&SDO,
                                             0x2000,
                                             0,
                                             0xf0,
                                             4);

        err = CO_SDO_process(
                &SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(0, err);

        make_msg_for_sdo_expedited_transfere(&SDO,
                                             0x2000,
                                             0,
                                             0xff,
                                             4);

        err = CO_SDO_process(
                &SDO,
                true,
                100,
                1000,
                NULL);

        TEST_ASSERT_EQUAL_UINT8(0, err);
        TEST_ASSERT_EQUAL_UINT8(3, last_sent_buffer.data[0] >> 5); // scs=3
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0] & 0xf); // not used, always 0
        // multiplexor
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[1] ); // not used, always 0
        TEST_ASSERT_EQUAL_UINT8(0x20, last_sent_buffer.data[2] ); // not used, always 0
        TEST_ASSERT_EQUAL_UINT8(0x0, last_sent_buffer.data[3] ); // not used, always 0
}
