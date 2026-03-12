cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(02-control C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(02-control
    main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stdio-task/stdio-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/protocol-task/protocol-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
)

target_link_libraries(02-control
    pico_stdlib
    hardware_gpio
    pico_stdio_usb          # <-- добавляем для работы USB COM-порта
)

pico_add_extra_outputs(02-control)