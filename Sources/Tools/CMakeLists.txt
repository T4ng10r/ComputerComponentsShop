cmake_minimum_required(VERSION 2.8)

project(Tools)
include(${QT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include)

SET ( TOOLS_SRC
qtTools.cpp
${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Tools/qtTools.h 
loggers.cpp
${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Tools/loggers.h 
)

IF (WIN32)
	SOURCE_GROUP(tools FILES ${TOOLS_SRC})
endif(WIN32)
  IF (Logger STREQUAL "log4Qt")
    add_definitions(-DUSE_LOG4QT)
    SET(LOGGER_LIBRARY ${LOG4QT_LIBRARY_NAME})
  ELSE (Logger STREQUAL "log4Qt")
    SET(LOGGER_LIBRARY "")
  ENDIF (Logger STREQUAL "log4Qt")

add_library(Tools_Lib STATIC ${TOOLS_SRC}
		)
	target_link_libraries(Tools_Lib ${LOGGER_LIBRARY} )
	add_dependencies(Tools_Lib ${LOGGER_LIBRARY})

