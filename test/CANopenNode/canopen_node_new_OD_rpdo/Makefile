include ../../makefile.rules

ifndef $(TARGET)
TARGET=canopen_node_new_OD
endif

PROJECT_BASE_DIR = .

TARGET_SRC_FILES= \
	$(SRC_DIR)/od_interface_linked_list_v2/CO_OD_interface.c \
	CO_driver.c \
	$(SRC_DIR)/CANopenNode/stack/CO_PDO.c \
	$(SRC_DIR)/CANopenNode/stack/CO_RPDO.c \
	$(SRC_DIR)/CANopenNode/stack/crc16-ccitt.c \

#$(SRC_DIR)/CANopenNode/stack/CO_OD_interface.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_Emergency.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_HBconsumer.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_NMT_Heartbeat.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_PDO.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_SDOmaster.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_SYNC.c \
#	$(SRC_DIR)/CANopenNode/stack/CO_trace.c \


#CANopen.c \


#$(SRC_DIR)/lib/utils/utils_data_dump.c

#LIB_SRC_FILES= \
#	$(SRC_DIR)/lib/opensource/printf.c


TARGET_SRC_TEST_FILES= Test_$(TARGET).c

TARGET_SRC_TEST_RUNNER_FILES= $(patsubst %.c,%_Runner.c, $(TARGET_SRC_TEST_FILES))

TARGET_SRC_ALL=$(TARGET_SRC_FILES) \
	$(LIB_SRC_FILES) \
	$(TARGET_SRC_TEST_FILES) \
	$(TARGET_SRC_TEST_RUNNER_FILES) \
	$(UNITY_SRC_FILES)

INC_DIRS= \
	$(SRC_DIR)/CANopenNode/stack/ \
	$(UNITY_INC_FILES) \
	$(SRC_DIR)/od_interface_linked_list_v2/ \
	. 

DEFINES+= -D USE_OD_DESCRIPTION

C_FLAGS+=   #-Os 

all: run

include ../../compile_and_run.mk

clean: clean_generic

ifeq ($(MAKECMDGOALS),rtags)
include $(TOOLS_DIR)/rtags/rtags.mk
endif

ifeq ($(MAKECMDGOALS),pclint)
include $(TOOLS_DIR)/pclint/pclint.mk
endif

