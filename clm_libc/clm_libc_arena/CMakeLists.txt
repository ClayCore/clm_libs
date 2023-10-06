if(NOT DEFINED CLM_BUILD_LIB_TYPE)
    if(BUILD_SHARED_LIBS)
        set(CLM_BUILD_LIB_TYPE SHARED)
    else()
        set(CLM_BUILD_LIB_TYPE STATIC)
    endif()
endif()

add_library(
    clm_libc_arena
    ${CLM_BUILD_LIB_TYPE}
)
add_library(clm::clm_libc_arena ALIAS clm_libc_arena)

target_sources(
    clm_libc_arena
    src/arena.c
)
target_link_libraries(
    clm_libc_arena
    PRIVATE clm_options clm_warnings
)
target_include_directories(
    clm_libc_arena
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_arena/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_arena/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_arena/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_arena/src>
)
target_compile_features(clm_libc_arena PRIVATE c_std_17)

set_target_properties(
    clm_libc_arena
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)
