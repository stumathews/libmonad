
cmake_minimum_required(VERSION 3.27)

project(libmonad VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Header-only library
add_library(monad INTERFACE)

target_include_directories(monad
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:include>
)

# Optional unit tests
option(BUILD_TESTING "Build libmonad tests" ON)

if(BUILD_TESTING)
    enable_testing()

    find_package(GTest CONFIG REQUIRED)

    add_executable(AllTests
        Tests/BindTests.cpp
        Tests/MapTests.cpp
        Tests/EitherTests.cpp
        Tests/OptionTests.cpp
        Tests/Examples.cpp
    )

    target_link_libraries(AllTests PRIVATE
        monad
        GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(AllTests)
endif()

