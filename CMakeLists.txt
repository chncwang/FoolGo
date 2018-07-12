CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_SUBDIRECTORY(googletest)

PROJECT(FoolGo)

SET(CMAKE_CXX_COMPILER c++)

IF(APPLE)
    SET(BASE_FLAGS ${BASE_FLAGS} -stdlib=libc++)
ENDIF()

SET(BASE_FLAGS "-std=c++0x -Wno-unused-result -iquote src")
SET(CMAKE_BUILD_TYPE "debug")
SET(CMAKE_CXX_FLAGS_DEBUG "${BASE_FLAGS} -O1 -g")  
SET(CMAKE_CXX_FLAGS_RELEASE "${BASE_FLAGS} -O3")
SET(CMAKE_EXE_LINKER_FLAGS["debug"] "-g")

FIND_PACKAGE(boost)
IF(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
ENDIF()

INCLUDE_DIRECTORIES(src googletest/googletest/include spdlog/include)

AUX_SOURCE_DIRECTORY(src/board SRCS)
AUX_SOURCE_DIRECTORY(src/piece_structure SRCS)
AUX_SOURCE_DIRECTORY(src/game SRCS)
AUX_SOURCE_DIRECTORY(src/player SRCS)
AUX_SOURCE_DIRECTORY(src/util SRCS)
ADD_EXECUTABLE(foolgo ${SRCS} src/foolishgo.cc)
IF (APPLE)
    TARGET_LINK_LIBRARIES(foolgo c++)
ENDIF()
TARGET_LINK_LIBRARIES(foolgo pthread)
ADD_EXECUTABLE(lab ${SRCS} src/lab.cc)
TARGET_LINK_LIBRARIES(lab pthread)
ADD_EXECUTABLE(trainer ${SRCS} src/trainer.cc)

# Test configs.
ENABLE_TESTING()

SET(GTEST_MAIN googletest/googletest/src/gtest_main.cc)
SET(TESTS ${SRCS} ${GTEST_MAIN})
AUX_SOURCE_DIRECTORY(test TESTS)
AUX_SOURCE_DIRECTORY(test/board TESTS)
AUX_SOURCE_DIRECTORY(test/game TESTS)
LIST(REMOVE_ITEM TESTS src/foolishgo.cc)
ADD_EXECUTABLE(tests ${TESTS})
SET(TEST_LIB gtest gtest_main pthread)
IF (APPLE)
    SET(TEST_LIB ${TEST_LIB} c++)
ENDIF()
TARGET_LINK_LIBRARIES(tests ${FOOLGO_LIB} ${TEST_LIB})

ADD_TEST(NAME BoardTest COMMAND tests)
ADD_TEST(NAME BoardInGmTest COMMAND tests)
ADD_TEST(NAME MonteCarloGameTest COMMAND tests)
ADD_TEST(NAME FreshGameTest COMMAND tests)
