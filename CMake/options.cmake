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
