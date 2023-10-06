macro(clm_configure_linker clm_linker_target)
    include(CheckCXXCompilerFlag)

    set(CLM_USER_LINKER_OPTION "lld" CACHE STRING "Linker to be used")
    set(CLM_USER_LINKER_OPTION_VALUES "lld" "gold" "bfg" "mold")
    set_property(CACHE CLM_USER_LINKER_OPTION PROPERTY STRINGS ${CLM_USER_LINKER_OPTION_VALUES})

    list(FIND CLM_USER_LINKER_OPTION_VALUES ${CLM_USER_LINKER_OPTION} CLM_USER_LINKER_OPTION_INDEX)

    if(${CLM_USER_LINKER_OPTION_INDEX} EQUAL -1)
        message(STATUS "** Using custom linker: '${CLM_USER_LINKER_OPTION}', supported entries: ${CLM_USER_LINKER_OPTION_VALUES}")
    endif()

    if(NOT CLM_ENABLE_USER_LINKER)
        return()
    endif()

    set(clm_linker_flag "-fuse-ld=${CLM_USER_LINKER_OPTION}")

    check_cxx_compiler_flag(${clm_linker_flag} clm_supports_user_linker)

    if(clm_supports_user_linker)
        target_compile_options(${clm_linker_target} INTERFACE ${clm_linker_flag})
    endif()
endmacro()
