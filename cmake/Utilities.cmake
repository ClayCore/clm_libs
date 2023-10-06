function(clm_find_substr_by_prefix output prefix input)
    string(FIND "${input}" "${prefix}" prefix_index)

    if("${prefix_index}" STREQUAL "-1")
        message(SEND_ERROR ";; could not find ${prefix} in ${input}")
    endif()

    string(LENGTH "${prefix}" prefix_length)
    math(EXPR start_index "${prefix_index} + ${prefix_length}")
    string(SUBSTRING "${input}" "${start_index}" "-1" _output)

    set("${output}" "${_output}" PARENT_SCOPE)
endfunction()

function(clm_set_env_from_string env_string)
    string(REGEX REPLACE ";" "__sep__" env_string_sep_added "${env_string}")
    string(REGEX REPLACE "\r?\n" ";" env_list "${env_string_sep_added}")

    foreach(env_var ${env_list})
        string(REGEX REPLACE "=" ";" env_parts "${env_var}")
        list(LENGTH env_parts env_parts_length)

        if("${env_parts_length}" EQUAL "2")
            list(GET env_parts 0 env_name)
            list(GET env_parts 1 env_value)

            string(REGEX REPLACE "__sep__" ";" env_value "${env_value}")

            set(ENV{${env_name}} "${env_value}")

            if("${env_name}" EQUAL "PATH")
                list(APPEND CMAKE_PROGRAM_PATH ${env_value})
            endif()
        endif()
    endforeach()
endfunction()

function(clm_get_all_targets result)
    set(targets)

    clm_get_all_targets_recurse(targets ${CMAKE_CURRENT_SOURCE_DIR})

    set(${result} ${targets} PARENT_SCOPE)
endfunction()

function(clm_get_all_installable_targets result)
    set(targets)
    clm_get_all_targets(targets)

    foreach(_target ${targets})
        get_target_property(_target_type ${_target} TYPE)

        if(NOT ${_target_type} MATCHES ".*LIBRARY|EXECUTABLE")
            list(REMOVE_ITEM targets ${_target})
        endif()
    endforeach()

    set(${result} ${targets} PARENT_SCOPE)
endfunction()

macro(clm_get_all_targets_recurse targets dir)
    get_property(subdirs DIRECTORY ${dir} PROPERTY SUBDIRECTORIES)

    foreach(subdir ${subdirs})
        clm_get_all_targets_recurse(${targets} ${subdir})
    endforeach()

    get_property(current_targets DIRECTORY ${dir} PROPERTY BUILDSYSTEM_TARGETS)
    list(APPEND ${targets} ${current_targets})
endmacro()

function(clm_is_verbose result)
    if(
        "CMAKE_MESSAGE_LOG_LEVEL" STREQUAL "VERBOSE"
        OR "CMAKE_MESSAGE_LOG_LEVEL" STREQUAL "DEBUG"
        OR "CMAKE_MESSAGE_LOG_LEVEL" STREQUAL "TRACE"
    )
        set(${result} ON PARENT_SCOPE)
    else()
        set(${result} OFF PARENT_SCOPE)
    endif()
endfunction()
