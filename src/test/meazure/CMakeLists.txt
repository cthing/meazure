
add_definitions(-D_CONSOLE -D_AFXDLL) 
set(CMAKE_MFC_FLAG 2)

if(CMAKE_CXX_FLAGS MATCHES "/EHs")
    string(REGEX REPLACE "/EHs" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa")
endif()

macro(add_meazure_test runner reuse)
    add_executable(${runner} WIN32 ${runner}.cpp Test.rc ${ARGN})
    if("${reuse}" STREQUAL "")
        target_precompile_headers(${runner} PRIVATE pch.h)
    else()
        target_precompile_headers(${runner} REUSE_FROM ${reuse})
    endif()
    target_include_directories(${runner} PRIVATE ${SRC_DIR} ${Boost_INCLUDE_DIRS} "${CMAKE_BINARY_DIR}/src")
    target_link_libraries(${runner} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} "version.lib" libexpat)
    set_target_properties(${runner} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    add_test(${runner} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${runner})    
endmacro()

add_meazure_test(ColorsTest "" ${APP_DIR}/graphics/Colors.cpp)
add_meazure_test(CommandLineInfoTest ColorsTest ${APP_DIR}/CommandLineInfo.cpp)
add_meazure_test(GeometryTest ColorsTest)
add_meazure_test(GUIDTest ColorsTest ${APP_DIR}/utilities/GUID.cpp)
add_meazure_test(NumericUtilsTest ColorsTest)
add_meazure_test(PlotterTest ColorsTest)
add_meazure_test(RegistryProfileTest ColorsTest ${APP_DIR}/profile/RegistryProfile.cpp ${APP_DIR}/VersionInfo.cpp)
add_meazure_test(SingletonTest ColorsTest)
add_meazure_test(StringUtilsTest ColorsTest ${APP_DIR}/utilities/StringUtils.cpp)
add_meazure_test(TimeStampTest ColorsTest ${APP_DIR}/utilities/TimeStamp.cpp)
add_meazure_test(UnitsTest ColorsTest ${APP_DIR}/units/Units.cpp)
add_meazure_test(VersionInfoTest ColorsTest ${APP_DIR}/VersionInfo.cpp)

add_meazure_test(FileProfileTest ColorsTest
                 ${APP_DIR}/profile/FileProfile.cpp
                 ${APP_DIR}/utilities/XMLParser.cpp
                 ${APP_DIR}/utilities/exval.cpp
                 ${APP_DIR}/utilities/TimeStamp.cpp
                 ${APP_DIR}/VersionInfo.cpp)
target_link_libraries(FileProfileTest libexpat)