cmake_minimum_required(VERSION 2.8)

set(Logger "log4Qt" CACHE STRING "Logger where to store logs from program executions")
set(LoggersNames "None;log4Qt;log4Cpp")
set_property(CACHE Logger PROPERTY STRINGS ${LoggersNames})

option(USE_BOOST "Use BOOST" TRUE)
option(COMPCONF_FROZEN_COLUMN "Use FROZEN COLUMN view and model" TRUE)
option(COMPCONF_FROZEN_ROW "Use FROZEN ROW view and model" TRUE)
option(PLUGIN_DEBUG "Activate DEBUG for plugins" TRUE)

SET ( LOG4QT_LIBRARY_NAME "log4qt" )
SET ( LOG4CPP_LIBRARY_NAME "log4cpp" )

macro(addLoggerLibrariesDependencies LibraryName OtherLibraries)
  IF (Logger STREQUAL "log4Qt")
    add_definitions(-DUSE_LOG4QT)
    SET(LOGGER_LIBRARY ${LOG4QT_LIBRARY_NAME})
  ELSEIF (Logger STREQUAL "log4Cpp")
    add_definitions(-DUSE_LOG4CPP)
    SET(LOGGER_LIBRARY ${LOG4CPP_LIBRARY_NAME})
  ELSE (Logger STREQUAL "log4Qt")
    SET(LOGGER_LIBRARY "")
  ENDIF (Logger STREQUAL "log4Qt")

  target_link_libraries(${LibraryName} ${${OtherLibraries}} ${LOGGER_LIBRARY})
  add_dependencies(${LibraryName} ${LOGGER_LIBRARY} ${${OtherLibraries}})
endmacro(addLoggerLibrariesDependencies)



