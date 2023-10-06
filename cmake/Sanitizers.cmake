function(
    clm_enable_sanitizers
    target
    enable_sanitizer_address
    enable_sanitizer_leak
    enable_sanitizer_memory
    enable_sanitizer_thread
    enable_sanitizer_undefined
)
    if(CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
        set(sanitizers "")

        if(${enable_sanitizer_address})
            list(APPEND sanitizers "address")
        endif()

        if(${enable_sanitizer_leak})
            list(APPEND sanitizers "leak")
        endif()

        if(${enable_sanitizer_undefined})
            list(APPEND sanitizers "undefined")
        endif()

        if(${enable_sanitizer_thread})
            if("address" IN_LIST sanitizers OR "leak" IN_LIST sanitizers)
                message(WARNING "!! Thread sanitizer does not work with address or leak sanitizers enabled")
            else()
                list(APPEND sanitizers "thread")
            endif()
        endif()

        if(${enable_sanitizer_memory} AND CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
            message(WARNING "!! MSan requires all code (including libc++) to be MSan-instrumented.")

            if("address" IN_LIST sanitizers OR "thread" IN_LIST sanitizers OR "leak" IN_LIST sanitizers)
                message(WARNING "!! MSan does not work with address, leak or thread sanitizers enabled")
            else()
                list(APPEND sanitizers "memory")
            endif()
        endif()
    elseif(MSVC)
        if(${enable_sanitizer_address})
            list(APPEND sanitizers "address")
        endif()

        if(
            ${enable_sanitizer_leak}
            OR ${enable_sanitizer_memory}
            OR ${enable_sanitizer_thread}
            OR ${enable_sanitizer_undefined}
        )
            message(WARNING "!! MSVC only supports address sanitizer")
        endif()
    endif()

    list(JOIN sanitizers "," sanitizer_list)

    if(sanitizer_list)
        if(NOT "${sanitizer_list}" STREQUAL "")
            if(NOT MSVC)
                target_compile_options(${target} INTERFACE -fsanitize=${sanitizer_list})
                target_link_options(${target} INTERFACE -fsanitize=${sanitizer_list})
            else()
                string(FIND "$ENV${PATH}" "$ENV{VSINSTALLDIR}" index_of_vs_install_dir)

                if("${index_of_vs_install_dir}" STREQUAL "-1")
                    message(SEND_ERROR ";; MSVC Sanitizers require MSVC environment. Execute 'vcvarsall.bat'")
                endif()

                target_compile_options(${target} INTERFACE /fsanitize=${sanitizer_list} /Zi /INCREMENTAL:NO)
                target_compile_definitions(${target} INTERFACE _DISABLE_VECTOR_ANNOTATION _DISABLE_STRING_ANNOTATION)
                target_link_options(${target} INTERFACE /INCREMENTAL:NO)
            endif()
        endif()
    endif()
endfunction()
