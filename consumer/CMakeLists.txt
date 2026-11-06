cmake_minimum_required(VERSION 3.27)

project(consumer LANGUAGES CXX)

find_package(libmonad CONFIG REQUIRED)

add_executable(consumer main.cpp)

target_link_libraries(consumer PRIVATE
    libmonad::monad
)
