
rtags:
	@echo "Running rtags"
	@rc --project-root $(PROJECT_BASE_DIR) -c gcc \
	$(CFLAGS) \
	$(DEFINES) \
	$(INC_DIRS_GCC) \
	$(TARGET_SRC_FILES) \
	$(TARGET_SRC_TEST_FILES) \
	@echo "Rtags finished"
