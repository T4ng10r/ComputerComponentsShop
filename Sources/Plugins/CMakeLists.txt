cmake_minimum_required(VERSION 2.8)

project(ShopPlugin)

	include(${QT_USE_FILE})
	include_directories(${log4qt_SOURCE_DIR})
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
	include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy)
	include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins)
	include_directories(${Boost_INCLUDE_DIR})

	ADD_DEFINITIONS( "-DHAS_BOOST" )
	
################################################################################
################################################################################
	SET ( PLUGIN_INTERFACE_NETWORK_SRC
		Network/LoadNetworkObject.cpp 
		Network/LoadNetworkObject.h 
		Network/LoadNetworkObjectPrivate.h 
	)
	SET ( PLUGIN_TOOLS 
	${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Tools/loggers.h 	
	${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Tools/qtTools.h 	
		)
	SET ( PLUGIN_INTERFACE_SRC
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/ShopInterface.h
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/CompPriceData.h
		ShopPluginBase.cpp 
		ShopPluginBase.h
		ShopPluginBase_p.h
	)
	SET ( PLUGIN_INTERFACE_MOC_HEADERS
		${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/ShopInterface.h
		${CMAKE_CURRENT_SOURCE_DIR}/Network/LoadNetworkObject.h
		${CMAKE_CURRENT_SOURCE_DIR}/Network/LoadNetworkObjectPrivate.h
		ShopPluginNetwork.h 
		ShopPluginBase.h
	)
	QT4_WRAP_CPP(PLUGIN_INTERFACE_MOC ${PLUGIN_INTERFACE_MOC_HEADERS} OPTIONS
      -I${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
	
IF (WIN32)
	SOURCE_GROUP(moc FILES ${PLUGIN_INTERFACE_MOC})
	SOURCE_GROUP(Network FILES ${PLUGIN_INTERFACE_NETWORK_SRC})
	SOURCE_GROUP(tools FILES ${PLUGIN_TOOLS})
endif(WIN32)

	add_library(ShopPluginInterface STATIC ${PLUGIN_INTERFACE_SRC} 
		${PLUGIN_INTERFACE_NETWORK_SRC} ${PLUGIN_TOOLS}
		${PLUGIN_INTERFACE_MOC} 
		)
	SET_TARGET_PROPERTIES(ShopPluginInterface PROPERTIES LINKER_LANGUAGE CXX)
	SET(LIBRARIES_LIST ${QT_LIBRARIES} tidy_lib Tools_Lib)
	addLoggerLibrariesDependencies(ShopPluginInterface ${LIBRARIES_LIST})

################################################################################
################################################################################
include(CMakeLists_Proline.txt)
include(CMakeLists_Komputronik.txt)
include(CMakeLists_Arest.txt)
################################################################################
################################################################################
