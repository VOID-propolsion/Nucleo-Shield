cmake_minimum_required(VERSION 3.12)
include(./gcc-arm-none-eabi.cmake)  

# Set the build directory
set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/build)

# ##############################################################################
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_VERBOSE_MAKEFILE OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/gcc-arm-none-eabi.cmake)

# Edit your MCU information up to the next border
project(Nucleo-Shield)
# Where the main project is
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/App) 

set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F446xx)
set(MCU_MODEL_ID STM32F446RETx)
set(CPU_PARAMETERS
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard)

set(STARTUP_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/startup_stm32f446xx.s)
set(MCU_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/${MCU_MODEL_ID}_FLASH.ld)
# ##############################################################################
set(EXECUTABLE ${CMAKE_PROJECT_NAME})
enable_language(C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# Headers
MACRO(GET_LIBRARY_HEADERS return_list)
    FILE(GLOB_RECURSE new_list "${CMAKE_SOURCE_DIR}/libs/*.h" "${CMAKE_SOURCE_DIR}/libs/*.hpp")
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

set(CUBEMX_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/${MCU_FAMILY}_HAL_Driver/Inc/Legacy
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Device/ST/${MCU_FAMILY}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/App
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/Target
    
    # App
    ${CMAKE_CURRENT_SOURCE_DIR}/App
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Recorder/LFS
    ${CMAKE_CURRENT_SOURCE_DIR}/App/Serial

    # Middlewares
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/littlefs
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/Third_Party/Unity
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library/Core/Inc

    # Test
    ${CMAKE_CURRENT_SOURCE_DIR}/Test
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/LPS22HB
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/LittleFS

)
set(PROJECT_INCLUDE_DIRECTORIES
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/LPS22HB
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/W25Q
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/SX1280
)
GET_LIBRARY_HEADERS(LIBRARY_INCLUDE_DIRECTORIES)


list(LENGTH LIBRARY_INCLUDE_DIRECTORIES LIBRARY_INCLUDE_DIRECTORIES_COUNT)
message(STATUS "[INFO] Found ${LIBRARY_INCLUDE_DIRECTORIES_COUNT} library directories.")

# Sources
file(GLOB_RECURSE STM32CUBEMX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/USB_DEVICE/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Middlewares/*.c
    ${CMAKE_CURRENT_SOURCE_DIR}/App/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Test/*.cpp

)
file(GLOB_RECURSE PROJECT_SOURCES FOLLOW_SYMLINKS
    ${PROJECT_DIR}/*.cpp
    ${PROJECT_DIR}/*.c)

# Executable files
add_executable(${EXECUTABLE}
    ${STM32CUBEMX_SOURCES} 
    ${PROJECT_SOURCES}
    ${STARTUP_SCRIPT}
    )

# Embedded macros(defines)
target_compile_definitions(${EXECUTABLE} PRIVATE
    ${MCU_MODEL}
    USE_HAL_DRIVER)

# Add header directories (AFTER add_executable !!)
target_include_directories(${EXECUTABLE} PRIVATE
    ${CUBEMX_INCLUDE_DIRECTORIES}
    ${PROJECT_INCLUDE_DIRECTORIES}
    ${LIBRARY_INCLUDE_DIRECTORIES}
)

# Compiler and linker options
target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    -w
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>:
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb>
    $<$<CONFIG:Release>:-Og -g0>)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    # --specs=nosys.specs
    -Wl,--start-group
    -lc
    -lm
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage)

# Set output directory to 'build'
set_target_properties(${EXECUTABLE} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR})

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>)

add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}>
    ${EXECUTABLE}.bin)


if (ENABLE_UNIT_TESTING)
    message(STATUS "ENABLE_UNIT_TESTING is ON")
    add_compile_definitions(ENABLE_UNIT_TESTS=1) # used to enable tests in code
  else()
    message(STATUS "ENABLE_UNIT_TESTING is OFF")
    add_compile_definitions(ENABLE_UNIT_TESTS=0) # used to disable tests in code
  endif()


# Check the host OS. if it is Windows, use Git Bash to run the test script.
# if(CMAKE_HOST_WIN32) # Windows
    set(CTEST_COMMAND python)
# else() # Linux
    # when on linux, just use simple bash or sh
    # set(CTEST_COMMAND python)
# endif()

include(CTest)
add_test( 
    NAME LPS22HB_TEST 
    COMMAND ${CTEST_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test/tester.py 1 \"COM3\"
)

add_test( 
    NAME LPS22HB_TEST2
    COMMAND ${CTEST_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test/tester.py 1 \"COM3\"
)

add_test( 
    NAME LITTLEFS_TEST 
    COMMAND ${CTEST_COMMAND} ${CMAKE_CURRENT_SOURCE_DIR}/scripts/test/tester.py 2 \"COM3\"
)