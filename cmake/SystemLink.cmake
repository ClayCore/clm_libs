function(clm_target_include_system_dirs target)
    set(multi_value_args INTERFACE PUBLIC PRIVATE)
    cmake_parse_arguments(
        arg
        ""
        ""
        "${multi_value_args}"
        ${ARGN}
    )

    foreach(scope IN ITEMS INTERFACE PUBLIC PRIVATE)
        foreach(lib IN LISTS arg_${scope})
            if(NOT MSVC)
                set(_SYSTEM SYSTEM)
            endif()

            if(${scope} STREQUAL "INTERFACE" OR ${scope} STREQUAL "PUBLIC")
                target_include_directories(
                    ${target}
                    ${_SYSTEM}
                    ${scope}
                    "$<BUILD_INTERFACE:${lib}>"
                    "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
                )
            else()
                target_include_directories(
                    ${target}
                    ${_SYSTEM}
                    ${scope}
                    ${lib}
                )
            endif()
        endforeach()
    endforeach()
endfunction()

function(clm_target_include_sys_lib target scope lib)
    if(TARGET ${lib})
        get_target_property(lib_include_dirs ${lib} INTERFACE_INCLUDE_DIRECTORIES)

        if(lib_include_dirs)
            clm_target_include_system_dirs(${target} ${scope} ${lib_include_dirs})
        else()
            message(WARNING "!! ${lib} library does not have INTERFACE_INCLUDE_DIRECTORIES property")
        endif()
    endif()
endfunction()

function(clm_target_link_sys_lib target scope lib)
    clm_target_include_sys_lib(${target} ${scope} ${lib})
    target_link_libraries(${target} ${scope} ${lib})
endfunction()

function(clm_target_link_sys_libs target)
    set(multi_value_args INTERFACE PUBLIC PRIVATE)
    cmake_parse_arguments(
        arg
        ""
        ""
        "${multi_value_args}"
        ${ARGN}
    )

    foreach(scope IN ITEMS INTERFACE PUBLIC PRIVATE)
        foreach(lib IN LISTS arg_${scope})
            clm_target_include_sys_lib(${target} ${scope} ${lib})
        endforeach()
    endforeach()
endfunction()
