cmake_minimum_required(VERSION 3.27)

# Only set `cxx_standard` if not set by anyone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# Only set `c_standard` if not set by anyone else
if(NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 17)
endif()

# Disable extensions for both languages
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_EXTENSIONS OFF)

# Set project name and languages
project(
    clm_template
    VERSION 0.0.1
    DESCRIPTION ""
    HOMEPAGE_URL "%%todo%%"
    LANGUAGES CXX C
)

# Prevent in-source builds
include(cmake/PreventInSourceBuilds.cmake)

# Define all project options
include(cmake/ProjectOptions.cmake)

clm_setup_options()
clm_global_options()

include(cmake/Dependencies.cmake)
clm_setup_dependencies()
clm_local_options()

# Set the visibility presets
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)

# Compute short git-sha
set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

target_compile_features(clm_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_features(clm_options INTERFACE c_std_${CMAKE_C_STANDARD})

add_library(clm::clm_options ALIAS clm_options)
add_library(clm::clm_warnings ALIAS clm_warnings)

add_subdirectory(clm_libc)
add_subdirectory(clm_libcpp)

if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

include(CTest)

if(BUILD_TESTING)
    message(WARNING "!! building tests...")
    add_subdirectory(test)
endif()

if(CLM_BUILD_FUZZ_TESTS)
    message(WARNING "!! building fuzz tests...")

    if(NOT CLM_ENABLE_SANITIZER_ADDRESS AND NOT CLM_ENABLE_SANITIZER_THREAD)
        message(WARNING "!! you need asan or tsan enabled for fuzz testing")
    endif()

    add_subdirectory(fuzz_test)
endif()

if(MSVC)
    clm_get_all_installable_targets(all_targets)
    set_target_properties(${all_targets} PROPERTIES VS_DEBUGGER_ENVIRONMENT "PATH=$(VC_ExecutablePath_x64);%PATH%")
endif()

if(CMAKE_SKIP_INSTALL_RULES)
    return()
endif()

include(cmake/PackageProject.cmake)
clm_package_project(
    TARGETS
    clm_options
    clm_warnings
)

set(
    CPACK_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_CXX_COMPILER_VERSION}"
)

include(CPack)
