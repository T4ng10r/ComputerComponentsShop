cmake_minimum_required(VERSION 2.8)

project(GUnitTest)


include(${QT_USE_FILE})
include_directories(${ShopPlugin_SOURCE_DIR})
include_directories(${Boost_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy)
include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/Plugins)
include_directories("${gmock_SOURCE_DIR}/include"
                    "${gmock_SOURCE_DIR}"
                    "${gtest_SOURCE_DIR}/include"
                    # This directory is needed to build directly from Google
                    # Test sources.
                    "${gtest_SOURCE_DIR}") 

#####

SET ( SOURCES_PRODUCTION_CODE 
	#Product\ code/ShopPluginBase.h 
)

SET ( SOURCES_MOCKS
	Mocks/LoadNetworkObjectMock.h 
	Mocks/ShopPluginBaseInheriterMock.h
)

SET ( SOURCES_TEST
	Tests/ShopPluginBaseTest.cpp
	Tests/ShopPluginBaseTest.h
	)

SET ( SOURCES_MAIN
      main.cpp
    )


if (WIN32)
	SOURCE_GROUP(moc FILES ${SOURCES_MOC})
	SOURCE_GROUP(Mocks FILES ${SOURCES_MOCKS})
	SOURCE_GROUP("Production code" FILES ${SOURCES_PRODUCTION_CODE})
	SOURCE_GROUP(TestCases FILES ${SOURCES_TEST})
endif(WIN32)

add_executable(GUnitTest ${SOURCES_MAIN} ${SOURCES_MOCKS}
					${SOURCES_MOC} 
					${SOURCES_TEST}
					${SOURCES_PRODUCTION_CODE}
)
target_link_libraries(GUnitTest ${QT_LIBRARIES} gmock gtest ShopPluginInterface Tools_Lib)
add_dependencies(GUnitTest gmock gtest ShopPluginInterface Tools_Lib)
#add_test(GUnitTest)
