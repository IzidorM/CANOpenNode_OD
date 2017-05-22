include ../../makefile.rules

ifndef $(TARGET)
TARGET=od_desc_gen
endif

PROJECT_BASE_DIR = .

TARGET_SRC_FILES= \
	$(SRC_DIR)/od_interface_linked_list_v2/canopen_od_interface.c \
	$(SRC_DIR)/od_description_generator/canopen_desc_gen.c

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
	$(SRC_DIR)/od_interface_linked_list_v2 \
	$(SRC_DIR)/od_description_generator \
	$(UNITY_INC_FILES) \
	. 

DEFINES+= #-D USE_OD_DESCRIPTION

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

