function(clm_set_project_warnings target warnings_as_errors msvc_warnings clang_warnings gcc_warnings cuda_warnings)
    if("${msvc_warnings}" STREQUAL "")
        set(
            msvc_warnings
            /W4
            /w14242
            /w14254
            /w14263
            /w14265
            /w14287
            /we4289
            /w14296
            /w14311
            /w14545
            /w14546
            /w14547
            /w14549
            /w14555
            /w14619
            /w14640
            /w14826
            /w14905
            /w14906
            /w14928
            /permissive-
        )
    endif()

    if("${clang_warnings}" STREQUAL "")
        set(
            clang_warnings
            -Wall
            -Wextra
            -Wshadow
            -Wnon-virtual-dtor
            -Wold-style-cast
            -Wcast-align
            -Wunused
            -Woverloaded-virtual
            -Wpedantic
            -Wconversion
            -Wsign-conversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wformat=2
            -Wimplicit-fallthrough
        )
    endif()

    if("${gcc_warnings}" STREQUAL "")
        set(
            gcc_warnings
            ${clang_warnings}
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wuseless-cast
        )
    endif()

    if("${cuda_warnings}" STREQUAL "")
        set(
            cuda_warnings
            -Wall
            -Wextra
            -Wunused
            -Wconversion
            -Wshadow
        )
    endif()

    if(warnings_as_errors)
        message(STATUS "** Warnings are treated as errors")
        list(APPEND clang_warnings -Werror)
        list(APPEND gcc_warnings -Werror)
        list(APPEND msvc_warnings /WX)
    endif()

    if(MSVC)
        set(clm_project_warnings_cxx ${msvc_warnings})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
        set(clm_project_warnings_cxx ${clang_warnings})
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
        set(clm_project_warnings_cxx ${gcc_warnings})
    else()
        message(WARNING "** No compiler warnings set for '${CMAKE_CXX_COMPILER_ID}'")
    endif()

    set(clm_project_warnings_c "${clm_project_warnings_cxx}")
    set(clm_project_warnings_cuda "${cuda_warnings}")

    target_compile_options(
        ${target}
        INTERFACE
        $<$<COMPILE_LANGUAGE:CXX>:${clm_project_warnings_cxx}>
        $<$<COMPILE_LANGUAGE:C>:${clm_project_warnings_c}>
        $<$<COMPILE_LANGUAGE:CUDA>:${clm_project_warnings_cuda}>
    )
endfunction()
