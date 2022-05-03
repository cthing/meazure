
find_package(Doxygen)

if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
    add_custom_target(apidocs
                      COMMAND ${DOXYGEN_EXECUTABLE} Meazure.cfg
                      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
elseif(NOT DOXYGEN_FOUND)
    message(WARNING "Doxygen not installed, apidocs target unavailable")
elseif(NOT DOXYGEN_DOT_FOUND)
    message(WARNING "Graphviz not installed, apidocs target unavailable")
endif()
