add_definitions(-D_CONSOLE -D_AFXDLL)
set(CMAKE_MFC_FLAG 2)
set(CMAKE_RC_FLAGS "${CMAKE_RC_FLAGS} /nologo")

if(CMAKE_CXX_FLAGS MATCHES "/EHs")
    string(REGEX REPLACE "/EHs" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa")
endif()

get_property(BOOST_INCLUDE_DIRS TARGET Boost::boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
get_property(XERCES_INCLUDE_DIRS TARGET XercesC::XercesC PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

# Builds and adds the specified test runner program to the list of unit tests to run.
#
# runner - Name of the test runner source file without the .cpp extension
# reuse  - Name of the first test runner built so that its PCH file can be reused
# ...    - Additional source files required to build the test runner
#
macro(ADD_MEAZURE_TEST runner reuse)
    add_executable(${runner} WIN32 ${runner}.cpp Test.rc ${ARGN})
    if("${reuse}" STREQUAL "")
        target_precompile_headers(${runner} PRIVATE pch.h)
    else()
        target_precompile_headers(${runner} REUSE_FROM ${reuse})
    endif()
    target_include_directories(${runner} PRIVATE
                               ${SRC_DIR}
                               ${BOOST_INCLUDE_DIRS}
                               ${XERCES_INCLUDE_DIRS}
                               "${CMAKE_BINARY_DIR}/src")
    target_link_libraries(${runner} Boost::unit_test_framework XercesC::XercesC "version.lib")
    set_target_properties(${runner} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    add_test(${runner} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${runner})
endmacro()

ADD_MEAZURE_TEST(ColorsTest "" ${APP_DIR}/graphics/Colors.cpp)
ADD_MEAZURE_TEST(CommandLineInfoTest ColorsTest ${APP_DIR}/CommandLineInfo.cpp)
ADD_MEAZURE_TEST(FileProfileTest ColorsTest
                 ${APP_DIR}/profile/FileProfile.cpp
                 ${APP_DIR}/xml/XMLParser.cpp
                 ${APP_DIR}/xml/XMLWriter.cpp
                 ${APP_DIR}/utilities/StringUtils.cpp
                 ${APP_DIR}/utilities/TimeStamp.cpp
                 ${APP_DIR}/VersionInfo.cpp)
ADD_MEAZURE_TEST(GeometryTest ColorsTest)
ADD_MEAZURE_TEST(GUIDTest ColorsTest ${APP_DIR}/utilities/GUID.cpp)
ADD_MEAZURE_TEST(NumericUtilsTest ColorsTest)
ADD_MEAZURE_TEST(PlotterTest ColorsTest)
ADD_MEAZURE_TEST(PositionTest ColorsTest
                 ${APP_DIR}/units/Units.cpp
                 ${APP_DIR}/xml/XMLParser.cpp
                 ${APP_DIR}/xml/XMLWriter.cpp
                 ${APP_DIR}/utilities/GUID.cpp
                 ${APP_DIR}/utilities/StringUtils.cpp
                 ${APP_DIR}/utilities/TimeStamp.cpp
                 ${APP_DIR}/position/PositionDesktop.cpp
                 ${APP_DIR}/position/PositionScreen.cpp
                 ${APP_DIR}/position/Position.cpp)
ADD_MEAZURE_TEST(PositionCollectionTest ColorsTest
                 ${APP_DIR}/units/Units.cpp
                 ${APP_DIR}/xml/XMLParser.cpp
                 ${APP_DIR}/xml/XMLWriter.cpp
                 ${APP_DIR}/utilities/GUID.cpp
                 ${APP_DIR}/utilities/StringUtils.cpp
                 ${APP_DIR}/utilities/TimeStamp.cpp
                 ${APP_DIR}/position/PositionDesktop.cpp
                 ${APP_DIR}/position/PositionScreen.cpp
                 ${APP_DIR}/position/Position.cpp
                 ${APP_DIR}/position/PositionCollection.cpp)
ADD_MEAZURE_TEST(PositionDesktopTest ColorsTest
                 ${APP_DIR}/units/Units.cpp
                 ${APP_DIR}/xml/XMLParser.cpp
                 ${APP_DIR}/utilities/GUID.cpp
                 ${APP_DIR}/utilities/StringUtils.cpp
                 ${APP_DIR}/position/PositionDesktop.cpp
                 ${APP_DIR}/position/PositionScreen.cpp)
ADD_MEAZURE_TEST(PositionScreenTest ColorsTest
                 ${APP_DIR}/units/Units.cpp
                 ${APP_DIR}/xml/XMLParser.cpp
                 ${APP_DIR}/utilities/StringUtils.cpp
                 ${APP_DIR}/position/PositionScreen.cpp)
ADD_MEAZURE_TEST(RegistryProfileTest ColorsTest ${APP_DIR}/profile/RegistryProfile.cpp ${APP_DIR}/VersionInfo.cpp)
ADD_MEAZURE_TEST(SingletonTest ColorsTest)
ADD_MEAZURE_TEST(StringUtilsTest ColorsTest ${APP_DIR}/utilities/StringUtils.cpp)
ADD_MEAZURE_TEST(TimeStampTest ColorsTest ${APP_DIR}/utilities/TimeStamp.cpp)
ADD_MEAZURE_TEST(UnitsTest ColorsTest ${APP_DIR}/units/Units.cpp)
ADD_MEAZURE_TEST(VersionInfoTest ColorsTest ${APP_DIR}/VersionInfo.cpp)
ADD_MEAZURE_TEST(XMLParserTest ColorsTest ${APP_DIR}/xml/XMLParser.cpp)
ADD_MEAZURE_TEST(XMLWriterTest ColorsTest ${APP_DIR}/xml/XMLWriter.cpp)
