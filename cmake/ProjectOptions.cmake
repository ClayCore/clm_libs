include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)
include(CMakeDependentOption)

macro(clm_supports_sanitizers)
    if(
        (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*"
        OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
        AND NOT WIN32
    )
        set(CLM_SUPPORTS_UBSAN ON)
    else()
        set(CLM_SUPPORTS_UBSAN OFF)
    endif()

    if(
        (CMAKE_C_COMPILER MATCHES ".*Clang.*"
        OR CMAKE_C_COMPILER MATCHES ".*GNU.*")
        AND NOT WIN32
    )
        set(CLM_SUPPORTS_UBSAN ON)
    else()
        set(CLM_SUPPORTS_UBSAN OFF)
    endif()

    if(
        (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*"
        OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
        AND WIN32
    )
        set(CLM_SUPPORTS_ASAN OFF)
    else()
        set(CLM_SUPPORTS_ASAN ON)
    endif()

    if(
        (CMAKE_C_COMPILER MATCHES ".*Clang.*"
        OR CMAKE_C_COMPILER MATCHES ".*GNU.*")
        AND WIN32
    )
        set(CLM_SUPPORTS_ASAN OFF)
    else()
        set(CLM_SUPPORTS_ASAN ON)
    endif()
endmacro()

macro(clm_setup_options)
    option(CLM_ENABLE_HARDENING "Enable hardening" ON)
    option(CLM_ENABLE_COVERAGE "Enable coverage reporting" OFF)
    cmake_dependent_option(
        CLM_ENABLE_GLOBAL_HARDENING
        "Attempt to push hardening options to built dependencies"
        ON
        CLM_ENABLE_HARDENING
        OFF
    )

    clm_supports_sanitizers()

    if(NOT PROJECT_IS_TOP_LEVEL OR CLM_PACKAGING_MAINTAINER_MODE)
        option(CLM_ENABLE_IPO "Enable IPO/LTO" OFF)
        option(CLM_WARNINGS_AS_ERRORS "Treat Warnings as Errors" OFF)
        option(CLM_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(CLM_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
        option(CLM_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(CLM_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
        option(CLM_ENABLE_CPPCHECK "Enable cppcheck analysis" OFF)
        option(CLM_ENABLE_PCH "Enable precompiled headers" OFF)
        option(CLM_ENABLE_CACHE "Enable compiler cache" OFF)
    else()
        option(CLM_ENABLE_IPO "Enable IPO/LTO" ON)
        option(CLM_WARNINGS_AS_ERRORS "Treat Warnings as Errors" ON)
        option(CLM_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(CLM_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${CLM_SUPPORTS_ASAN})
        option(CLM_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(CLM_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${CLM_SUPPORTS_UBSAN})
        option(CLM_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(CLM_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
        option(CLM_ENABLE_CPPCHECK "Enable cppcheck analysis" ON)
        option(CLM_ENABLE_PCH "Enable precompiled headers" OFF)
        option(CLM_ENABLE_CACHE "Enable compiler cache" ON)
    endif()

    if(NOT PROJECT_IS_TOP_LEVEL)
        mark_as_advanced(
            CLM_ENABLE_IPO
            CLM_WARNINGS_AS_ERRORS
            CLM_ENABLE_USER_LINKER
            CLM_ENABLE_SANITIZER_ADDRESS
            CLM_ENABLE_SANITIZER_LEAK
            CLM_ENABLE_SANITIZER_MEMORY
            CLM_ENABLE_SANITIZER_THREAD
            CLM_ENABLE_SANITIZER_UNDEFINED
            CLM_ENABLE_UNITY_BUILD
            CLM_ENABLE_CLANG_TIDY
            CLM_ENABLE_CPPCHECK
            CLM_ENABLE_COVERAGE
            CLM_ENABLE_PCH
            CLM_ENABLE_CACHE
        )
    endif()

    clm_check_libfuzzer_support(CLM_LIBFUZZER_SUPPORTED)

    if(
        CLM_LIBFUZZER_SUPPORTED AND
        (CLM_ENABLE_SANITIZER_ADDRESS OR
        CLM_ENABLE_SANITIZER_THREAD OR
        CLM_ENABLE_SANITIZER_UNDEFINED)
    )
        set(CLM_DEFAULT_FUZZER ON)
    else()
        set(CLM_DEFAULT_FUZZER OFF)
    endif()

    option(CLM_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${CLM_DEFAULT_FUZZER})
endmacro()

macro(clm_global_options)
    if(CLM_ENABLE_IPO)
        include(cmake/InterproceduralOptimization.cmake)
        clm_enable_ipo()
    endif()

    clm_supports_sanitizers()

    if(CLM_ENABLE_HARDENING AND CLM_ENABLE_GLOBAL_HARDENING)
        include(cmake/Hardening.cmake)

        if(
            NOT CLM_SUPPORTS_UBSAN
            OR CLM_ENABLE_SANITIZER_ADDRESS
            OR CLM_ENABLE_SANITIZER_LEAK
            OR CLM_ENABLE_SANITIZER_THREAD
            OR CLM_ENABLE_SANITIZER_UNDEFINED
        )
            set(CLM_ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else()
            set(CLM_ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif()

        clm_enable_hardening(clm_options ON ${CLM_ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif()
endmacro()

macro(clm_local_options)
    if(PROJECT_IS_TOP_LEVEL)
        include(cmake/StandardProjectSettings.cmake)
    endif()

    add_library(clm_warnings INTERFACE)
    add_library(clm_options INTERFACE)

    include(cmake/CompilerWarnings.cmake)
    clm_set_project_warnings(
        clm_warnings
        ${CLM_WARNINGS_AS_ERRORS}
        "" "" "" ""
    )

    if(CLM_ENABLE_USER_LINKER)
        include(cmake/Linker.cmake)
        clm_configure_linker(clm_options)
    endif()

    include(cmake/Sanitizers.cmake)
    clm_enable_sanitizers(
        clm_options
        ${CLM_ENABLE_SANITIZER_ADDRESS}
        ${CLM_ENABLE_SANITIZER_LEAK}
        ${CLM_ENABLE_SANITIZER_MEMORY}
        ${CLM_ENABLE_SANITIZER_THREAD}
        ${CLM_ENABLE_SANITIZER_UNDEFINED}
    )

    set_target_properties(clm_options PROPERTIES UNITY_BUILD ${CLM_ENABLE_UNITY_BUILD})

    if(CLM_ENABLE_PCH)
        target_precompile_headers(
            clm_options
            INTERFACE
            $<$<COMPILE_LANGUAGE:CXX>:${CMAKE_SOURCE_DIR}/pch/libcpp.hpp>
            $<$<COMPILE_LANGUAGE:C>:${CMAKE_SOURCE_DIR}/pch/libc.h>
        )
    endif()

    if(CLM_ENABLE_CACHE)
        include(cmake/Cache.cmake)
        clm_enable_cache()
    endif()

    include(cmake/StaticAnalyzers.cmake)

    if(CLM_ENABLE_CLANG_TIDY)
        clm_enable_clang_tidy(clm_options ${CLM_WARNINGS_AS_ERRORS})
    endif()

    if(CLM_ENABLE_CPPCHECK)
        clm_enable_cppcheck(${CLM_WARNINGS_AS_ERRORS} "")
    endif()

    if(CLM_ENABLE_COVERAGE)
        include(cmake/Tests.cmake)
        clm_enable_coverage(clm_options)
    endif()

    if(CLM_ENABLE_HARDENING AND NOT CLM_ENABLE_GLOBAL_HARDENING)
        include(cmake/Hardening.cmake)

        if(
            NOT CLM_SUPPORTS_UBSAN
            OR CLM_ENABLE_SANITIZER_ADDRESS
            OR CLM_ENABLE_SANITIZER_LEAK
            OR CLM_ENABLE_SANITIZER_THREAD
            OR CLM_ENABLE_SANITIZER_UNDEFINED
        )
            set(CLM_ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else()
            set(CLM_ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif()

        clm_enable_hardening(clm_options OFF ${CLM_ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif()
endmacro()
