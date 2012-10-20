cmake_minimum_required(VERSION 2.6)

project(UnitTest)

#dodajemy dane dotyczace QT - sciezki, biblioteki
#find_package(Qt4 COMPONENTS QtCore QtGui QtTest QtNetwork REQUIRED )
include(${QT_USE_FILE})

	include_directories(${ComputerConfShop_SOURCE_DIR})
	include_directories(${ComputerConfShop_Sources_SOURCE_DIR})
	include_directories(${ComputerConfShop_Sources_SOURCE_DIR}/Plugins)
	SET( INCLUDE_DIR ${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
	include_directories(${INCLUDE_DIR})
	include_directories(${INCLUDE_DIR}/Plugins)
	include_directories(${INCLUDE_DIR}/tidy)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})
	include_directories(${ComputerConfShop_SOURCE_DIR}/Sources/Main)
	include_directories(${ComputerConfShop_SOURCE_DIR}/Sources/Plugins)

####################

####################
SET ( SOURCES_TEST_DATA
		CComputerConfModelTest.cpp 
		CComputerConfModelTest.h
		ProxySettingClass.cpp 
		CCustomHeadersTest.cpp
		CCustomHeadersTest.h 
	  )

SET ( SOURCES_TEST_PLUGINS_DATA
		PluginsTests/CArestParserTest.cpp
		PluginsTests/CArestParserTest.h
		PluginsTests/CArestPluginTest.cpp
		PluginsTests/CArestPluginTest.h
		PluginsTests/ShopPluginNetworkTest.cpp 
		PluginsTests/ShopPluginNetworkTest.h
		PluginsTests/CProlinePluginTest.cpp 
		PluginsTests/CProlinePluginTest.h 
		PluginsTests/CProlineParserTest.cpp
		PluginsTests/CProlineParserTest.h 
		PluginsTests/CKomputronikPluginTest.cpp
		PluginsTests/CKomputronikPluginTest.h 
		PluginsTests/CKomputronikParserTest.cpp
		PluginsTests/CKomputronikParserTest.h
)


SET ( SOURCES_MAIN
      main.cpp
    )

SET ( SOURCES_TEST_MOC 
	CComputerConfModelTest.h
	CCustomHeadersTest.h 
    PluginsTests/CArestPluginTest.h 
	PluginsTests/CArestParserTest.h
	PluginsTests/CProlineParserTest.h 
	PluginsTests/CProlinePluginTest.h 
	PluginsTests/CKomputronikParserTest.h
	PluginsTests/CKomputronikPluginTest.h 
    PluginsTests/ShopPluginNetworkTest.h
    )

QT4_WRAP_CPP(SOURCES_MOC ${SOURCES_TEST_MOC})

IF (WIN32)
	SOURCE_GROUP(moc FILES ${SOURCES_MOC})
	SOURCE_GROUP(TestCases FILES ${SOURCES_TEST_DATA})
	SOURCE_GROUP(TestCases\\Plugins FILES ${SOURCES_TEST_PLUGINS_DATA})
endif(WIN32)
    
add_executable(UnitTest ${SOURCES_MAIN} ${SOURCES_MOC} 
					${SOURCES_TEST_DATA} ${SOURCES_TEST_PLUGINS_DATA}
)
target_link_libraries(UnitTest ${QT_LIBRARIES} ComputerConfShop_lib ProlineShopPlugin_TestLib 
					KomputronikShopPlugin_TestLib ArestShopPlugin_TestLib tidy_lib)
add_dependencies(UnitTest ComputerConfShop_lib 
    ShopPluginInterface ArestShopPlugin ProlineShopPlugin KomputronikShopPlugin 
	ArestShopPlugin_TestLib KomputronikShopPlugin_TestLib ProlineShopPlugin_TestLib )

	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Komputronik_QNAP TS-212_Refresh.html"
		 "${UnitTest_BINARY_DIR}/TestPages/page_Komputronik_QNAP TS-212_Refresh.html")
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Komputronik_AMD Phenom II 965_SinglePage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Komputronik_AMD Phenom II 965_SinglePage.html" )
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Komputronik_Wieloryb_NotFoundPage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Komputronik_Wieloryb_NotFoundPage.html" )
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Komputronik_Corsair DDR3_SingleSearchPage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Komputronik_Corsair DDR3_SingleSearchPage.html" )

	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Proline_AMD Phenom II 965 BOX_SinglePage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Proline_AMD Phenom II 965 BOX_SinglePage.html" )
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Proline_Gigabyte 1155 GA-Z77_SingleSearchPage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Proline_Gigabyte 1155 GA-Z77_SingleSearchPage.html" )
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Proline_Wieloryb_NotFoundPage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Proline_Wieloryb_NotFoundPage.html" )
 
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Arest_AMD PHENOM II X4 965_SinglePage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Arest_AMD PHENOM II X4 965_SinglePage.html" )
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Arest_AMD PHENOM X4_SingleSearchPage.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Arest_AMD PHENOM X4_SingleSearchPage.html" )
	add_custom_command(TARGET UnitTest POST_BUILD 
		 COMMAND ${CMAKE_COMMAND} -E copy "${UnitTest_SOURCE_DIR}/TestPages/page_Arest_Wieloryb_NotFound.html" 
		 "${UnitTest_BINARY_DIR}/TestPages/page_Arest_Wieloryb_NotFound.html" )

add_test(UnitTest UnitTest)

