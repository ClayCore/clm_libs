include(GenerateExportHeader)

add_library(clm_libc_string)
add_library(clm::clm_libc_string ALIAS clm_libc_string)

target_sources(
    clm_libc_string
    PRIVATE
    src/clm_string.c
)
target_link_libraries(
    clm_libc_string
    PRIVATE clm_options clm_warnings
    INTERFACE clm_libc_logging clm_libc_arena
)
target_include_directories(
    clm_libc_string
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_logging/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_arena/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_string/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_string/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_arena/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_logging/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_string/include>
)
target_compile_features(clm_libc_string PRIVATE c_std_17)

set_target_properties(
    clm_libc_string
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(
    clm_libc_string
    EXPORT_FILE_NAME
    ${PROJECT_BINARY_DIR}/clm_libc/clm_libc_string/include/clm_libc_string_export.h
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_libc_string PUBLIC CLM_LIBC_STRING_STATIC_DEFINE)
endif()

add_custom_command(
    TARGET clm_libc_string POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:clm_libc_string> ${PROJECT_SOURCE_DIR}/target/
)
