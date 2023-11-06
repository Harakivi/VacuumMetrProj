cmake_minimum_required(VERSION 3.20)

file(GLOB FreeRTOS_SRC
     "*.c"
     "*.s"
)

add_library(FreeRTOS STATIC ${FreeRTOS_SRC} ./portable/GCC/ARM_CM3/port.c ./portable/MemMang/heap_4.c)

target_link_options(FreeRTOS PRIVATE -nostdlib)

target_include_directories(FreeRTOS PUBLIC ./include ../../../libs ./portable/GCC/ARM_CM3)