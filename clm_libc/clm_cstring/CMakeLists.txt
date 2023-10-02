include(GenerateExportHeader)

file(GLOB_RECURSE clm_cstring_sources "src/" "*.c")
add_library(clm_cstring ${clm_cstring_sources})
add_library(clm::clm_cstring ALIAS clm_cstring)

target_link_libraries(clm_cstring PRIVATE clm_options clm_warnings)
target_include_directories(
    clm_cstring
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)
target_compile_features(clm_cstring PUBLIC c_std_17)

set_target_properties(
    clm_cstring
    PROPERTIES ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(clm_cstring EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/clm_cstring/carena_export.h)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_cstring PUBLIC clm_cstring_STATIC_DEFINE)
endif()
