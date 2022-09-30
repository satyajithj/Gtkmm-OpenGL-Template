# TARGET_EXEC := tiling

BUILD_DIR := ./build

SRC_DIRS := ./src
SRC_DIRS += ./extern

# Find all the C and C++ files we want to compile
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Includes
# INC_DIRS := ./extern
INC_FLAGS := $(addprefix -I,$(SRC_DIRS))

# Linker flags
# LD_FLAGS := 

GTK_MM_FLAGS := $(shell pkg-config gtkmm-4.0 --cflags --libs)
GLIB_MM_FLAGS := $(shell pkg-config glibmm-2.68 --cflags --libs)
LIB_EPOXY_FLAGS := $(shell pkg-config epoxy --cflags --libs)

CFLAGS := $(INC_FLAGS) $(GTK_MM_FLAGS) $(GLIB_MM_FLAGS) $(LIB_EPOXY_FLAGS)

tiling:
	@if [ -d $(BUILD_DIR) ]; then rm -Rf $(BUILD_DIR); fi
	@mkdir -p $(BUILD_DIR)
	@g++ $(SRCS) $(CFLAGS) -o $(BUILD_DIR)/ogl_app
	@echo "Application compiled"

clean:
	@if [ -d $(BUILD_DIR) ]; then rm -Rf $(BUILD_DIR); echo "Build directory cleaned"; fi