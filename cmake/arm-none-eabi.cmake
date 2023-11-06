SET(CMAKE_TOOLCHAIN_PREFIX arm-none-eabi-) 

find_program(CMAKE_ASM_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}gcc)
find_program(CMAKE_C_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}gcc)
find_program(CMAKE_CXX_COMPILER ${CMAKE_TOOLCHAIN_PREFIX}g++)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(MCU_FLAGS "-mcpu=cortex-m3 -mthumb -Wall -fdata-sections -ffunction-sections")
set(CMAKE_C_FLAGS "${MCU_FLAGS}"  CACHE INTERNAL "c compiler flags")
set(CMAKE_CXX_FLAGS "${MCU_FLAGS}" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_ASM_FLAGS "${MCU_FLAGS}" CACHE INTERNAL "asm compiler flags")

set(CMAKE_C_FLAGS_DEBUG "${MCU_FLAGS} -g -Og -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${MCU_FLAGS} -g -Og -DDEBUG")
set(CMAKE_ASM_FLAGS_DEBUG "${MCU_FLAGS} -g -Og -DDEBUG")

set(CMAKE_C_FLAGS_RELEASE "${MCU_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${MCU_FLAGS}")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,--print-memory-usage --specs=nosys.specs -Wl,--gc-sections")