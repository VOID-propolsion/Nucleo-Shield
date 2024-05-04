.PHONY: all build cmake clean format

TARGET = Astrea

BUILD_DIR := build
BUILD_TYPE ?= Debug

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_TOOLCHAIN_FILE=gcc-arm-none-eabi.cmake \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		-G "Unix Makefiles"

cmake: ${BUILD_DIR}/Makefile

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory

SRCS := $(shell find . -name '*.[ch]' -or -name '*.[ch]pp')
format: $(addsuffix .format,${SRCS})

%.format: %
	clang-format -i $<

clean:
	cmd /c rd /s /q $(BUILD_DIR)

erase: $(BUILD_DIR)/$(TARGET).elf
	openocd -f ./openocd.cfg -c "init; reset halt; stm32f4x mass_erase 0; exit"

flash: $(BUILD_DIR)/$(TARGET).elf
	openocd -f ./openocd.cfg -c "program $(BUILD_DIR)/$(TARGET).elf verify reset exit"