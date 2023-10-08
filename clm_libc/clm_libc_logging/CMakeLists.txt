include(GenerateExportHeader)

add_library(clm_libc_logging)
add_library(clm::clm_libc_logging ALIAS clm_libc_logging)

target_sources(
    clm_libc_logging
    PRIVATE
    src/clm_logging.c
)
target_link_libraries(
    clm_libc_logging
    PRIVATE clm_options clm_warnings
)
target_include_directories(
    clm_libc_logging
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_logging/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_logging/src>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libc/clm_libc_logging/include>
)
target_compile_features(clm_libc_logging PRIVATE c_std_17)

set_target_properties(
    clm_libc_logging
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(
    clm_libc_logging
    EXPORT_FILE_NAME
    ${PROJECT_BINARY_DIR}/clm_libc/clm_libc_logging/include/clm_libc_logging_export.h
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_libc_logging PUBLIC CLM_LIBC_LOGGING_STATIC_DEFINE)
endif()

add_custom_command(
    TARGET clm_libc_logging POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:clm_libc_logging> ${PROJECT_SOURCE_DIR}/target/
)
