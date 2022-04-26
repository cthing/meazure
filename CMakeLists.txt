cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project(Meazure)

if(NOT WIN32)
    message(FATAL_ERROR "Meazure only builds on Windows")
endif(NOT WIN32)

enable_testing()

set(BOOST_ROOT "C:/Program Files/boost/boost_1_78_0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost COMPONENTS date_time REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories("${Boost_LIBRARY_DIRS}")

find_package(MFC REQUIRED)
find_package(HTMLHelp REQUIRED)
find_package(PythonInterp REQUIRED)

include(CheckIncludeFile)
check_include_file(WinSDKVer.h HAVE_WINSDKVER_H)
if("${HAVE_WINDSDKVER_H}")
    add_definitions(-DHAVE_WINSDKVER_H)
endif()

set(SUPPORT_DIR "${PROJECT_SOURCE_DIR}/support")
set(TOOLS_DIR "${SUPPORT_DIR}/tools")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/src/test")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /wd4100 /nologo /WX")

if(CMAKE_BUILD_TOOL MATCHES "nmake")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR}/${CMAKE_BUILD_TYPE})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR}/${CMAKE_BUILD_TYPE})

    # Set the compiler warning level
    set(CMAKE_CXX_WARNING_LEVEL 4)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
else(CMAKE_BUILD_TOOL MATCHES "nmake")
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${Meazure_BINARY_DIR})
endif(CMAKE_BUILD_TOOL MATCHES "nmake")

include_directories(
    "${CMAKE_SOURCE_DIR}/src"
)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

add_definitions(-D_MBCS)

add_subdirectory(support)
add_subdirectory(src)
