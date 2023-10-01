# ----------------------------------------------------------------------------------------------------- #
# `Linker.cmake`
# Configures user provided linker
# ----------------------------------------------------------------------------------------------------- #

macro(clm_configure_linker project_name)
    include(CheckCXXCompilerFlag)
    include(CheckCCompilerFlag)

    set(USER_LINKER_OPTION "lld" CACHE STRING "Linker to be used")
    set(USER_LINKER_OPTION_VALUES "lld" "gold" "bfd" "mold")
    set_property(CACHE USER_LINKER_OPTION PROPERTY STRINGS ${USER_LINKER_OPTION_VALUES})
    list(FIND USER_LINKER_OPTION_VALUES ${USER_LINKER_OPTION} USER_LINKER_OPTION_INDEX)

    if(${USER_LINKER_OPTION_INDEX} EQUAL -1)
        message(
            STATUS
            "** Using custom linker: '${USER_LINKER_OPTION}', supported linkers are ${USER_LINKER_OPTION_VALUES}"
        )
    endif()

    if(NOT clm_ENABLE_USER_LINKER)
        return()
    endif()

    set(LINKER_FLAG "-fuse-ld=${USER_LINKER_OPTION}")
    check_cxx_compiler_flag(${LINKER_FLAG} CXX_SUPPORTS_USER_LINKER)
    check_c_compiler_flag(${LINKER_FLAG} C_SUPPORTS_USER_LINKER)

    if(CXX_SUPPORTS_USER_LINKER)
        target_compile_options(${project_name} INTERFACE ${LINKER_FLAG})
    endif()

    if(C_SUPPORTS_USER_LINKER)
        target_compile_options(${project_name} INTERFACE ${LINKER_FLAG})
    endif()
endmacro()
