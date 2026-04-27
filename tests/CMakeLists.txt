include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(GLOB TEST_SOURCES *.cpp)

add_executable(${PROJECT_NAME}Tests ${TEST_SOURCES})
target_link_libraries(${PROJECT_NAME}Tests
    PRIVATE
        ${PROJECT_NAME}Lib
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME}Tests)

if(ENABLE_COVERAGE)
    target_compile_options(${PROJECT_NAME}Tests PRIVATE --coverage)
    target_link_options(${PROJECT_NAME}Tests PRIVATE --coverage)

    find_program(GCOVR_EXECUTABLE gcovr)
    if(GCOVR_EXECUTABLE)
        add_custom_target(coverage
            COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
            COMMAND ${CMAKE_CTEST_COMMAND} --test-dir ${CMAKE_BINARY_DIR} --output-on-failure
            COMMAND ${GCOVR_EXECUTABLE}
                --root ${CMAKE_SOURCE_DIR}
                --filter ${CMAKE_SOURCE_DIR}/src/
                --filter ${CMAKE_SOURCE_DIR}/include/
                --exclude ${CMAKE_SOURCE_DIR}/src/Main.cpp
                --html --html-details
                --output ${CMAKE_BINARY_DIR}/coverage/index.html
                --print-summary
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            DEPENDS ${PROJECT_NAME}Tests
            COMMENT "Running tests and generating gcovr coverage report at build/coverage/index.html"
            VERBATIM
        )
    else()
        message(WARNING "ENABLE_COVERAGE is ON but gcovr was not found in PATH. "
                        "The instrumented binary will still build, but the 'coverage' target "
                        "will not be defined. Install gcovr (e.g. 'dnf install gcovr' or "
                        "'pip install gcovr') and reconfigure.")
    endif()
endif()
