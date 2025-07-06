	cmake_minimum_required(VERSION 3.27)

project(monad VERSION 1.0)

find_package(GTest REQUIRED)

add_library(monad INTERFACE)

set_target_properties(monad PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(monad INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)

# GoogleTest requires at least C++17

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(GTest CONFIG REQUIRED)

# Make an executable that runs all the tests the the provided files

add_executable(
	AllTests
	Tests/BindTests.cpp
	Tests/MapTests.cpp
	Tests/EitherTests.cpp
	Tests/OptionTests.cpp
	Tests/Examples.cpp
)

# Set the libaries to link to for the AllTests target
target_link_libraries(AllTests PRIVATE monad GTest::gtest_main)

# make your header files visible in IDEs like Visual Studio.
target_sources(monad INTERFACE
        FILE_SET HEADERS FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Option.h
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/Either.h
    )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/Option.h ${CMAKE_CURRENT_SOURCE_DIR}/lib/Either.h DESTINATION include)
