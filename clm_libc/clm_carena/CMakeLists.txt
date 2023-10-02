include(GenerateExportHeader)

file(GLOB_RECURSE clm_carena_sources "src/" "*.c")
add_library(clm_carena ${clm_carena_sources})
add_library(clm::clm_carena ALIAS clm_carena)

target_link_libraries(clm_carena PRIVATE clm_options clm_warnings)
target_include_directories(
    clm_carena
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)
target_compile_features(clm_carena PUBLIC c_std_17)

set_target_properties(
    clm_carena
    PROPERTIES ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(clm_carena EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/clm_carena/carena_export.h)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_carena PUBLIC CLM_CARENA_STATIC_DEFINE)
endif()
