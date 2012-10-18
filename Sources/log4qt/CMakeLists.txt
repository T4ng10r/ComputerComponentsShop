cmake_minimum_required(VERSION 2.8)

project(log4qt)

SET( INCLUDE_DIR ${ComputerConfShop_PROJECT_SOURCE_DIR}/include)

#dodajemy dane dotyczace QT - sciezki, biblioteki
find_package(Qt4 COMPONENTS QtCore REQUIRED )
include(${QT_USE_FILE})

include_directories (${INCLUDE_DIR})
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/../)

SET( LOG4QT_HEADERS
    ${INCLUDE_DIR}/log4qt/appender.h
    ${INCLUDE_DIR}/log4qt/appenderskeleton.h 
    ${INCLUDE_DIR}/log4qt/basicconfigurator.h 
    ${INCLUDE_DIR}/log4qt/consoleappender.h 
    ${INCLUDE_DIR}/log4qt/dailyrollingfileappender.h 
    ${INCLUDE_DIR}/log4qt/fileappender.h 
    ${INCLUDE_DIR}/log4qt/hierarchy.h 
    ${INCLUDE_DIR}/log4qt/layout.h 
    ${INCLUDE_DIR}/log4qt/level.h 
    ${INCLUDE_DIR}/log4qt/log4qt.h 
    ${INCLUDE_DIR}/log4qt/logger.h 
    ${INCLUDE_DIR}/log4qt/loggerrepository.h 
    ${INCLUDE_DIR}/log4qt/loggingevent.h 
    ${INCLUDE_DIR}/log4qt/logmanager.h 
    ${INCLUDE_DIR}/log4qt/mdc.h 
    ${INCLUDE_DIR}/log4qt/ndc.h 
    ${INCLUDE_DIR}/log4qt/patternlayout.h 
    ${INCLUDE_DIR}/log4qt/propertyconfigurator.h 
    ${INCLUDE_DIR}/log4qt/rollingfileappender.h 
    ${INCLUDE_DIR}/log4qt/simplelayout.h 
    ${INCLUDE_DIR}/log4qt/ttcclayout.h 
    ${INCLUDE_DIR}/log4qt/writerappender.h 

    ${INCLUDE_DIR}/log4qt/helpers/classlogger.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/configuratorhelper.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/datetime.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/factory.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/initialisationhelper.h 
    ${INCLUDE_DIR}/log4qt/helpers/logerror.h 
    ${INCLUDE_DIR}/log4qt/helpers/logobject.h 
    ${INCLUDE_DIR}/log4qt/helpers/logobjectptr.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/optionconverter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/patternformatter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/properties.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/spi/filter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/debugappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/denyallfilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/nullappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/levelmatchfilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/levelrangefilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/listappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/stringmatchfilter.h
    )

SET ( LOG4QT_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/appenderskeleton.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/basicconfigurator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/consoleappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/dailyrollingfileappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/fileappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/classlogger.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/configuratorhelper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/datetime.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/factory.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/initialisationhelper.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/logerror.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/logobject.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/logobjectptr.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/optionconverter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/patternformatter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/properties.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/hierarchy.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/layout.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/level.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/log4qt.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/logger.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/loggerrepository.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/loggingevent.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/logmanager.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/mdc.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ndc.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/patternlayout.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/propertyconfigurator.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/rollingfileappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/simplelayout.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/spi/filter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/ttcclayout.cpp   
    ${CMAKE_CURRENT_SOURCE_DIR}/writerappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/debugappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/denyallfilter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/nullappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/levelmatchfilter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/levelrangefilter.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/listappender.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/stringmatchfilter.cpp
    )

SET( LOG4QT_MOC_HEADERS
    ${INCLUDE_DIR}/log4qt/appender.h
    ${INCLUDE_DIR}/log4qt/appenderskeleton.h 
    ${INCLUDE_DIR}/log4qt/consoleappender.h 
    ${INCLUDE_DIR}/log4qt/dailyrollingfileappender.h 
    ${INCLUDE_DIR}/log4qt/fileappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/helpers/configuratorhelper.h 
    ${INCLUDE_DIR}/log4qt/helpers/logobject.h 
    ${INCLUDE_DIR}/log4qt/layout.h 
    ${INCLUDE_DIR}/log4qt/logger.h 
    ${INCLUDE_DIR}/log4qt/patternlayout.h 
    ${INCLUDE_DIR}/log4qt/rollingfileappender.h 
    ${INCLUDE_DIR}/log4qt/simplelayout.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/spi/filter.h 
    ${INCLUDE_DIR}/log4qt/ttcclayout.h 
    ${INCLUDE_DIR}/log4qt/writerappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/debugappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/denyallfilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/nullappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/levelmatchfilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/levelrangefilter.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/listappender.h 
    ${CMAKE_CURRENT_SOURCE_DIR}/varia/stringmatchfilter.h
	)

    qt_wrap_cpp(log4qt_lib LOG4QT_MOC ${LOG4QT_MOC_HEADERS})
    SOURCE_GROUP(moc FILES ${LOG4QT_MOC})

  ADD_LIBRARY ( ${LOG4QT_LIBRARY_NAME} ${LOG4QT_SOURCES} ${LOG4QT_HEADERS}  ${LOG4QT_MOC} )
  target_link_libraries(${LOG4QT_LIBRARY_NAME} ${QT_LIBRARIES})
