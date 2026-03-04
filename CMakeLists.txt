cmake_minimum_required(VERSION 3.25)
project(AdventureLog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)

file(GLOB SOURCES src/*.cpp)

add_executable(AdventureLog ${SOURCES})

target_include_directories(AdventureLog PRIVATE include)
target_compile_options(AdventureLog PRIVATE -Werror)
