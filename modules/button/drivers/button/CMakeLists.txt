zephyr_library()

zephyr_library_sources(button.cpp)

# Add .h files to the CMake search
zephyr_include_directories(.)
