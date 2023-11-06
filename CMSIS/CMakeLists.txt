cmake_minimum_required(VERSION 3.20)

file(GLOB CMSIS_SRC
     "*.c"
     "*.s"
     "GCC/*s"
)

add_library(CMSIS STATIC ${CMSIS_SRC})

target_link_options(CMSIS PRIVATE -nostdlib)

target_include_directories(CMSIS PUBLIC ./ GCC/)
