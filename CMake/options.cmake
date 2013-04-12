cmake_minimum_required(VERSION 2.8)


find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)

find_package(Boost REQUIRED)

set(Logger "log4Qt" CACHE STRING "Logger where to store logs from program executions")
set(LoggersNames "None;log4Qt")
set_property(CACHE Logger PROPERTY STRINGS ${LoggersNames})
SET ( LOG4QT_LIBRARY_NAME "log4qt" )

option(COMPCONF_FROZEN_COLUMN "Use FROZEN COLUMN view and model" TRUE)
option(COMPCONF_FROZEN_ROW "Use FROZEN ROW view and model" TRUE)
option(PLUGIN_DEBUG "Activate DEBUG for plugins" TRUE)
