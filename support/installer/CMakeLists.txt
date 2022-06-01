set(PACKAGE_BASENAME "meazr${Meazure_VERSION_MAJOR}${Meazure_VERSION_MINOR}${Meazure_VERSION_PATCH}x64")
set(DIST_DIR "${CMAKE_SOURCE_DIR}/dist")
set(PACKAGE_PATHNAME "${DIST_DIR}/${PACKAGE_BASENAME}.exe")
set(PACKAGE_SHA256_PATHNAME "${PACKAGE_PATHNAME}.sha256")

configure_file(Meazure.iss.in Meazure.iss @ONLY)

set(CMAKE_INSTALL_MFC_LIBRARIES TRUE)
include(InstallRequiredSystemLibraries)
set(REDIST_FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})

set(DEST_REDIST_FILES)
foreach(redist_file ${REDIST_FILES})
    cmake_path(GET redist_file FILENAME REDIST_NAME)
    if(CMAKE_BUILD_TOOL MATCHES "nmake")
        set(DEST_REDIST_FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${REDIST_NAME}")
    else()
        set(DEST_REDIST_FILE "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}/${REDIST_NAME}")
    endif()
    set(DEST_REDIST_FILES ${DEST_REDIST_FILES} ${DEST_REDIST_FILE})
    add_custom_command(OUTPUT ${DEST_REDIST_FILE}
                       COMMAND ${CMAKE_COMMAND} -E copy ${REDIST_FILE} ${DEST_REDIST_FILE})
endforeach()

add_custom_target(redist DEPENDS ${DEST_REDIST_FILES})

add_custom_command(OUTPUT ${PACKAGE_PATHNAME}
                   COMMAND iscc /DPackageBasename=${PACKAGE_BASENAME}
                                /DProjectDir=${CMAKE_SOURCE_DIR}
                                /DDistDir="${DIST_DIR}"
                                /DBuildDir="${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
                                /DSupportDir="${SUPPORT_DIR}"
                                "${CMAKE_CURRENT_BINARY_DIR}/Meazure.iss"
                   MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/Meazure.iss"
                   DEPENDS Meazure redist
                   WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(OUTPUT ${PACKAGE_SHA256_PATHNAME}
                   COMMAND ${CMAKE_COMMAND} -E sha256sum ${PACKAGE_PATHNAME} > ${PACKAGE_SHA256_PATHNAME}
                   MAIN_DEPENDENCY ${PACKAGE_PATHNAME})

add_custom_target(dist DEPENDS ${PACKAGE_PATHNAME} ${PACKAGE_SHA256_PATHNAME})
