cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_COMPILER /bin/g++)
project(log LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SOURCES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

target_include_directories(${PROJECT_NAME} PRIVATE include)
#target_compile_options(AdventureLog PRIVATE -Werror -Wall -Wpedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror )
