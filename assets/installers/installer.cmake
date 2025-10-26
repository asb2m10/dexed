set(DIST_DIR ${CMAKE_BINARY_DIR}/dist)
file(MAKE_DIRECTORY ${DIST_DIR})

add_custom_target(dist)
add_custom_target(installer)

function(package format)
    get_target_property(output_dir ${PROJECT_NAME} RUNTIME_OUTPUT_DIRECTORY)

    if(TARGET ${PROJECT_NAME}_${format})
        add_dependencies(dist ${PROJECT_NAME}_${format})
        add_custom_command(
                TARGET dist
                POST_BUILD
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                COMMAND echo "Installing ${output_dir}/${format} to ${DIST_DIR}"
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${output_dir}/${format}/ ${DIST_DIR}/
        )
    endif()
endfunction()

package(VST3)
package(Standalone)
package(CLAP)
if (APPLE)
    package(AU)
endif()

add_dependencies(installer dist)

if (APPLE)
    set(ARCH_NAME "macOS")
elseif (WIN32)
    set(ARCH_NAME "win")
else()
    set(ARCH_NAME "lnx")
endif()


if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(VERSION_NAME ${PROJECT_VERSION})
else()
    set(VERSION_NAME ${PROJECT_VERSION}-NIGHTLY-${BUILD_ID})
endif()

set(PACKAGE_NAME ${PROJECT_NAME}-${VERSION_NAME}-${ARCH_NAME})

add_custom_command(
        TARGET installer
        POST_BUILD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/LICENSE ${DIST_DIR}/
)

add_custom_command(
        TARGET installer
        POST_BUILD
        WORKING_DIRECTORY ${DIST_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/installer
        COMMAND ${CMAKE_COMMAND} -E tar cvf ${CMAKE_BINARY_DIR}/installer/${PACKAGE_NAME}.zip --format=zip .
        COMMAND ${CMAKE_COMMAND} -E echo "Artifact in: installer/${PACKAGE_NAME}.zip")

if (APPLE)
    message(STATUS "Configuring for mac installer")
    add_custom_command(
            TARGET installer
            POST_BUILD
            USES_TERMINAL
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
            COMMAND ${CMAKE_COMMAND} -E make_directory installer
            COMMAND ${CMAKE_SOURCE_DIR}/assets/installers/make_macos_pkg.sh ${PROJECT_NAME} ${DIST_DIR} ${PROJECT_VERSION} ${CMAKE_BINARY_DIR}/installer ${PACKAGE_NAME}
    )
elseif (WIN32)
    message(STATUS "Configuring for windows installer")
    find_program(INNOSETUP_COMPILER_EXECUTABLE iscc)

    if(
            NOT INNOSETUP_COMPILER_EXECUTABLE
            OR "${INNOSETUP_COMPILER_EXECUTABLE}" MATCHES "NOTFOUND"
            OR NOT EXISTS "${INNOSETUP_COMPILER_EXECUTABLE}"
    )
        message(STATUS "Inno Setup compiler not found")
    else()
        message(
                STATUS
                "Inno Setup compiler found: ${INNOSETUP_COMPILER_EXECUTABLE}"
        )
        add_executable(innosetup::compiler IMPORTED GLOBAL)

        set_target_properties(
                innosetup::compiler
                PROPERTIES
                IMPORTED_LOCATION "${INNOSETUP_COMPILER_EXECUTABLE}"
                INSTALL_SCRIPT "${CMAKE_SOURCE_DIR}/assets/installers/windows/installer.iss"
        )

        add_custom_command(
                TARGET installer
                POST_BUILD
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                COMMAND ${CMAKE_COMMAND} -E make_directory installer
                COMMAND innosetup::compiler
                /O"${CMAKE_BINARY_DIR}/installer" /DName="${PROJECT_NAME}"
                /DNameCondensed="${PROJECT_NAME}" /DVersion="${VERSION_NAME}"
                /DVST3 /DSA
                /DLicense="${CMAKE_SOURCE_DIR}/LICENSE"
                /DStagedAssets="${DIST_DIR}"
                /DData="${CMAKE_SOURCE_DIR}/assets/installers/windows" "$<TARGET_PROPERTY:innosetup::compiler,INSTALL_SCRIPT>"
        )
    endif()
endif()