add_library(
    clm_libcpp_alloc
    INTERFACE
    include/alloc.hpp
)
add_library(clm::clm_libcpp_alloc ALIAS clm_libcpp_alloc)

target_link_libraries(
    clm_libcpp_alloc
    INTERFACE clm_options clm_warnings
)
target_include_directories(
    clm_libcpp_alloc
    ${WARNING_GUARD}
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_meta/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_traits/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_types/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_alloc/include>
)
target_compile_features(clm_libcpp_alloc INTERFACE cxx_std_20)

set_target_properties(
    clm_libcpp_alloc
    PROPERTIES
    VERSION ${PROJECT_VERSION}

    # VISIBILITY_INLINES_HIDDEN YES
)

# Horrible hack for integration with `compile_commands.json`
# this is necessary because the aforementioned json file
# gives precisely no shits about headers
add_executable(
    clm_libcpp_alloc_dummy
    src/dummy.cpp
)

target_link_libraries(
    clm_libcpp_alloc_dummy
    PRIVATE
    clm_options clm_warnings clm_libcpp_alloc
)
target_include_directories(
    clm_libcpp_alloc_dummy
    ${WARNING_GUARD}
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_meta/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_traits/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_types/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_alloc/include>
)
target_compile_features(clm_libcpp_alloc_dummy INTERFACE cxx_std_20)

set_target_properties(
    clm_libcpp_alloc_dummy
    PROPERTIES
    VERSION ${PROJECT_VERSION}

    # VISIBILITY_INLINES_HIDDEN YES
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_libcpp_alloc INTERFACE CLM_LIBCPP_ALLOC_STATIC_DEFINE)
    target_compile_definitions(clm_libcpp_alloc_dummy PRIVATE CLM_LIBCPP_ALLOC_STATIC_DEFINE)
endif()
