# toggle for various build stuff to control the platform we build for
# (valid values: dev, os3, os4)
OS := dev

# compilation based upon the OS-target
# includes flags
CC := gcc
CFLAGS :=
LDFLAGS :=

SRC_DIR := src
TESTS_DIR := tests
BUILD_DIR := build-$(OS)

ifeq ($(OS), os3)
	CC = m68k-amigaos-gcc
endif

ifeq ($(OS), os4)
	CC = ppc-amigaos-gcc
endif


# libraries (mostly test related)
UNITY_FRAMEWORK_SRC := unity/src
UNITY_FIXTURE_SRC   := unity/extras/fixture/src

UNITY_OBJECTS := unity.o unity_fixture.o
AmiEnvMon: _main_platform_check $(BUILD_DIR) $(BUILD_DIR)/httpget.o $(BUILD_DIR)/main.o
	@echo "todo. actually do main stuff"

_main_platform_check:
	@if [ "dev" = $(OS) ]; then\
		echo "Building main program for dev target is NOT allowed"; \
		exit 1; \
	fi

tests: $(BUILD_DIR) $(BUILD_DIR)/main.o
	$(CC) $(BUILD_DIR)/main.o -o tests-$(OS)

build-$(OS):
	@mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@

clean:
	@rm -rf build-dev build-os3 build-os4
	@rm -f tests-os3 tests-os4 tests-dev
