include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

macro(clm_enable_hardening clm_hardening_target global ubsan_min_runtime)
    message(STATUS "** Enabling Hardening (Target=${clm_hardening_target})")

    if(MSVC)
        set(new_compile_options "${new_compile_options} /sdl /DYNAMICBASE /guard:cf")
        set(new_link_options "${new_link_options} /NXCOMPAT /CETCOMPAT")
    elseif(CMAKE_CXX_COMPILER_ID MATCHES ".*Clang|GNU.*")
        set(new_cxx_definitions "${new_cxx_definitions} -D_GLIBCXX_ASSERTIONS")
        set(new_compile_options "${new_compile_options} -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3")

        check_cxx_compiler_flag(-fstack-protector-strong stack_protector)

        if(stack_protector)
            set(new_compile_options "${new_compile_options} -fstack-protector-strong")
        endif()

        message(STATUS "** Strong stack protection enabled")

        check_cxx_compiler_flag(-fcf-protection cf_protection)

        if(cf_protection)
            set(new_compile_options "${new_compile_options} -fcf-protection")
        endif()

        message(STATUS "** CF protection enabled")

        check_cxx_compiler_flag(-fstack-clash-protection clash_protection)

        if(clash_protection)
            if(LINUX OR CMAKE_CXX_COMPILER_ID MATCHES "GNU")
                set(new_compile_options "${new_compile_options} -fstack-clash-protection")
            endif()
        endif()

        message(STATUS "** Stack clash protection enabled")
    else()
        message(WARNING "!! Hardening not supported")
    endif()

    if(${ubsan_min_runtime})
        check_cxx_compiler_flag("-fsanitize=undefined -fno-sanitize-recover=undefined -fsanitize-minimal-runtime" min_runtime)

        if(min_runtime)
            set(new_compile_options "${new_compile_options} -fsanitize=undefined -fsanitize-minimal-runtime")
            set(new_link_options "${new_link_options} -fsanitize=undefined -fsanitize-minimal-runtime")

            if(NOT ${global})
                set(new_compile_options "${new_compile_options} -fno-sanitize-recover=undefined")
                set(new_link_options "${new_link_options} -fno-sanitize-recover=undefined")
            endif()

            message(STATUS "** ubsan minimal runtime enabled")
        endif()
    else()
        message(WARNING "!! ubsan minimal runtime not supported")
    endif()

    if(${global})
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${new_compile_options}")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${new_link_options}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${new_cxx_definitions}")
    else()
        target_compile_options(${clm_hardening_target} INTERFACE ${new_compile_options})
        target_link_options(${clm_hardening_target} INTERFACE ${new_link_options})
        target_compile_definitions(${clm_hardening_target} INTERFACE ${new_cxx_definitions})
    endif()
endmacro()
