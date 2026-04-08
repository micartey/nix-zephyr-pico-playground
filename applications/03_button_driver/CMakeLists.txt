cmake_minimum_required(VERSION 3.20)

set(EXTRA_ZEPHYR_MODULES "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/rp2040_bootloader" "${CMAKE_CURRENT_SOURCE_DIR}/../../modules/button")

# set(DTC_OVERLAY_FILE "${CMAKE_CURRENT_SOURCE_DIR}/app.overlay")
# set(DTS_ROOT "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(my_cpp23_project)

# Use absolute pathing based on where this CMakeLists.txt lives
target_sources(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)

target_compile_features(app PUBLIC cxx_std_23)
