include(GenerateExportHeader)

add_library(clm_libc_vector)
add_library(clm::clm_libc_vector ALIAS clm_libc_vector)

target_sources(
    clm_libc_vector
    PRIVATE
    src/clm_vector.c
)
target_link_libraries(
    clm_libc_vector
    PRIVATE clm_options clm_warnings
    INTERFACE clm_libc_logging clm_libc_arena
)
target_include_directories(
    clm_libc_vector
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_logging/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_arena/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_vector/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_vector/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_arena/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_logging/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_vector/include>
)
target_compile_features(clm_libc_vector PRIVATE c_std_17)

set_target_properties(
    clm_libc_vector
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(
    clm_libc_vector
    EXPORT_FILE_NAME
    ${PROJECT_BINARY_DIR}/clm_libc/clm_libc_vector/include/clm_libc_vector_export.h
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_libc_vector PUBLIC CLM_LIBC_VECTOR_STATIC_DEFINE)
endif()

add_custom_command(
    TARGET clm_libc_vector POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:clm_libc_vector> ${PROJECT_SOURCE_DIR}/target/
)
