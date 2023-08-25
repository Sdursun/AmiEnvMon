# toggle for various build stuff to control the platform we build for
# (valid values: dev, os3, os4)
OS := dev

# compilation based upon the OS-target
# includes flags
CC := gcc
CFLAGS := -fprofile-arcs -ftest-coverage
LDFLAGS :=

SRC_DIR := src
TESTS_DIR := tests
BUILD_DIR := build-$(OS)

# TODO: split up the cflags and ldflags better. lazy here
ifeq ($(OS), os3)
	CC = m68k-amigaos-gcc
	CFLAGS = -mcrt=clib2 -m68020-60 -msoft-float
    LDFLAGS = -lmui -lm -lc -lg -lnet -mcrt=clib2
endif

ifeq ($(OS), os4)
	CC = ppc-amigaos-gcc
	CFLAGS =  -mcrt=newlib -mcpu=powerpc
	LDFLAGS = -lauto -mcpu=powerpc
endif


# libraries (mostly test related)
JSON_LIB_SRC        := jsonlib
UNITY_FRAMEWORK_SRC := unity/src
UNITY_FIXTURE_SRC   := unity/extras/fixture/src

UNITY_OBJECTS := unity.o unity_fixture.o
UNITY_OBJECTS := $(foreach obj,$(UNITY_OBJECTS),$(addprefix $(BUILD_DIR)/,$(obj))) 

TEST_OBJECTS := string_utils.o string_utils_isstringempty_test.o envmondata.o envmondata_test.o httpget_responseparse.o httpget_responseparse_test.o testmain.o
TEST_OBJECTS := $(foreach obj,$(TEST_OBJECTS),$(addprefix $(BUILD_DIR)/,$(obj))) 
MAIN_OBJECTS := string_utils.o envmondata.o httpget_responseparse.o gui.o httpget.o main.o
MAIN_OBJECTS := $(foreach obj,$(MAIN_OBJECTS),$(addprefix $(BUILD_DIR)/,$(obj)))


AmiEnvMon: _main_platform_check $(BUILD_DIR) $(BUILD_DIR)/main.o $(MAIN_OBJECTS)
	$(CC) $(MAIN_OBJECTS) -o AmiEnvMon $(CFLAGS) $(LDFLAGS)

Settings: _main_platform_check $(BUILD_DIR) $(BUILD_DIR)/settings_gui.o
	$(CC) $(BUILD_DIR)/settings_gui.o -o Settings $(CFLAGS) $(LDFLAGS)	

_main_platform_check:
	@if [ "dev" = $(OS) ]; then\
		echo "Building main program for dev target is NOT allowed"; \
		exit 1; \
	fi

tests: $(BUILD_DIR) $(UNITY_OBJECTS) $(TEST_OBJECTS)
	$(CC) $(UNITY_OBJECTS) $(TEST_OBJECTS) -o tests-$(OS) $(CFLAGS) $(LDFLAGS)

build-$(OS):
	@mkdir -p $(BUILD_DIR)

# TODO: any way to combine the compilations?
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) -I$(JSON_LIB_SRC) -I/opt/amiga3/m68k-amigaos/ndk-include -I/opt/amiga3/include

$(BUILD_DIR)/%.o: $(TESTS_DIR)/%.c
	$(CC) -c $< $(CFLAGS) -I$(SRC_DIR) -I$(UNITY_FRAMEWORK_SRC) -I$(UNITY_FIXTURE_SRC) -DUNITY_FIXTURE_NO_EXTRAS -o $@

$(BUILD_DIR)/%.o: $(UNITY_FRAMEWORK_SRC)/%.c
	$(CC) -c $< -o $@ $(CFLAGS)

$(BUILD_DIR)/%.o: $(UNITY_FIXTURE_SRC)/%.c
	$(CC) -c $< $(CFLAGS) -I$(UNITY_FRAMEWORK_SRC) -I$(UNITY_FIXTURE_SRC) -DUNITY_FIXTURE_NO_EXTRAS -o $@

clean:
	@rm -rf build-dev build-os3 build-os4
	@rm -f tests-os3 tests-os4 tests-dev
	@rm -f AmiEnvMon
