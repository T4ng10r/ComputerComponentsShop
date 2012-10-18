cmake_minimum_required(VERSION 2.8)

project(ShopPlugin)

  include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
  include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy)
  include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins)
	include(${QT_USE_FILE})

	IF (Boost_FOUND)
		INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
		ADD_DEFINITIONS( "-DHAS_BOOST" )
	ENDIF(Boost_FOUND)
	
################################################################################
################################################################################
	SET ( PLUGIN_INTERFACE_SRC
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/ShopInterface.h
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/CompPriceData.h
		ShopPluginBase.cpp 
		ShopPluginBase.h
		ShopPluginNetwork.cpp
		ShopPluginNetwork.h 
		loggers.cpp 
	)
	SET ( PLUGIN_INTERFACE_MOC_HEADERS
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/ShopInterface.h
		ShopPluginNetwork.h 
		ShopPluginBase.h
	)
	QT4_WRAP_CPP(PLUGIN_INTERFACE_MOC ${PLUGIN_INTERFACE_MOC_HEADERS} OPTIONS
      -I${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
	
IF (WIN32)
	SOURCE_GROUP(moc FILES ${PLUGIN_INTERFACE_MOC})
endif(WIN32)

	add_library(ShopPluginInterface STATIC ${PLUGIN_INTERFACE_SRC} ${PLUGIN_INTERFACE_MOC})
	SET_TARGET_PROPERTIES(ShopPluginInterface PROPERTIES LINKER_LANGUAGE CXX)
  SET(LIBRARIES_LIST ${QT_LIBRARIES} tidy_lib)
  addLoggerLibrariesDependencies(ShopPluginInterface ${LIBRARIES_LIST})

################################################################################
################################################################################
include(CMakeLists_Proline.txt)
#include(CMakeLists_Proline_lib.txt)
include(CMakeLists_Komputronik.txt)
#include(CMakeLists_Komputronik_lib.txt)
include(CMakeLists_Arest.txt)
#include(CMakeLists_Arest_lib.txt)
################################################################################
################################################################################
