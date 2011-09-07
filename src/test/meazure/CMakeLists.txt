
add_definitions(-D_CONSOLE -D_AFXDLL)
set(CMAKE_MFC_FLAG 2)

set(APP_DIR "${PROJECT_SOURCE_DIR}/src/meazure")

include_directories(
	"${TEST_DIR}/gtest/include"
	"${APP_DIR}"
)

if(CMAKE_CXX_FLAGS MATCHES "/EHs")
    string(REGEX REPLACE "/EHs" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa")
endif()

macro(add_meazure_test runner)
    add_executable(${runner} WIN32 ${runner}.cpp ${ARGN})
    set_target_properties(${runner} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
    add_test(${runner} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${runner})    
endmacro(add_meazure_test)

add_meazure_test(ColorsTest ${APP_DIR}/Colors.cpp)
add_meazure_test(TimeStampTest ${APP_DIR}/TimeStamp.cpp)
add_meazure_test(UtilsTest ${APP_DIR}/Utils.cpp)
