cmake_minimum_required(VERSION 2.6)

project(ComputerConfShop_PROJECT)
ENABLE_TESTING()

include(${PROJECT_SOURCE_DIR}/CMake/options.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/plugins.cmake)

add_subdirectory(Sources)
