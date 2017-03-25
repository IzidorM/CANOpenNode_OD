
INC_DIRS_GCC = $(patsubst %,-I%, $(INC_DIRS))

run: $(TARGET)
	@echo "Running tests"
	@/usr/bin/time --format="\nTest run time: %E, Max memory: %M Kb" ./$(TARGET)
	@printf "\nGCOV:\n"
	gcov $(TARGET)


$(TARGET): $(TARGET_SRC_ALL)
	@$(C_COMPILER) $(C_FLAGS) $(DEFINES) $(INC_DIRS_GCC) $(SYMBOLS) $(TARGET_SRC_ALL) -o $(TARGET)
	@echo ""
	@size $(TARGET)
	@echo ""

clean_generic:
	@rm -f $(TARGET) *_Runner.c src_files inc_dirs *.gcno *.gcda *.info *.gcov

