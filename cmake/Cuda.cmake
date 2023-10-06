macro(clm_target_link_cuda target)
    set(one_value_args cuda_warnings)
    cmake_parse_arguments(
        _cuda_args
        ""
        "${one_value_args}"
        ""
        ${ARGN}
    )

    enable_language(CUDA)

    if("${CMAKE_CUDA_STANDARD}" STREQUAL "")
        set(CMAKE_CUDA_STANDARD "${CMAKE_CXX_STANDARD}")
    endif()

    set_target_properties(${target} PROPERTIES POSITION_INDEPENDENT_CODE ON)
    set_target_properties(${target} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

    if(APPLE)
        set_property(TARGET ${target} PROPERTY BUILD_RPATH ${CMAKE_CUDA_IMPLICIT_LINK_DIRECTORIES})
    endif()

    if(WIN32 AND "$ENV{VSCMD_VER}" STREQUAL "")
        message(WARNING "!! Compiling Cuda on Windows outside the Visual Studio Commant prompt or without running `vcvarsall.bat x64` probably fails")
    endif()
endmacro()
