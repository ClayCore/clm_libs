# ----------------------------------------------------------------------------------------------------- #
# `ProjectOptions.cmake`
# User configuration for the project
# ----------------------------------------------------------------------------------------------------- #

# ----------------------------------------------------------------------------------------------------- #
# CMAKE MODULES --------------------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #
include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)
include(CMakeDependentOption)

include(SystemLink.cmake)
include(LibFuzzer.cmake)

# ----------------------------------------------------------------------------------------------------- #
# CMAKE MACROS ---------------------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #

# Check if compiler supports sanitizers
macro(clm_supports_sanitizers)
    if(
        (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
        AND NOT WIN32
    )
        set(SUPPORTS_UBSAN ON)
    else()
        set(SUPPORTS_UBSAN OFF)
    endif()

    if(
        (CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*")
        AND WIN32
    )
        set(SUPPORTS_ASAN OFF)
    else()
        set(SUPPORTS_UBSAN ON)
    endif()

    if(
        (CMAKE_C_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_C_COMPILER_ID MATCHES ".*GNU.*")
        AND NOT WIN32
    )
        set(SUPPORTS_UBSAN ON)
    else()
        set(SUPPORTS_UBSAN OFF)
    endif()

    if(
        (CMAKE_C_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_C_COMPILER_ID MATCHES ".*GNU.*")
        AND WIN32
    )
        set(SUPPORTS_ASAN OFF)
    else()
        set(SUPPORTS_UBSAN ON)
    endif()
endmacro()

# Describes and defines all setup options
macro(clm_setup_options)
    # Hardening and coverage
    option(clm_ENABLE_HARDENING "Enable hardening" ON)
    option(clm_ENABLE_COVERAGE "Enable coverage reporting" OFF)

    cmake_dependent_option(
        clm_ENABLE_GLOBAL_HARDENING
        "Attempt to push hardening options to built dependencies"
        ON
        clm_ENABLE_HARDENING
        OFF
    )

    # Check sanitizer support
    clm_supports_sanitizers()

    if(NOT PROJECT_IS_TOP_LEVEL OR clm_PACKAGING_MAINTAINER_MODE)
        option(clm_ENABLE_IPO "Enable IPO/LTO" OFF)
        option(clm_WARNINGS_AS_ERRORS "Treat Warnings as Errors" OFF)
        option(clm_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(clm_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
        option(clm_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(clm_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
        option(clm_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(clm_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(clm_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(clm_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
        option(clm_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
        option(clm_ENABLE_PCH "Enable precompiled headers" OFF)
        option(clm_ENABLE_CACHE "Enable ccache" OFF)
    else()
        option(clm_ENABLE_IPO "Enable IPO/LTO" ON)
        option(clm_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
        option(clm_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
        option(clm_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
        option(clm_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
        option(clm_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
        option(clm_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
        option(clm_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
        option(clm_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
        option(clm_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
        option(clm_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
        option(clm_ENABLE_PCH "Enable precompiled headers" OFF)
        option(clm_ENABLE_CACHE "Enable ccache" ON)
    endif()

    if(NOT PROJECT_IS_TOP_LEVEL)
        mark_as_advanced(
            clm_ENABLE_IPO
            clm_WARNINGS_AS_ERRORS
            clm_ENABLE_USER_LINKER
            clm_ENABLE_SANITIZER_ADDRESS
            clm_ENABLE_SANITIZER_LEAK
            clm_ENABLE_SANITIZER_UNDEFINED
            clm_ENABLE_SANITIZER_THREAD
            clm_ENABLE_SANITIZER_MEMORY
            clm_ENABLE_UNITY_BUILD
            clm_ENABLE_CLANG_TIDY
            clm_ENABLE_CPPCHECK
            clm_ENABLE_COVERAGE
            clm_ENABLE_PCH
            clm_ENABLE_CACHE
        )
    endif()

    # Fuzzer support
    clm_check_libfuzzer_support(LIBFUZZER_SUPPORTED)

    if(
        LIBFUZZER_SUPPORTED AND
        (clm_ENABLE_SANITIZER_ADDRESS OR clm_ENABLE_SANITIZER_THREAD OR clm_ENABLE_SANITIZER_UNDEFINED)
    )
        set(DEFAULT_FUZZER ON)
    else()
        set(DEFAULT_FUZZER OFF)
    endif()

    option(clm_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})
endmacro()

# Describes and defines all global options
macro(clm_global_options)
    # Enable IPO
    if(clm_ENABLE_IPO)
        include(InterproceduralOptimization.cmake)
        clm_enable_ipo()
    endif()

    # Sanitizer support
    clm_supports_sanitizers()

    # Enable hardening
    if(clm_ENABLE_HARDENING AND clm_ENABLE_GLOBAL_HARDENING)
        include(Hardening.cmake)

        if(
            NOT SUPPORTS UBSAN
            OR clm_ENABLE_SANITIZER_ADDRESS
            OR clm_ENABLE_SANITIZER_THREAD
            OR clm_ENABLE_SANITIZER_LEAK
            OR clm_ENABLE_SANITIZER_UNDEFINED
        )
            set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else()
            set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif()

        # message("${clm_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${clm_ENABLE_SANITIZER_UNDEFINED}")
        clm_enable_hardening(clm_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif()
endmacro()

# Describes and defined local options
macro(clm_local_options)
    if(PROJECT_IS_TOP_LEVEL)
        include(StandardProjectSettings.cmake)
    endif()

    add_library(clm_warnings INTERFACE)
    add_library(clm_options INTERFACE)

    include(CompilerWarnings.cmake)
    clm_set_project_warnings(
        clm_warnings
        ""
        ""
        ""
        ""
    )

    if(clm_ENABLE_USER_LINKER)
        include(Linker.cmake)
        clm_configure_linker(clm_options)
    endif()

    include(Sanitizers.cmake)
    clm_enable_sanitizers(
        clm_options
        ${clm_ENABLE_SANITIZER_ADDRESS}
        ${clm_ENABLE_SANITIZER_LEAK}
        ${clm_ENABLE_SANITIZER_UNDEFINED}
        ${clm_ENABLE_SANITIZER_THREAD}
        ${clm_ENABLE_SANITIZER_MEMORY}
    )

    set_target_properties(clm_options PROPERTIES UNITY_BUILD ${clm_ENABLE_UNITY_BUILD})

    if(clm_ENABLE_PCH)
        target_precompile_headers(
            clm_options
            INTERFACE

            # TODO check for language and add headers
        )
    endif()

    if(clm_ENABLE_CACHE)
        include(Cache.cmake)
        clm_enable_cache()
    endif()

    include(StaticAnalyzers.cmake)

    if(clm_ENABLE_CLANG_TIDY)
        clm_enable_clang_tidy(clm_options ${clm_WARNINGS_AS_ERRORS})
    endif()

    if(clm_ENABLE_CPPCHECK)
        clm_enable_cppcheck(${clm_WARNINGS_AS_ERRORS})
    endif()

    if(clm_ENABLE_COVERAGE)
        include(Tests.cmake)
        clm_enable_coverage(clm_options)
    endif()

    if(clm_ENABLE_HARDENING AND NOT clm_ENABLE_GLOBAL_HARDENING)
        include(Hardening.cmake)

        if(
            NOT SUPPORTS_UBSAN
            OR clm_ENABLE_SANITIZER_ADDRESS
            OR clm_ENABLE_SANITIZER_LEAK
            OR clm_ENABLE_SANITIZER_THREAD
            OR clm_ENABLE_SANITIZER_UNDEFINED
        )
            set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
        else()
            set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
        endif()

        clm_enable_hardening(clm_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
    endif()
endmacro()
