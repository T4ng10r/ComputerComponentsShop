cmake_minimum_required(VERSION 2.8)


find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)

#boost build with bjam threading=multi runtime-link=static
SET(Boost_USE_MULTITHREADED ON)
SET(Boost_USE_STATIC_LIBS ON)
SET(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED COMPONENTS thread system date_time chrono)

if (UNIX)
  #SET (BIT_64 false)
  set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
  # -fPIC
endif()


set(Logger "log4Qt" CACHE STRING "Logger where to store logs from program executions")
set(LoggersNames "None;log4Qt")
set_property(CACHE Logger PROPERTY STRINGS ${LoggersNames})
SET ( LOG4QT_LIBRARY_NAME "log4qt" )

option(COMPCONF_FROZEN_COLUMN "Use FROZEN COLUMN view and model" TRUE)
option(COMPCONF_FROZEN_ROW "Use FROZEN ROW view and model" TRUE)
option(PLUGIN_DEBUG "Activate DEBUG for plugins" TRUE)

foreach(flag_var
        CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE
        CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
   if(${flag_var} MATCHES "/MD")
      string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
   endif(${flag_var} MATCHES "/MD")
endforeach(flag_var)
if (MSVC)
	SET(CMAKE_SHARED_LINKER_FLAGS /NODEFAULTLIB:\"LIBCMTD.lib\")
endif()
