macro(clm_enable_cppcheck warnings_as_errors CPPCHECK_OPTIONS)
    find_program(cppcheck_binary cppcheck)

    if(cppcheck_binary)
        if(CMAKE_GENERATOR MATCHES ".*Visual Studio.*")
            set(cppcheck_template "vc")
        else()
            set(cppcheck_template "gcc")
        endif()

        if("${CPPCHECK_OPTIONS}" STREQUAL "")
            set(CMAKE_CXX_CPPCHECK
                ${cppcheck_binary}
                --template=${cppcheck_template}
                --enable=style,performance,warning,portability
                --inline-suppr
                --suppress=cppcheckError
                --suppress=internalAstError
                --suppress=unmatchedExpression
                --suppress=passedByValue
                --suppress=syntaxError
                --suppress=preprocessorErrorDirective
                --inconclusive
            )
        else()
            set(CMAKE_CXX_CPPCHECK ${cppcheck_binary} --template=${cppcheck_template} ${CPPCHECK_OPTIONS})
        endif()

        if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
            set(CMAKE_CXX_CPPCHECK ${CMAKE_CXX_CPPCHECK} --std=c++${CMAKE_CXX_STANDARD})
        endif()

        if(${warnings_as_errors})
            list(APPEND CMAKE_CXX_CPPCHECK --error-exitcode=2)
        endif()
    else()
        message(WARNING "!! cppcheck requested but not found")
    endif()
endmacro()

macro(clm_enable_clang_tidy clm_clang_tidy_target warnings_as_errors)
    find_program(clang_tidy_binary clang-tidy)

    if(clang_tidy_binary)
        if(NOT CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*")
            get_target_property(TARGET_PCH ${clm_clang_tidy_target} INTERFACE_PRECOMPILE_HEADERS)

            if("${TARGET_PCH}" STREQUAL "TARGET_PCH-NOTFOUND")
                get_target_property(TARGET_PCH ${clm_clang_tidy_target} PRECOMPILE_HEADERS)
            endif()

            if(NOT("${TARGET_PCH}" STREQUAL "TARGET_PCH-NOTFOUND"))
                message(SEND_ERROR ";; clang-tidy cannot be enabled with non-clang compiler and PCH")
            endif()
        endif()

        set(
            CLANG_TIDY_OPTIONS
            ${clang_tidy_binary}
            -extra-arg=-Wno-unknown-warning-option
            -extra-arg=-Wno-ignored-optimization-argument
            -extra-arg=-Wno-unused-command-line-argument
            -p
        )

        if(NOT "${CMAKE_CXX_STANDARD}" STREQUAL "")
            if($"${CLANG_TIDY_OPTIONS_DRIVER_MODE}" STREQUAL "cl")
                set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=/std:c++${CMAKE_CXX_STANDARD})
            else()
                set(CLANG_TIDY_OPTIONS ${CLANG_TIDY_OPTIONS} -extra-arg=-std=c++${CMAKE_CXX_STANDARD})
            endif()
        endif()

        if(${warnings_as_errors})
            list(APPEND CLANG_TIDY_OPTIONS -warnings-as-errors=*)
        endif()

        set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_OPTIONS})
    else()
        message(WARNING "!! clang-tidy requested but not found")
    endif()
endmacro()

macro(clm_enable_include_what_you_use)
    find_program(include_what_you_use include-what-you-use)

    if(include_what_you_use)
        set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${include_what_you_use})
    else()
        message(WARNING "!! include-what-you-use requested but not found")
    endif()
endmacro()
