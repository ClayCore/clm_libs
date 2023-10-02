include(GenerateExportHeader)

file(GLOB_RECURSE clm_ctest_sources "src/" "*.c")
add_library(clm_ctest ${clm_ctest_sources})

add_library(clm::clm_ctest ALIAS clm_ctest)

target_link_libraries(clm_ctest PRIVATE clm_options clm_warnings)
target_include_directories(
    clm_ctest
    ${WARNING_GUARD}
    PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
)
target_compile_features(clm_ctest PUBLIC c_std_17)

set_target_properties(
    clm_ctest
    PROPERTIES ${PROJECT_VERSION}
    C_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(clm_ctest EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/include/clm_ctest/carena_export.h)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_ctest PUBLIC clm_ctest_STATIC_DEFINE)
endif()
