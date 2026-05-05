cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_COMPILER /usr/bin/g++)
set(CMAKE_C_COMPILER /usr/bin/gcc)
project(AdventureLog)

set(CMAKE_CXX_STANDARD 26)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_COVERAGE "Build with coverage instrumentation (gcc/clang)" OFF)

# Split sources: the library gets every translation unit except Main.cpp,
# so tests can link the project's code without colliding with main().
file(GLOB LIB_SOURCES src/*.cpp)
list(REMOVE_ITEM LIB_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/Main.cpp")

add_library(${PROJECT_NAME}Lib STATIC ${LIB_SOURCES})
target_include_directories(${PROJECT_NAME}Lib PUBLIC include)
target_compile_options(${PROJECT_NAME}Lib PRIVATE -Werror)
#target_compile_options(${PROJECT_NAME}Lib PRIVATE -Werror -Wall -Wpedantic)

if(ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME}Lib PRIVATE --coverage -O0 -g)
    target_link_options(${PROJECT_NAME}Lib PRIVATE --coverage)
endif()

add_executable(${PROJECT_NAME} src/Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

if(ENABLE_COVERAGE)
    target_link_options(${PROJECT_NAME} PRIVATE --coverage)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

# Unit tests via gtest (FetchContent). Disable with -DBUILD_TESTING=OFF.
include(CTest)
if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
