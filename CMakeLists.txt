cmake_minimum_required(VERSION 3.20)

if(NOT DEFINED run_number)
set(run_number 0)
endif()

project(VacuumMetr VERSION 1.0.0.${run_number} LANGUAGES C CXX ASM)

#############################################################
#                          CMSIS                            #
#############################################################

add_subdirectory(CMSIS)
include_directories(CMSIS)

#############################################################
#                       FREERTOS-KERNEL                     #
#############################################################
# set(FREERTOS_PORT GCC_ARM_CM3 CACHE STRING \"\")
# set(FREERTOS_HEAP 4 CACHE STRING \"\")
# add_library(freertos_config INTERFACE)
# target_include_directories(freertos_config SYSTEM INTERFACE  libs)
# target_compile_definitions(freertos_config INTERFACE projCOVERAGE_TEST=0)
# add_subdirectory(FreeRTOS-Kernel)
# include_directories(FreeRTOS-Kernel)

add_subdirectory(Utilities/FreeRTOS/Source)
include_directories(Utilities/FreeRTOS/Source)


#############################################################
#                          VacuumMetr                       #
#############################################################

file(GLOB TASKS_SOURCES Tasks/*.c)

file(GLOB GAMES_SOURCES Games/*.c)

file(GLOB LIBS_SOURCES libs/*.c)

set(TARGET ${PROJECT_NAME}.elf)
set(SOURCE Project/main.c)

add_executable(${TARGET} ${SOURCE} ${TASKS_SOURCES} ${GAMES_SOURCES} ${LIBS_SOURCES})

target_include_directories(${TARGET} PRIVATE Tasks Games libs Utilities/FreeRTOS/Source/include  Utilities/FreeRTOS/Source/portable/GCC/ARM_CM3 Project)

get_target_property(TARGET_SOURCE_DIR ${TARGET} SOURCE_DIR)

string(TIMESTAMP DATE "%d.%m.%Y")

set(VERSION_INFO "#define VERSION \"${PROJECT_VERSION}\"" )

set(DATE_INFO "#define DATE \"${DATE}\"" )

message("Build version = ${PROJECT_VERSION}\r\nBuild date = ${DATE}")

file(WRITE ${TARGET_SOURCE_DIR}/libs/Version.h ${VERSION_INFO} "\r\n" ${DATE_INFO})

target_link_libraries(${TARGET} CMSIS FreeRTOS)

target_link_options(${TARGET} PUBLIC -T ${TARGET_SOURCE_DIR}/libs/VacuumMetrProj.ld -Wl,-Map=${PROJECT_NAME}.map)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${TARGET} ${PROJECT_NAME}.bin)

install(FILES build/${PROJECT_NAME}.bin DESTINATION build/install)