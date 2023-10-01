# ----------------------------------------------------------------------------------------------------- #
# `StaticAnalyzers.cmake`
# Allows converting the project into a proper package
# ----------------------------------------------------------------------------------------------------- #

function(clm_package_project)
    cmake_policy(SET CMP0103 NEW)
    set(_options ARCH_INDEPENDENT)
    set(
        _oneValueArgs
        NAME
        COMPONENT
        VERSION
        COMPATIBILITY
        CONFIG_EXPORT_DESTINATION
        CONFIG_INSTALL_DESTINATION
    )
    set(
        _multiValueArgs
        TARGETS
        PUBLIC_INCLUDES
        PUBLIC_DEPENDENCIES_CONFIGURED
        PUBLIC_DEPENDENCIES
        PRIVATE_DEPENDENCIES_CONFIGURED
        PRIVATE_DEPENDENCIES
    )

    cmake_parse_arguments(
        _PackageProject
        "${_options}"
        "${_oneValueArgs}"
        "${_multiValueArgs}"
        "${ARGN}"
    )

    include(GNUInstallDirs)

    if(NOT _PackageProject_TARGETS)
        get_all_installable_targets(_PackageProject_TARGETS)
        message(STATUS "** package_project: considering ${_PackageProject_TARGETS} as the exported targets")
    endif()

    if("${_PackageProject_NAME}" STREQUAL "")
        set(_PackageProject_NAME ${PROJECT_NAME})
    endif()

    set(_PackageProject_NAMESPACE "${_PackageProject_NAME}::")
    set(_PackageProject_VARS_PREFIX ${_PackageProject_NAME})
    set(_PackageProject_EXPORT ${_PackageProject_NAME})

    if("${_PackageProject_VERSION}" STREQUAL "")
        set(_PackageProject_VERSION ${PROJECT_VERSION})
    endif()

    if("${_PackageProject_COMPATIBILITY}" STREQUAL "")
        set(_PackageProject_COMPATIBILITY "SameMajorVersion")
    endif()

    if("${_PackageProject_CONFIG_EXPORT_DESTINATION}" STREQUAL "")
        set(_PackageProject_CONFIG_EXPORT_DESTINATION "${CMAKE_BINARY_DIR}")
    endif()

    set(_PackageProject_EXPORT_DESTINATION "${_PackageProject_CONFIG_EXPORT_DESTINATION}")

    if("${_PackageProject_CONFIG_INSTALL_DESTINATION}" STREQUAL "")
        set(_PackageProject_CONFIG_INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/${_PackageProject_NAME}")
    endif()

    set(_PackageProject_INSTALL_DESTINATION "${_PackageProject_CONFIG_INSTALL_DESTINATION}")

    if(NOT "${_PackageProject_PUBLIC_INCLUDES}" STREQUAL "")
        foreach(_INC ${_PackageProject_PUBLIC_INCLUDES})
            if(NOT IS_ABSOLUTE ${_INC})
                set(_INC "${CMAKE_CURRENT_SOURCE_DIR}/${_INC}")
            endif()

            if(IS_DIRECTORY ${_INC})
                install(DIRECTORY ${_INC} DESTINATION "./")
            else()
                install(FILES ${_INC} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
            endif()
        endforeach()
    endif()

    if(NOT "${_PackageProject_PUBLIC_DEPENDENCIES}" STREQUAL "")
        set(_PUBLIC_DEPENDENCIES_CONFIG)

        foreach(DEP ${_PackageProject_PUBLIC_DEPENDENCIES_CONFIGURED})
            list(APPEND _PUBLIC_DEPENDENCIES_CONFIG "${DEP} CONFIG")
        endforeach()
    endif()

    list(APPEND _PackageProject_PUBLIC_DEPENDENCIES ${_PUBLIC_DEPENDENCIES_CONFIG})
    set(_PackageProject_DEPENDENCIES ${_PackageProject_PUBLIC_DEPENDENCIES})

    if(NOT "${_PackageProject_PRIVATE_DEPENDENCIES_CONFIGURED}" STREQUAL "")
        set(_PRIVATE_DEPENDENCIES_CONFIG)

        foreach(DEP ${_PackageProject_PRIVATE_DEPENDENCIES_CONFIGURED})
            list(APPEND _PRIVATE_DEPENDENCIES_CONFIG "${DEP} CONFIG")
        endforeach()
    endif()

    list(APPEND _PackageProject_PRIVATE_DEPENDENCIES ${_PRIVATE_DEPENDENCIES_CONFIG})

    install(
        TARGETS ${_PackageProject_TARGETS}
        EXPORT ${_PackageProject_EXPORT}
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${_PackageProject_NAME}" COMPONENT dev
    )

    set(_targets_str "")

    foreach(_target ${_PackageProject_TARGETS})
        set(_targets_str "${_targets_str} ${_PackageProject_NAMESPACE}${_target}")
    endforeach()

    set(
        USAGE_FILE_CONTENT
        "
            The package ${_PackageProject_NAME} provides CMake targets:

            find_package(${_PackageProject_NAME} CONFIG REQUIRED)
            target_link_libraries(main PRIVATE ${_targets_str})
        "
    )
    install(CODE "MESSAGE(STATUS \"${USAGE_FILE_CONTENT})")
    file(WRITE "${_PackageProject_EXPORT_DESTINATION}/usage" "${USAGE_FILE_CONTENT}")
    install(FILES "${_PackageProject_EXPORT_DESTINATION}/usage" DESTINATION "${_PackageProject_CONFIG_INSTALL_DESTINATION}")
    unset(_PackageProject_TARGETS)

    FetchContent_Declare(
        _fargs
        URL
        https://github.com/polysquare/cmake-forward-arguments/archive/8c50d1f956172edb34e95efa52a2d5cb1f686ed2.zip
    )
    FetchContent_GetProperties(_fargs)

    if(NOT _fargs_POPULATED)
        FetchContent_Populate(_fargs)
    endif()

    include("${_fargs_SOURCE_DIR}/ForwardArguments.cmake")

    set(_FARGS_LIST)
    cmake_forward_arguments(
        _PackageProject
        _FARGS_LIST
        OPTION_ARGS
        "${_options};"
        SINGLEVAR_ARGS
        "${_oneValueArgs};EXPORT_DESTINATION;INSTALL_DESTINATION;NAMESPACE;VARS_PERFIX;EXPORT"
        MULTIVAR_ARGS
        "${_multiValueArgs};DEPENDENCIES;PRIVATE_DEPENDENCIES"
    )

    FetchContent_Declare(_ycm URL https://github.com/robotology/ycm/archive/refs/tags/v0.13.0.zip)
    FetchContent_GetProperties(_ycm)

    if(NOT _ycm_POPULATED)
        FetchContent_Populate(_ycm)
    endif()

    include("${_ycm_SOURCE_DIR}/modules/InstallBasicPackageFiles.cmake")
    install_basic_package_files(${_PackageProject_NAME} "${_FARGS_LIST}")
    include("${_ycm_SOURCE_DIR}/modules/AddUninstallTarget.cmake")
endfunction()
