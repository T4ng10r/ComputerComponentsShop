cmake_minimum_required(VERSION 2.8)

project(ShopPlugin)

	SET( INCLUDE_DIR ${ComputerConfShop_SOURCE_DIR}/include)
	include_directories(${INCLUDE_DIR}/Plugins)
	include_directories(${INCLUDE_DIR}/tidy)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	find_package(Qt4 REQUIRED QtWebKit QtCore QtXml )
	include(${QT_USE_FILE})

	IF (COMP_CONF_USE_BOOST)
		find_package(Boost)
		IF (Boost_FOUND)
			INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
			ADD_DEFINITIONS( "-DHAS_BOOST" )
		ENDIF(Boost_FOUND)
	ENDIF(COMP_CONF_USE_BOOST)
	
################################################################################
################################################################################
	SET ( PLUGIN_INTERFACE_SRC
		${INCLUDE_DIR}/Plugins/ShopInterface.h
		${INCLUDE_DIR}/Plugins/CompPriceData.h
		ShopPluginBase.cpp 
		ShopPluginBase.h
		ShopPluginNetwork.cpp
		ShopPluginNetwork.h 
		loggers.cpp 
	)
	SET ( PLUGIN_INTERFACE_MOC_HEADERS
		${INCLUDE_DIR}/Plugins/ShopInterface.h
		ShopPluginNetwork.h 
		ShopPluginBase.h
	)
	QT4_WRAP_CPP(PLUGIN_INTERFACE_MOC ${PLUGIN_INTERFACE_MOC_HEADERS} OPTIONS -I${INCLUDE_DIR})
	
IF (WIN32)
	SOURCE_GROUP(moc FILES ${PLUGIN_INTERFACE_MOC})
endif(WIN32)

	add_library(ShopPluginInterface STATIC ${PLUGIN_INTERFACE_SRC} ${PLUGIN_INTERFACE_MOC})
	SET_TARGET_PROPERTIES(ShopPluginInterface PROPERTIES LINKER_LANGUAGE CXX)
	IF (COMP_CONF_USE_LOG4QT)
		target_link_libraries(ShopPluginInterface ${QT_LIBRARIES} ${LOG4QT_LIBRARY_NAME} tidy_lib)
  	    add_dependencies(ShopPluginInterface ${LOG4QT_LIBRARY_NAME} tidy_lib)
		add_definitions(-DUSE_LOG4QT)
    ELSEIF (COMP_CONF_USE_LOG4CPP)
		target_link_libraries(ShopPluginInterface ${QT_LIBRARIES} ${LOG4CPP_LIBRARY_NAME} tidy_lib)
  	    add_dependencies(ShopPluginInterface ${LOG4CPP_LIBRARY_NAME} tidy_lib)
		add_definitions(-DUSE_LOG4CPP)
    ELSEIF (COMP_CONF_USE_BOOST_LOG)
		target_link_libraries(ShopPluginInterface ${QT_LIBRARIES} tidy_lib)
		add_definitions(-DUSE_BOOSTLOG)
    ELSE (COMP_CONF_USE_LOG4QT)
		target_link_libraries(ShopPluginInterface ${QT_LIBRARIES} tidy_lib)
    ENDIF (COMP_CONF_USE_LOG4QT)

################################################################################
################################################################################
include(CMakeLists_Proline.txt)
include(CMakeLists_Proline_lib.txt)
include(CMakeLists_Komputronik.txt)
include(CMakeLists_Komputronik_lib.txt)
include(CMakeLists_Arest.txt)
include(CMakeLists_Arest_lib.txt)
################################################################################
################################################################################
