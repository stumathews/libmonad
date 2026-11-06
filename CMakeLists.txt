
cmake_minimum_required(VERSION 3.27)

project(libmonad VERSION 0.6.1 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Header-only library
add_library(monad INTERFACE)

add_library(libmonad::monad ALIAS monad)

target_compile_features(monad INTERFACE cxx_std_17)

target_include_directories(monad
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Optional unit tests
option(BUILD_TESTING "Build libmonad tests" OFF)

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

# Installation
install(TARGETS monad
    EXPORT libmonadTargets
)

install(DIRECTORY lib/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

# Export targets
install(EXPORT libmonadTargets
    NAMESPACE libmonad::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libmonad
)

# Generate package configuration
configure_package_config_file(
    cmake/libmonadConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/libmonadConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libmonad
)

# Generate package version configuration
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/libmonadConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Install package configuration files
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/libmonadConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/libmonadConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libmonad
)

