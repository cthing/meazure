cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(Meazure)

if(NOT WIN32)
    message(FATAL_ERROR "Meazure only builds on Windows")
endif(NOT WIN32)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(WARNING "Meazure may not compile in a 64-bit build environment")
endif()

enable_testing()

set(BOOST_ROOT "$ENV{ProgramFiles}/boost/boost_1_47_0")
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
set(Boost_ADDITIONAL_VERSIONS "1.47" "1.47.0")
find_package(Boost COMPONENTS date_time REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories("${Boost_LIBRARY_DIRS}")

find_package(MFC QUIET)
if(NOT MFC_FOUND)
    set(DDK_SEARCH_PATHS
        "$ENV{DDK_PATH}"
        "$ENV{SystemDrive}/WinDDK"
        "$ENV{ProgramFiles}/WinDDK"
        "$ENV{CommonProgramFiles}/WinDDK")

    set(ATL_GUESSES ".")
    foreach(SEARCH_DIR ${DDK_SEARCH_PATHS})
        if(EXISTS ${SEARCH_DIR})
            file(GLOB DDK_DIRS ${SEARCH_DIR}/*)
            foreach(DDKDIR ${DDK_DIRS})
                file(GLOB CUR ${DDKDIR}/inc/atl*)
                set(ATL_GUESSES ${ATL_GUESSES} ${CUR})

                file(GLOB CUR ${DDKDIR}/lib/atl/i386)
                set(ATLLIB_GUESSES ${ATLLIB_GUESSES} ${CUR})

                file(GLOB CUR ${DDKDIR}/inc/mfc*)
                set(MFC_GUESSES ${MFC_GUESSES} ${CUR})

                file(GLOB CUR ${DDKDIR}/lib/mfc/i386)
                set(MFCLIB_GUESSES ${MFCLIB_GUESSES} ${CUR})
            endforeach()
        endif()
    endforeach()

    find_file(ATLWIN atlwin.h PATHS ${ATL_GUESSES} NO_DEFAULT_PATH)    
    find_file(ATLLIB atls.lib PATHS ${ATLLIB_GUESSES} NO_DEFAULT_PATH)
    find_file(MFCWIN winres.h PATHS ${MFC_GUESSES} NO_DEFAULT_PATH)
    find_file(MFCLIB mfc42.lib PATHS ${MFCLIB_GUESSES} NO_DEFAULT_PATH)

    if(ATLWIN AND ATLLIB AND MFCWIN AND MFCLIB)
        get_filename_component(ATL_INCLUDE_DIR ${ATLWIN} PATH CACHE)
        get_filename_component(ATL_LIBRARY_DIR ${ATLLIB} PATH CACHE)
        get_filename_component(MFC_INCLUDE_DIR ${MFCWIN} PATH CACHE)
        get_filename_component(MFC_LIBRARY_DIR ${MFCLIB} PATH CACHE)
        
        message(STATUS "Found ATL include dir: ${ATL_INCLUDE_DIR}")
        message(STATUS "Found ATL lib dir: ${ATL_LIBRARY_DIR}")
        message(STATUS "Found MFC include dir: ${MFC_INCLUDE_DIR}")
        message(STATUS "Found MFC lib dir: ${MFC_LIBRARY_DIR}")

        include_directories("${MFC_INCLUDE_DIR}" "${ATL_INCLUDE_DIR}")
        link_directories("${MFC_LIBRARY_DIR}" "${ATL_LIBRARY_DIR}")
    else()
        message(FATAL_ERROR "Meazure requires ATL/MFC libs to be installed. Please download the Microsoft DDK and install it in $ENV{SystemDrive}\\WinDDK")
    endif()
endif()

find_package(HTMLHelp REQUIRED)

set(Python_ADDITIONAL_VERSIONS 3.2)
find_package(PythonInterp REQUIRED)

include(CheckIncludeFile)
check_include_file(WinSDKVer.h HAVE_WINSDKVER_H)
if("${HAVE_WINDSDKVER_H}")
    add_definitions(-DHAVE_WINSDKVER_H)
endif()

set(SUPPORT_DIR "${PROJECT_SOURCE_DIR}/support")
set(TOOLS_DIR "${SUPPORT_DIR}/tools")
set(REDIST_DIR "${SUPPORT_DIR}/redist")
set(TEST_DIR "${PROJECT_SOURCE_DIR}/src/test")

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:wchar_t- /wd4100 /nologo")

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
