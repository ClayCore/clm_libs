add_library(
    clm_libcpp_traits
    INTERFACE
    include/traits.hpp
)
add_library(clm::clm_libcpp_traits ALIAS clm_libcpp_traits)

target_link_libraries(
    clm_libcpp_traits
    INTERFACE clm_options clm_warnings
)
target_include_directories(
    clm_libcpp_traits
    ${WARNING_GUARD}
    INTERFACE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_traits/include>
)
target_compile_features(clm_libcpp_traits INTERFACE cxx_std_20)

set_target_properties(
    clm_libcpp_traits
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

# Horrible hack for integration with `compile_commands.json`
# this is necessary because the aforementioned json file
# gives precisely no shits about headers
add_executable(
    clm_libcpp_traits_dummy
    src/dummy.cpp
)

target_link_libraries(
    clm_libcpp_traits_dummy
    PRIVATE
    clm_options clm_warnings clm_libcpp_traits
)
target_include_directories(
    clm_libcpp_traits_dummy
    ${WARNING_GUARD}
    PRIVATE
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_shared/include>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libcpp/clm_libcpp_traits/include>
)
target_compile_features(clm_libcpp_traits_dummy INTERFACE cxx_std_20)

set_target_properties(
    clm_libcpp_traits_dummy
    PROPERTIES
    VERSION ${PROJECT_VERSION}
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
)

if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(clm_libcpp_traits INTERFACE CLM_LIBCPP_TRAITS_STATIC_DEFINE)
    target_compile_definitions(clm_libcpp_traits_dummy PRIVATE CLM_LIBCPP_TRAITS_STATIC_DEFINE)
endif()
