add_subdirectory(clm_libc_arena)
add_subdirectory(clm_libc_logging)
add_subdirectory(clm_libc_string)
add_subdirectory(clm_libc_vector)

# add_library(
# clm_libc
# )
# add_library(clm::clm_libc ALIAS clm_libc)

# target_link_libraries(
# clm_libc
# PRIVATE clm_options clm_warnings
# clm_libc_logging clm_libc_arena clm_libc_string
# )
# target_include_Directories(
# clm_libc
# ${WARNING_GUARD}
# PUBLIC
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_shared/include>
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_logging/include>
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_logging/src>
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_arena/include>
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_arena/src>
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_string/include>
# $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/clm_libc/clm_libc_string/src>
# )
# target_compile_features(clm_libc PRIVATE c_std_17)

# set_target_properties(
# clm_libc
# PROPERTIES
# VERSION ${PROJECT_VERSION}
# C_VISIBILITY_PRESET hidden
# VISIBILITY_INLINES_HIDDEN YES
# )

# add_custom_command(
# TARGET clm_libc POST_BUILD
# COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:clm_libc> ${PROJECT_SOURCE_DIR}/target/
# )
