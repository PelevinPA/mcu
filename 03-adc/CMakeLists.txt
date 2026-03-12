cmake_minimum_required(VERSION 3.13)
include(pico_sdk_import.cmake)

project(03-adc C CXX ASM)                     # <-- имя изменено

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(03-adc
    main.c
    ${CMAKE_CURRENT_SOURCE_DIR}/stdio-task/stdio-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/protocol-task/protocol-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led-task/led-task.c
    ${CMAKE_CURRENT_SOURCE_DIR}/adc-task/adc-task.c   # <-- добавлен модуль АЦП
)

target_link_libraries(03-adc
    pico_stdlib
    hardware_gpio
    pico_stdio_usb
    hardware_adc                                     # <-- библиотека АЦП
)

pico_add_extra_outputs(03-adc)