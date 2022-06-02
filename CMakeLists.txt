cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Meazure
    VERSION 3.0.3.1
    DESCRIPTION "A tool for easily measuring and capturing portions of the screen."
    HOMEPAGE_URL "https://github.com/cthing/meazure"
)

if(NOT WIN32)
    message(FATAL_ERROR "Meazure only builds on Windows")
endif()

set(CMAKE_BUILD_TYPE Release)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(APP_DIR "${SRC_DIR}/meazure")
set(TEST_DIR "${SRC_DIR}/test")
set(SUPPORT_DIR "${PROJECT_SOURCE_DIR}/support")
set(TOOLS_DIR "${SUPPORT_DIR}/tools")

enable_testing()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BOOST_ROOT "C:/Program Files/boost/boost_1_79_0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_NO_WARN_NEW_VERSIONS ON)

find_package(Boost COMPONENTS date_time unit_test_framework REQUIRED)
find_package(MFC REQUIRED)
find_package(HTMLHelp REQUIRED)
find_package(PythonInterp REQUIRED)

find_program(CMAKE_LINT_PROGRAM "cmake-lint")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX /W4")

add_compile_definitions(_MBCS)

if(CMAKE_BUILD_TOOL MATCHES "nmake")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR}/${CMAKE_BUILD_TYPE})
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR})
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_subdirectory(support)
add_subdirectory(src)

if(CMAKE_LINT_PROGRAM)
    file(GLOB_RECURSE BUILD_FILES CMakeLists.txt)
    add_custom_target(cmakelint
                      COMMAND ${CMAKE_LINT_PROGRAM} ${BUILD_FILES} --config ${SUPPORT_DIR}/cmake-lint/config.py
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()
