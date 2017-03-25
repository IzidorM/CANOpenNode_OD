ifndef $(TARGET)
TARGET=main
endif

TOOLS_DIR = ../../tools/
SRC_DIR = ../../
BIN = bin

C_COMPILER=gcc



PROJECT_BASE_DIR = .

TARGET_SRC_FILES= \
	$(SRC_DIR)/od_interface_linked_list/canopen_od_interface.c

#$(SRC_DIR)/lib/utils/utils_data_dump.c

#LIB_SRC_FILES= \
#	$(SRC_DIR)/lib/opensource/printf.c


TARGET_SRC_TEST_FILES= $(TARGET).c

TARGET_SRC_ALL=$(TARGET_SRC_FILES) \
	$(LIB_SRC_FILES) \
	$(TARGET_SRC_TEST_FILES) \

INC_DIRS= \
	$(SRC_DIR)/od_interface_linked_list \
	. 

DEFINES+= 

C_FLAGS+=-g -ggdb3 -Wall -std=c11 -pedantic -fprofile-arcs -ftest-coverage


all: $(TARGET)

INC_DIRS_GCC = $(patsubst %,-I%, $(INC_DIRS))

$(TARGET): $(TARGET_SRC_ALL)
	@$(C_COMPILER) $(C_FLAGS) $(DEFINES) $(INC_DIRS_GCC) $(SYMBOLS) $(TARGET_SRC_ALL) -o $(TARGET)
	@echo ""
	@size $(TARGET)
	@echo ""

clean:
	@rm -f $(TARGET) *_Runner.c src_files inc_dirs *.gcno *.gcda *.info *.gcov

include $(TOOLS_DIR)/rtags/rtags.mk

