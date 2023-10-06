function(assure_out_of_bounds_builds)
    get_filename_component(srcdir "${CMAKE_SOURCE_DIR}" REALPATH)
    get_filename_component(bindir "${CMAKE_BINARY_DIR}" REALPATH)

    if("${srcdir}" STREQUAL "${bindir}")
        message(WARNING "* in-source builds are disabled. Please create a separate build directory")
    endif()
endfunction()

assure_out_of_bounds_builds()
