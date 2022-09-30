# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.8)

#set_property(DIRECTORY main PROPERTY CXX_STANDARD 17)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

#component_compile_options(-std=c++17)

project(rainbow)

