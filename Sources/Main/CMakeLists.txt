cmake_minimum_required(VERSION 2.8)

project(ComputerConfShop)

#dodajemy dane dotyczace QT - sciezki, biblioteki
find_package(Qt4 REQUIRED QtNetwork QtGui QtCore QtXml QtTest)
include(${QT_USE_FILE})

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ADD_DEFINITIONS( "-DHAS_BOOST" )


IF (COMPCONF_FROZEN_COLUMN)
  add_definitions(-DVIEW_WITH_FROZEN_COLUMN)
ENDIF (COMPCONF_FROZEN_COLUMN)
IF (COMPCONF_FROZEN_ROW)
  add_definitions(-DVIEW_WITH_FROZEN_ROW)
ENDIF (COMPCONF_FROZEN_ROW)

	include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${log4qt_SOURCE_DIR})

    SET ( SOURCES_TOOLS
		${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Tools/loggers.h 	
		${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Tools/qtTools.h 	
		)
    SET ( SOURCES_OTHER
        main.cpp
        ${SOURCES_TOOLS}
		  )
		  
    SET ( SOURCES_DATA_MODELS
		Data/ComputerConfModel.cpp
		Data/ComputerConfModel.h
    )
    SET ( SOURCES_DATA
		Data/CDataThread.cpp         
		Data/CDataThread.h
		Data/Enumerations.h
		Data/ProxySettings.h 
		Data/CProxySettings.cpp
		Data/CProxySettings.h 
		Data/ComponentConfXMLReader.cpp 
		Data/ComponentConfXMLReader.h
		Data/CRefreshData.cpp
		Data/CRefreshData.h
		  )
    
    SET ( SOURCES_GUI
		GUI/MainWindow.cpp
		GUI/MainWindow.h
		GUI/ProxySettingsGUI.cpp 
		GUI/ProxySettingsGUI.h
    )
    SET ( SOURCES_GUI_VIEW
		GUI/ComponentHeaderView.cpp
		GUI/ComponentHeaderView.h
		GUI/ShopHeaderView.cpp
		GUI/ShopHeaderView.h
		GUI/ShopHeaderEntry.cpp
		GUI/ShopHeaderEntry.h
		GUI/ComponentHeaderEntry.cpp 
		GUI/ComponentHeaderEntry.h
		GUI/FrozenTableView.cpp 
		GUI/FrozenTableView.h
		GUI/CustomHeaderView.cpp
		GUI/CustomHeaderView.h
		GUI/CustomHeaderViewEntry.h
	    GUI/ComponentDelegate.cpp 
		GUI/ComponentDelegate.h
    )

    SET ( SOURCES_PLUGINS
		${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/ShopInterface.h
		${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/CompPriceData.h
        )
   
    SET ( SOURCES_MOC_HEADERS
		${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins/ShopInterface.h
		${CMAKE_CURRENT_SOURCE_DIR}/Data/ComputerConfModel.h
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CDataThread.h
		${CMAKE_CURRENT_SOURCE_DIR}/Data/CRefreshData.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/MainWindow.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/ComponentHeaderEntry.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/ShopHeaderEntry.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/ProxySettingsGUI.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/FrozenTableView.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/CustomHeaderView.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/CustomHeaderViewEntry.h
		${CMAKE_CURRENT_SOURCE_DIR}/GUI/ComponentDelegate.h
        )
        
    #qt4_wrap_cpp(ComputerConfShop SOURCES_MOC ${SOURCES_MOC_HEADERS})
    qt4_wrap_cpp(SOURCES_MOC ${SOURCES_MOC_HEADERS})
    QT4_ADD_RESOURCES( SOURCES_RCC ConfCompShop.qrc )
    
IF (WIN32)
    SOURCE_GROUP(Data FILES ${SOURCES_DATA})
    SOURCE_GROUP(Data\\Models FILES ${SOURCES_DATA_MODELS})
    SOURCE_GROUP(GUI FILES ${SOURCES_GUI})
    SOURCE_GROUP(GUI\\View FILES ${SOURCES_GUI_VIEW})
    SOURCE_GROUP(moc FILES ${SOURCES_MOC})
    SOURCE_GROUP(Plugins FILES ${SOURCES_PLUGINS})
    SOURCE_GROUP(tools FILES ${SOURCES_TOOLS})
ENDIF (WIN32)
    
IF (WIN32)
    add_executable(ComputerConfShop ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_GUI_VIEW} ${SOURCES_OTHER} ${SOURCES_PLUGINS}
        ${SOURCES_RCC} ${SOURCES_DATA_MODELS} main.cpp TODO.txt )
ELSE (WIN32)
    add_executable(ComputerConfShop ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_GUI_VIEW} ${SOURCES_OTHER} ${SOURCES_PLUGINS}
        ${SOURCES_RCC} ${SOURCES_DATA_MODELS} main.cpp TODO.txt )
ENDIF (WIN32)

    add_library(ComputerConfShop_lib STATIC ${SOURCES_DATA} ${SOURCES_MOC} 
        ${SOURCES_GUI} ${SOURCES_GUI_VIEW} ${SOURCES_OTHER} ${SOURCES_PLUGINS}
        main.cpp ${SOURCES_RCC} ${SOURCES_DATA_MODELS})
        
	SET(LIBRARIES_LIST ${QT_LIBRARIES} ShopPluginInterface ProlineShopPlugin KomputronikShopPlugin ArestShopPlugin Tools_Lib ${Boost_LIBRARIES})
  	addLoggerLibrariesDependencies(ComputerConfShop LIBRARIES_LIST)
	addLoggerLibrariesDependencies(ComputerConfShop_lib LIBRARIES_LIST)
