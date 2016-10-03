cmake_minimum_required(VERSION 3.6)
project(pdp_11_emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES core/Unibus.cpp core/Unibus.h)
set(SOURCE_FILES ${SOURCE_FILES} core/UnibusDevice.h)
set(SOURCE_FILES ${SOURCE_FILES} core/Memory.cpp core/Memory.h)
set(SOURCE_FILES ${SOURCE_FILES} core/CPU.cpp core/CPU.h)
set(SOURCE_FILES ${SOURCE_FILES} main.cpp common.h)
set(SOURCE_FILES ${SOURCE_FILES} core/io_devices/PDPKeyboard.cpp core/io_devices/PDPKeyboard.h)
set(SOURCE_FILES ${SOURCE_FILES} core/io_devices/PDPTapeWriter.cpp core/io_devices/PDPTapeWriter.h)
set(SOURCE_FILES ${SOURCE_FILES} core/io_devices/PDPDisplayAdapter.cpp core/io_devices/PDPDisplayAdapter.h)
add_executable(pdp_11_emulator ${SOURCE_FILES})