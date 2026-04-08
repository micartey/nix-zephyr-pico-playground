cmake_minimum_required(VERSION 3.20)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(blink_unit_tests)

target_sources(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp ${CMAKE_CURRENT_LIST_DIR}/../../src/blink.cpp)
target_include_directories(app PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../../src)
target_compile_options(app PRIVATE -std=c++23)