if(NOT DEFINED CLM_BUILD_LIB_TYPE)
    if(BUILD_SHARED_LIBS)
        set(CLM_BUILD_LIB_TYPE SHARED)
    else()
        set(CLM_BUILD_LIB_TYPE STATIC)
    endif()
endif()

add_library(
    clm_libc_string
    ${CLM_BUILD_LIB_TYPE}
)
add_library(clm::clm_libc_string ALIAS clm_libc_string)

target_sources(
    clm_libc_string
    PRIVATE
    src/clm_string.c
)
target_link_libraries(
    clm_libc_string
    PRIVATE clm_options clm_warnings
)
target_include_directories(
    clm_libc_string
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_string/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_string/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_string/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_string/src>
)
target_compile_features(clm_libc_string PRIVATE c_std_17)

set_target_properties(
    clm_libc_string
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)
