# C Thing Software wrapper around the conan.cmake functionality.

list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

include(${PROJECT_SOURCE_DIR}/support/conan/conan.cmake)

get_property(IS_MULTI_CONFIG GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)

if(IS_MULTI_CONFIG)
    set(CTHING_CONAN_GENERATOR cmake_find_package_multi)
    set(CTHING_FIND_PACKAGE_OPT CONFIG)
else()
    set(CTHING_CONAN_GENERATOR cmake_find_package)
    set(CTHING_FIND_PACKAGE_OPT "")
endif()

macro(CTHING_CONAN_INSTALL)
    if(IS_MULTI_CONFIG)
        foreach(type ${CMAKE_CONFIGURATION_TYPES})
            conan_cmake_autodetect(settings BUILD_TYPE ${type})
            conan_cmake_install(PATH_OR_REFERENCE . BUILD missing REMOTE conancenter SETTINGS ${settings})
        endforeach()
    else()
        conan_cmake_autodetect(settings)
        conan_cmake_install(PATH_OR_REFERENCE . BUILD missing REMOTE conancenter SETTINGS ${settings})
    endif()
endmacro()
