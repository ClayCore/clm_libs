function(clm_enable_doxygen DOXYGEN_THEME)
    if((NOT DOXYGEN_USE_MDFILE_AS_MAINPAGE) AND EXISTS "${PROJECT_SOURCE_DIR}/README.md")
        set(DOXYGEN_USE_MDFILE_AS_MAINPAGE "${PROJECT_SOURCE_DIR}/README.md")
    endif()

    clm_is_verbose(_is_verbose)

    if(NOT ${_is_verbose})
        set(DOXYGEN_QUIET YES)
    endif()

    set(DOXYGEN_CALLER_GRAPH YES)
    set(DOXYGEN_CALL_GRAPH YES)
    set(DOXYGEN_EXTRACT_ALL YES)
    set(DOXYGEN_GENERATE_TREEVIEW YES)
    set(DOXYGEN_DOT_IMAGE_FORMAT svg)
    set(DOXYGEN_DOT_TRANSPARENT YES)

    if(NOT DOXYGEN_EXCLUDE_PATTERNS)
        set(DOXYGEN_EXCLUDE_PATTERNS "${CMAKE_CURRENT_BINARY_DIR}/vcpkg_installed/*" "${CMAKE_CURRENT_BINARY_DIR}/_deps/*")
    endif()

    if("${DOXYGEN_THEME}" STREQUAL "")
        set(DOXYGEN_THEME "awesome-sidebar")
    endif()

    if("${DOXYGEN_THEME}" STREQUAL "awesome" OR "${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
        FetchContent_Declare(
            _doxygen_theme
            URL https://github.com/jothepro/doxygen-awesome-css/archive/refs/tags/v1.6.1.zip
        )
        FetchContent_MakeAvailable(_doxygen_theme)

        if("${DOXYGEN_THEME}" STREQUAL "awesome" OR "${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
            set(DOXYGEN_HTML_EXTRA_STYLESHEET "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome.css")
        endif()

        if("${DOXYGEN_THEME}" STREQUAL "awesome-sidebar")
            set(
                DOXYGEN_HTML_EXTRA_STYLESHEET
                ${DOXYGEN_HTML_EXTRA_STYLESHEET}
                "${_doxygen_theme_SOURCE_DIR}/doxygen-awesome-sidebar-only.css")
        endif()
    endif()

    find_package(Doxygen REQUIRED OPTIONAL_COMPONENTS dot)
    message(STATUS "** Adding `doxygen-docs` target that builds documentation")
    doxygen_add_docs(
        doxygen-docs
        ALL ${PROJECT_SOURCE_DIR}
        COMMENT "** Generating docs - entry file: ${CMAKE_CURRENT_BINARY_DIR}/html/index.html"
    )
endfunction()
