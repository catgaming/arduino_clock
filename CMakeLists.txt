cmake_minimum_required(VERSION 3.15)
project(AlarmClock LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11) # arduino cli only supports up to C++11 :(
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set our globals
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/source")
set(BOARD_FQBN "arduino:avr:uno")
set(PORT "COM4") # serial port from device manager. if communication fails check this first
set(ARDUINO_CLI "C:/Projects/arduino/tools/arduino-cli.exe") # for some reason adding it to PATH didn't work. manually defining it here instead

include_directories("${CMAKE_SOURCE_DIR}/thirdparty")

file(GLOB SOURCE
        "${SOURCE_DIR}/source.ino"
        "${SOURCE_DIR}/*.cpp"
        "${SOURCE_DIR}/*.h"
)
add_library(clion_indexer_dummy STATIC EXCLUDE_FROM_ALL ${SOURCE}) # dummy target so CLion can index all source files
set_target_properties(clion_indexer_dummy PROPERTIES LINKER_LANGUAGE CXX)

add_custom_target(build ALL
        DEPENDS "${SOURCE_DIR}/source.ino" # source.ino is required by the arduino cli
        DEPENDS ${SOURCE} # re run if anything changes
        COMMAND "${ARDUINO_CLI}" compile --fqbn ${BOARD_FQBN} ${SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_target(upload
        DEPENDS build # build before uploading every time. there's really no case where you want to just upload and not build
        COMMAND "${ARDUINO_CLI}" upload -p ${PORT} --fqbn ${BOARD_FQBN} ${SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# this adds // to a dummy file every time this target gets built
# clock.cpp includes this file, that way we get a new clock obj file (with a new __TIME__) every upload
add_custom_target(upload_force_new ALL
        COMMAND ${CMAKE_COMMAND} -E echo "//" >> ${SOURCE_DIR}/force_compile.h
        COMMAND "${ARDUINO_CLI}" compile --fqbn ${BOARD_FQBN} ${SOURCE_DIR}
        COMMAND "${ARDUINO_CLI}" upload -p ${PORT} --fqbn ${BOARD_FQBN} ${SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        VERBATIM
)

