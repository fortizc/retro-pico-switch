cmake_minimum_required(VERSION 3.25.1)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(retro_pico_switch C CXX ASM)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# RPI initialization
set(PICO_CXX_ENABLE_EXCEPTIONS 1)
pico_sdk_init()

if (NOT CONTROLLER_TYPE OR (NOT ${CONTROLLER_TYPE} STREQUAL "N64" AND NOT ${CONTROLLER_TYPE} STREQUAL "GC"))
    MESSAGE(FATAL_ERROR "Missing controller type, CONTROLLER_TYPE must be defined (N64 or GC)")
endif()

add_compile_definitions(CFG_TUSB_CONFIG_FILE="${CMAKE_CURRENT_LIST_DIR}/src/tusb_config.h")

add_library(controller INTERFACE)
target_link_libraries(controller INTERFACE pico_stdlib tinyusb_device hardware_pio)
pico_generate_pio_header(controller ../src/Controller.pio)

file(GLOB MAIN_SRC src/*.cpp)
add_executable(${PROJECT_NAME} ${MAIN_SRC})
target_link_libraries(${PROJECT_NAME} pico_stdlib tinyusb_device hardware_pio controller)
