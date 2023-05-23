# toggle for various build stuff to control the platform we build for
# (valid values: dev, os3, os4)
OS := dev

# compilation based upon the OS-target
# includes flags
CC := gcc
CFLAGS :=
LDFLAGS :=

SRC_DIR := src
BUILD_DIR := build-$(OS)

ifeq ($(OS), os3)
	CC = m68k-amigaos-gcc
endif

ifeq ($(OS), os4)
	CC = ppc-amigaos-gcc
endif


tests: $(BUILD_DIR) $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/main.o -o tests

build-$(OS):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

clean:
	rm -rf build-dev build-os3 build-os4
	rm tests
