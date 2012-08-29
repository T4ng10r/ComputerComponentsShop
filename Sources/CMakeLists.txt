cmake_minimum_required(VERSION 2.8)

project(ComputerConfShop_Sources)

add_subdirectory(Main)
add_subdirectory(UnitTest)
add_subdirectory(Plugins)
add_subdirectory(tidy)
if (COMP_CONF_USE_LOG4QT)
add_subdirectory(log4qt)
endif (COMP_CONF_USE_LOG4QT)
if (COMP_CONF_USE_LOG4CPP)
add_subdirectory(log4cpp)
endif (COMP_CONF_USE_LOG4CPP)
