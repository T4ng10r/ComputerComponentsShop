cmake_minimum_required(VERSION 2.8)

project(tidy)

  include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include)
  include_directories(${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy)
	include_directories(${CMAKE_CURRENT_SOURCE_DIR})

  SET ( TIDY_SRC
    access.c
    access.h
    alloc.c
    attrask.c
    attrdict.c
    attrdict.h
    attrget.c
    attrs.c
    attrs.h
    buffio.c
    charsets.c
    charsets.h
    clean.c
    clean.h
    CMakeLists.txt
    config.c
    config.h
    entities.c
    entities.h
    fileio.c
    fileio.h
    forward.h
    iconvtc.c
    iconvtc.h
    istack.c
    lexer.c
    lexer.h
    localize.c
    mappedio.c
    mappedio.h
    message.h
    parser.c
    parser.h
    pprint.c
    pprint.h
    streamio.c
    streamio.h
    tagask.c
    tags.c
    tags.h
    tidy-int.h
    tidylib.c
    tmbstr.c
    tmbstr.h
    utf8.c
    utf8.h
    version.h
    win32tc.c
    win32tc.h
	)


  SET ( TIDY_INCLUDE
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy/buffio.h
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy/platform.h
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy/tidy.h
      ${ComputerConfShop_PROJECT_SOURCE_DIR}/include/tidy/tidyenum.h
      )

      add_library(tidy_lib STATIC ${TIDY_SRC} ${TIDY_INCLUDE} )
