#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

//#include "utils.h"

#include "unity.h"

#include "CO_driver.h"
#include "CO_NMT.h"
#include "CO_Config.h"
#include "CO_OD_interface.h"

extern CO_CANtx_t last_sent_buffer;
CO_NMT_internalState_t store_previous_state;
CO_NMT_internalState_t store_requested_state;
uint32_t nmt_state_changed_callback_cnt = 0;

void setUp(void)
{
        memset(&last_sent_buffer, 0, sizeof(CO_CANtx_t));
        nmt_state_changed_callback_cnt = 0;
}

void tearDown(void)
{
        
}

// This is a function simulating the master node starting a sdo expedited transfer
// It will insert the sdo message to the "driver rx buffer"
static void make_msg_for_nmt_transfere(CO_NMT_t *NMT, CO_NMT_command_t nmt_command)
{
        CO_CANrxMsg_t msg;

        msg.ident = NMT->nodeId;
        msg.DLC = 2; // SDO msgs are always 8 bytes ???
        msg.data[0] = nmt_command;
        msg.data[1] = NMT->nodeId;

        CO_NMT_receive(NMT, &msg);
}

void nmt_state_changed_callback(CO_NMT_internalState_t previous_state, CO_NMT_internalState_t requested_state)
{
        store_previous_state = previous_state;
        store_requested_state = requested_state;
        nmt_state_changed_callback_cnt += 1;
        return;
}

static CO_NMT_t *init_nmt(uint32_t node_id, void *OD)
{
        CO_ReturnError_t err;
        // malloc memory for SDO.
        // Dont bother freeing it, we have enough ram on a PC :)
        CO_NMT_t *NMT = malloc(sizeof(CO_NMT_t));
        CO_CANrx_t *rxArray = malloc(sizeof(CO_CANrx_t));
        CO_CANtx_t *txArray = malloc(sizeof(CO_CANtx_t));;
        CO_CANmodule_t *NOT_USED = malloc(sizeof(CO_CANmodule_t));

        NOT_USED->rxArray = rxArray;
        NOT_USED->txArray = txArray;
        
        err = CO_NMT_init(
                NMT,
                node_id,
                OD,
                nmt_state_changed_callback,
                NOT_USED, // in testing sdo Rx function will be called directly
                0,
                CO_CAN_ID_NMT_SERVICE);
        if (err)
        {
                return NULL;
        }
        else
        {
                return NMT;
        }
}

void test_NMT_init(void)
{
        uint8_t node_id = 0x20;
        struct con_od_list_node_var OD;
        //just init OD entry so next ptr is set to null. 
        INIT_OD_ENTRY_VAR(OD, 0x2000, OD_TYPE_UINT32, CO_ODA_WRITEABLE, &node_id, NULL, NULL);

        CO_NMT_t *NMT = init_nmt(node_id, &OD);
        TEST_ASSERT_NOT_NULL(NMT);
        TEST_ASSERT_EQUAL_PTR(&OD, NMT->OD);
        TEST_ASSERT_EQUAL_UINT8(node_id, NMT->nodeId);
        TEST_ASSERT_EQUAL_PTR(nmt_state_changed_callback, NMT->state_changed_callback);

        // check boot msg
        TEST_ASSERT_EQUAL_UINT32(node_id, last_sent_buffer.ident);
        TEST_ASSERT_EQUAL_UINT8(0, last_sent_buffer.data[0]);

        // check callbacks
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_INITIALIZING, store_previous_state);
//        TEST_ASSERT_EQUAL_UINT32(CO_NMT_OPERATIONAL, store_requested_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_PRE_OPERATIONAL, store_requested_state);
        TEST_ASSERT_EQUAL_UINT32(1, nmt_state_changed_callback_cnt);

        //TODO: Test automatic startup
        uint32_t nmt_startup = 0; // no automatic startup capabilities
        INIT_OD_ENTRY_VAR(OD, 0x1f80, OD_TYPE_UINT32, CO_ODA_WRITEABLE,
                          &nmt_startup, NULL, NULL);

        NMT = init_nmt(node_id, &OD);
        TEST_ASSERT_NOT_NULL(NMT);

        // check callbacks
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_INITIALIZING, store_previous_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_PRE_OPERATIONAL, store_requested_state);
        TEST_ASSERT_EQUAL_UINT32(2, nmt_state_changed_callback_cnt);

        nmt_startup = 0x8; // automatic startup
        NMT = init_nmt(node_id, &OD);
        TEST_ASSERT_NOT_NULL(NMT);

        // check callbacks
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_INITIALIZING, store_previous_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_OPERATIONAL, store_requested_state);
        TEST_ASSERT_EQUAL_UINT32(3, nmt_state_changed_callback_cnt);
}

void test_NMT_state_changes(void)
{
        CO_NMT_reset_cmd_t r;
        uint8_t node_id = 0x20;
        struct con_od_list_node_var OD;
        //just init OD entry so next ptr is set to null. 
        INIT_OD_ENTRY_VAR(OD, 0x2000, OD_TYPE_UINT32, CO_ODA_WRITEABLE, &node_id, NULL, NULL);

        CO_NMT_t *NMT = init_nmt(node_id, &OD);
        TEST_ASSERT_NOT_NULL(NMT);
        TEST_ASSERT_EQUAL_UINT32(1, nmt_state_changed_callback_cnt);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_INITIALIZING, store_previous_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_PRE_OPERATIONAL, store_requested_state);
        
        make_msg_for_nmt_transfere(NMT, CO_NMT_ENTER_OPERATIONAL);
        r = CO_NMT_process(NMT);
        TEST_ASSERT_EQUAL_UINT32( CO_RESET_NOT, r);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_PRE_OPERATIONAL, store_previous_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_OPERATIONAL, store_requested_state);

        make_msg_for_nmt_transfere(NMT, CO_NMT_ENTER_PRE_OPERATIONAL);
        r = CO_NMT_process(NMT);
        TEST_ASSERT_EQUAL_UINT32( CO_RESET_NOT, r);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_OPERATIONAL, store_previous_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_PRE_OPERATIONAL, store_requested_state);
                                   
        make_msg_for_nmt_transfere(NMT, CO_NMT_ENTER_STOPPED);
        r = CO_NMT_process(NMT);
        TEST_ASSERT_EQUAL_UINT32( CO_RESET_NOT, r);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_PRE_OPERATIONAL, store_previous_state);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_STOPPED, store_requested_state);

        make_msg_for_nmt_transfere(NMT, CO_NMT_RESET_COMMUNICATION);
        r = CO_NMT_process(NMT);
        TEST_ASSERT_EQUAL_UINT32(CO_RESET_COMM, r);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_INITIALIZING, CO_NMT_getInternalState(NMT));

        make_msg_for_nmt_transfere(NMT, CO_NMT_RESET_NODE);
        r = CO_NMT_process(NMT);
        TEST_ASSERT_EQUAL_UINT32(CO_RESET_APP, r);
        TEST_ASSERT_EQUAL_UINT32(CO_NMT_INITIALIZING, CO_NMT_getInternalState(NMT));
}
