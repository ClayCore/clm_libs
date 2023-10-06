function(clm_enable_cache)
    set(CLM_CACHE_OPTION "ccache" CACHE STRING "Compiler cache to be used")
    set(CLM_CACHE_OPTION_VALUES "ccache" "sccache")
    set_property(CACHE CLM_CACHE_OPTION PROPERTY STRINGS ${CLM_CACHE_OPTION_VALUES})

    list(FIND CLM_CACHE_OPTION_VALUES ${CLM_CACHE_OPTION} CLM_CACHE_OPTION_INDEX)

    if(${CLM_CACHE_OPTION_INDEX} EQUAL -1)
        message(STATUS "** Using custom compiler cache: '${CLM_CACHE_OPTION}', supported entries: ${CLM_CACHE_OPTION_VALUES}")
    endif()

    find_program(clm_cache_binary NAMES ${CLM_CACHE_OPTION_VALUES})

    if(clm_cache_binary)
        message(STATUS "** ${clm_cache_binary} found and enabled")
        set(CMAKE_CXX_COMPILER_LAUNCHER ${clm_cache_binary} CACHE FILEPATH "CXX compiler cache used")
        set(CMAKE_C_COMPILER_LAUNCHER ${clm_cache_binary} CACHE FILEPATH "C compiler cache used")
    else()
        message(WARNING "!! ${CLM_CACHE_OPTION} is enabled but was not found.")
    endif()
endfunction()
