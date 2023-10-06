function(clm_package_project)
    include(FetchContent)

    cmake_policy(SET CMP0103 NEW)

    set(options ARCH_INDEPENDENT)
    set(
        one_value_args
        NAME
        COMPONENT
        VERSION
        COMPATIBILITY
        CONFIG_EXPORT_DESTINATION
        CONFIG_INSTALL_DESTINATION
    )
    set(
        multi_value_args
        TARGETS
        PUBLIC_INCLUDES
        PUBLIC_DEPENDENCIES_CONFIGURED
        PUBLIC_DEPENDENCIES
        PRIVATE_DEPENDENCIES_CONFIGURED
        PRIVATE_DEPENDENCIES
    )

    cmake_parse_arguments(
        _PackageProject
        "${options}"
        "${one_value_args}"
        "${multi_value_args}"
        "${ARGN}"
    )

    include(GNUInstallDirs)

    if(NOT _PackageProject_TARGETS)
        clm_get_all_installable_targets(_PackageProject_TARGETS)
        message(STATUS "** PackageProject, using ${_PackageProject_TARGETS} as exported targets")
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
        foreach(inc ${_PackageProject_PUBLIC_INCLUDES})
            if(NOT IS_ABSOLUTE ${inc})
                set(inc "${CMAKE_CURRENT_SOURCE_DIR}/${inc}")
            endif()

            if(IS_DIRECTORY ${inc})
                install(DIRECTORY ${inc} DESTINATION "./")
            else()
                install(FILES ${inc} DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
            endif()
        endforeach()
    endif()

    if(NOT "${_PackageProject_PUBLIC_DEPENDENCIES_CONFIGURED}" STREQUAL "")
        set(_public_dependencies_config)

        foreach(dep ${_PackageProject_PUBLIC_DEPENDENCIES_CONIGURED})
            list(APPEND _public_dependencies_config "${DEP} CONFIG")
        endforeach()
    endif()

    list(APPEND _PackageProject_DEPENDENCIES ${_PackageProject_PUBLIC_DEPENDENCIES})
    set(_PackageProject_DEPENDENCIES ${_PackageProject_PUBLIC_DEPENDENCIES})

    if(NOT "${_PackageProject_PRIVATE_DEPENDENCIES_CONFIGURED}" STREQUAL "")
        set(_private_dependencies_config)

        foreach(dep ${_PackageProject_PRIVATE_DEPENDENCIES_CONIGURED})
            list(APPEND _private_dependencies_config "${DEP} CONFIG")
        endforeach()
    endif()

    list(APPEND _PackageProject_DEPENDENCIES ${_PackageProject_PRIVATE_DEPENDENCIES})

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
        The package ${_PackageProject_NAME} provides the following CMake targets:

        find_package(${_PackageProject_NAME} CONFIG REQUIRED)
        target_link_libraries(main PRIVATE ${_targets_str})
        "
    )
    install(CODE "MESSAGE(STATUS \"${USAGE_FILE_CONTENT}\")")
    file(WRITE "${_PackageProject_EXPORT_DESTINATION}/usage" "${USAGE_FILE_CONTENT}")
    install(FILES "${_PackageProject_EXPORT_DESTINATION}/usage" DESTINATION "${_PackageProject_CONFIG_INSTALL_DESTINATION}")
    unset(_PackageProject_TARGETS)

    FetchContent_Declare(
        _fargs
        URL https://github.com/polysquare/cmake-forward-arguments/archive/8c50d1f956172edb34e95efa52a2d5cb1f686ed2.zip
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
        "${options};"
        SINGLEVAR_ARGS
        "${one_value_args};EXPORT_DESTINATION;INSTALL_DESTINATION;NAMESPACE;VARS_PREFIX;EXPORT"
        MULTIVAR_ARGS
        "${multi_value_args};DEPENDENCIES;PRIVATE_DEPENDENCIES"
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
