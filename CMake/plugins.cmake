cmake_minimum_required(VERSION 2.8)

function(AddPluginSharedLibrary PluginName SourceFilesList MocHeadersList
        ExtraFilesList bCreateStaticLib)
	include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include(${QT_USE_FILE})
	IF (Boost_FOUND)
		INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
		ADD_DEFINITIONS( "-DHAS_BOOST" )
	ENDIF(Boost_FOUND)

  SET ( PLUGIN_SRC ${${SourceFilesList}} )
  SET ( PLUGIN_MOC_HEADERS ${${MocHeadersList}} )
  SET ( PLUGIN_EXTRA ${${ExtraFilesList}} )

  QT4_WRAP_CPP(${PluginName}_MOC ${PLUGIN_MOC_HEADERS} OPTIONS -I${ComputerConfShop_SOURCE_DIR}/include)
  QT4_ADD_RESOURCES( ${PluginName}_RCC ${PluginName}.qrc  )

	IF (WIN32)
		SOURCE_GROUP(moc FILES ${PLUGIN_MOC})
	ENDIF(WIN32)

	add_definitions(${QT_DEFINITIONS})
  SET(LIBRARIES_LIST ${QT_LIBRARIES} ShopPluginInterface tidy_lib)
  if (${bCreateStaticLib})
    add_library(${PluginName}Plugin_TestLib STATIC ${PLUGIN_SRC} ${PLUGIN_MOC}
        ${PLUGIN_RCC} ${PLUGIN_EXTRA} ${${PluginName}_MOC} ${PLUGIN_EXTRA})
    set_target_properties(${PluginName}Plugin_TestLib PROPERTIES COMPILE_DEFINITIONS WITHOUT_INTERFACE)
    addLoggerLibrariesDependencies(${PluginName}Plugin_TestLib LIBRARIES_LIST)
  else (${bCreateStaticLib})
    add_library(${PluginName}Plugin SHARED ${PLUGIN_SRC} ${PLUGIN_MOC}
        ${PLUGIN_RCC} ${PLUGIN_EXTRA} ${${PluginName}_MOC} ${PLUGIN_EXTRA})
    set_target_properties(${PluginName}Plugin PROPERTIES COMPILE_DEFINITIONS QT_PLUGIN)
    set_target_properties(${PluginName}Plugin PROPERTIES COMPILE_DEFINITIONS QT_SHARED)
    addLoggerLibrariesDependencies(${PluginName}Plugin LIBRARIES_LIST)
  endif (${bCreateStaticLib})
  ##here we added dependencies to loggers

endfunction(AddPluginSharedLibrary)

function(InstallPlugin LibraryName)
  #message(STATUS "LibraryName: " ${LibraryName} )
	#####path to binary PluginShopPlugin library
  GET_TARGET_PROPERTY(${LibraryName}Plugin_LIB_PATH ${LibraryName}Plugin LOCATION)
  #message(STATUS "LibraryName_LIB_PATH: " ${${LibraryName}Plugin_LIB_PATH} )
	
  #get locations were 
  SET(${LibraryName}Plugin_BIN_DIR ${${LibraryName}Plugin_BINARY_DIR}/${CMAKE_CFG_INTDIR}/)
  #message(STATUS ${LibraryName} "Plugin_BIN_DIR: " ${${LibraryName}Plugin_BIN_DIR} ) 

  #remove from path to binary lib whole directory part, this way we have
  #library file name
	#REPLACE 'pattern_string' 'replace_string' output_var 'input'
  STRING(REPLACE ${${LibraryName}Plugin_BIN_DIR} "" ${LibraryName}_PLUGIN_DLL
      ${${LibraryName}Plugin_LIB_PATH})
	
	#####path to UnitTest BIN directory
	SET(UT_PLUGINS_DIR ${UnitTest_BINARY_DIR}/plugins/${${LibraryName}_PLUGIN_DLL})
  #message(STATUS "UnitTests Bin Dir: " ${UnitTest_BINARY_DIR})
	
	#####path to Main BIN directory
	SET(CCS_PLUGINS_DIR ${ComputerConfShop_BINARY_DIR}/plugins/${${LibraryName}_PLUGIN_DLL})
  #message(STATUS "ComputerConf Bin Dir: " ${ComputerConfShopMain_BINARY_DIR})

	add_custom_command(TARGET ${LibraryName}Plugin POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy ${${LibraryName}Plugin_LIB_PATH}  ${UT_PLUGINS_DIR})
	
 add_custom_command(TARGET ${LibraryName}Plugin POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy ${${LibraryName}Plugin_LIB_PATH}  ${CCS_PLUGINS_DIR})
		 
#		 ProlineSelectors.xml
    
endfunction(InstallPlugin)

function(addLoggerLibrariesDependencies LibraryName OtherLibraries)
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
endfunction(addLoggerLibrariesDependencies)

