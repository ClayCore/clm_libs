include(GenerateExportHeader)

add_library(
    clm_libcpp_math
    INTERFACE
)
add_library(clm::clm_libcpp_math ALIAS clm_libcpp_math)

target_sources(
    clm_libcpp_math
    INTERFACE
    include/math.hpp
)
target_link_libraries(
    clm_libcpp_math
    INTERFACE clm_options clm_warnings
)
target_include_directories(
    clm_libcpp_math
    ${WARNING_GUARD}
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_math/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libcpp/clm_libcpp_shared/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/clm_libcpp/clm_libcpp_math/include>
)
target_compile_features(clm_libcpp_math INTERFACE cxx_std_20)

set_target_properties(
    clm_libcpp_math
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

generate_export_header(
    clm_libcpp_math
    EXPORT_FILE_NAME
    ${PROJECT_BINARY_DIR}/clm_libcpp/clm_libcpp_math/include/clm_libcpp_math_export.hpp
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_libcpp_math INTERFACE CLM_LIBCPP_MATH_STATIC)
endif()
