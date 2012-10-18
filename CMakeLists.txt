cmake_minimum_required(VERSION 2.6)

project(ComputerConfShop_PROJECT)

include(${PROJECT_SOURCE_DIR}/CMake/options.cmake)
include(${PROJECT_SOURCE_DIR}/CMake/plugins.cmake)

find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)

IF (USE_BOOST)
	find_package(Boost)
ENDIF(USE_BOOST)

add_subdirectory(Sources)
