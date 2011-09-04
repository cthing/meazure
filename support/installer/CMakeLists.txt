
add_custom_target(dist
                  COMMAND iscc Meazure.iss
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                  SOURCES Meazure.iss)
                  
add_dependencies(dist Meazure)

if(MFC_FOUND)
    set(CMAKE_INSTALL_MFC_LIBRARIES TRUE)
    include(InstallRequiredSystemLibraries)
    set(REDIST_FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
else(MFC_FOUND)
    file(GLOB REDIST_FILES "${REDIST_DIR}/msvc*.dll" "${REDIST_DIR}/mfc*.dll" "${REDIST_DIR}/Microsoft.*.manifest")
endif(MFC_FOUND)

set(DEST_REDIST_FILES)
foreach(REDIST_FILE ${REDIST_FILES})
    get_filename_component(REDIST_NAME ${REDIST_FILE} NAME)
    if(CMAKE_BUILD_TOOL MATCHES "nmake")
        set(DEST_REDIST_FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${REDIST_NAME}")
    else()
        set(DEST_REDIST_FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/${REDIST_NAME}")
    endif()
    set(DEST_REDIST_FILES ${DEST_REDIST_FILES} ${DEST_REDIST_FILE})
    add_custom_command(OUTPUT ${DEST_REDIST_FILE}
                       COMMAND ${CMAKE_COMMAND} -E copy ${REDIST_FILE} ${DEST_REDIST_FILE})
endforeach(REDIST_FILE)

add_custom_target(redist DEPENDS ${DEST_REDIST_FILES})
add_dependencies(dist redist)
