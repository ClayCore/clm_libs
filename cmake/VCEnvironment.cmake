include("${CMAKE_CURRENT_LIST_DIR}/Utilities.cmake")

macro(clm_detect_architecture)
    string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" sys_proc_lower)

    if(sys_proc_lower STREQUAL x86 OR sys_proc_lower MATCHES "^i[3456]86$")
        set(vcvarsall_arch x86)
    elseif(sys_proc_lower STREQUAL x64 OR sys_proc_lower STREQUAL x86_64 OR sys_proc_lower STREQUAL amd64)
        set(vcvarsall_arch x64)
    elseif(sys_proc_lower STREQUAL arm)
        set(vcvarsall_arch arm)
    elseif(sys_proc_lower STREQUAL arm64 OR sys_proc_lower STREQUAL aarch64)
        set(vcvarsall_arch arm64)
    else()
        if(CMAKE_HOST_SYSTEM_PROCESSOR)
            set(vcvarsall_arch ${CMAKE_HOST_SYSTEM_PROCESSOR})
        else()
            message(WARNING "!! Unknown processor architecture (${sys_proc_lower}), using x64")
            set(vcvarsall_arch x64)
        endif()
    endif()

    message(STATUS "** Detected architecture: ${sys_proc_lower}")
endmacro()

function(clm_run_vcvarsall)
    if(MSVC AND "$ENV{VSCMD_VER}" STREQUAL "")
        get_filename_component(msvc_dir ${CMAKE_CXX_COMPILER} DIRECTORY)
        find_file(
            vcvarsall_file
            NAMES vcvarsall.bat
            PATHS "${msvc_dir}"
            "${msvc_dir}/.."
            "${msvc_dir}/../.."
            "${msvc_dir}/../../../../../../../.."
            "${msvc_dir}/../../../../../../.."
            PATH_SUFFIXES "VC/Auxiliary/Build" "Common7/Tools" "Tools"
        )

        if(EXISTS ${vcvarsall_file})
            clm_detect_architecture()
            set(vcvarsall_cmd "\"${vcvarsall_file}\" ${vcvarsall_arch}")
            separate_arguments(vcvarsall_cmd NATIVE_COMMAND ${vcvarsall_cmd})

            message(STATUS "** running ${vcvarsall_cmd}")

            # Test to see if the command is properly escaped
            # execute_process(COMMAND cmd.exe /c echo ${vcvarsall_cmd})
            execute_process(
                COMMAND
                "cmd.exe" "/c" ${vcvarsall_cmd}
                "&&" "call" "echo" "VCVARSALL_ENV_START"
                "&" "set"
                OUTPUT_VARIABLE vcvarsall_output
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )

            clm_find_substr_by_prefix(vcvarsall_env "VCVARSALL_ENV_START" "${vcvarsall_output}")
            clm_set_env_from_string("${vcvarsall_env}")
        else()
            message(WARNING "!! could not find 'vcvarsall.bat'")
        endif()
    endif()
endfunction()
