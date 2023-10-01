cmake_minimum_required(VERSION 3.27)

# ----------------------------------------------------------------------------------------------------- #
# PRE-ELIMINARY SETUP --------------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #

# Only set C++ standard, when it's not set by someone else
if(NOT DEFINED CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 20)
endif()

# Only set C standard, when it's not set by someone else
if(NOT DEFINED CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 17)
endif()

# Disable C/C++ compiler extensions of any kind
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

# ----------------------------------------------------------------------------------------------------- #
# PROJECT SETUP --------------------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #
project(
    "clm_libs"
    VERSION 0.1.0
    DESCRIPTION ""
    HOMEPAGE_URL "%%https://github.com/ClayCore/clm_libs%%"
    LANGUAGES CXX C
)

# ----------------------------------------------------------------------------------------------------- #
# CMAKE MODULES --------------------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #
include(cmake/PreventInSourceBuilds.cmake)
include(cmake/ProjectOptions.cmake)

# ----------------------------------------------------------------------------------------------------- #
# PROJECT SETUP AND OPTIONS --------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #
clm_setup_options()
clm_global_options()

include(cmake/Dependencies.cmake)
clm_setup_dependencies()
clm_local_options()

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(GIT_SHA "Unknown" CACHE STRING "SHA this build was generated from")
string(SUBSTRING "${GIT_SHA}" 0 8 GIT_SHORT_SHA)

target_compile_features(clm_options INTERFACE cxx_std_${CMAKE_CXX_STANDARD})
target_compile_features(clm_options INTERFACE c_std_${CMAKE_C_STANDARD})

add_library(clm::clm_options ALIAS clm_options)
add_library(clm::clm_warnings ALIAS clm_warnings)

# ----------------------------------------------------------------------------------------------------- #
# PROJECT CONFIGURATION ------------------------------------------------------------------------------- #
# ----------------------------------------------------------------------------------------------------- #
add_subdirectory(configured_files)

# Add source directory here.
# add_subdirectory(src)
if(NOT PROJECT_IS_TOP_LEVEL)
    return()
endif()

# Add tests
include(CTest)

if(BUILD_TESTING)
    message(AUTHOR_WARNING "** Building tests...")

    # add_subdirectory(test)
endif()

if(clm_BUILD_FUZZ_TESTS)
    message(AUTHOR_WARNING "** Building fuzz tests...")

    if(NOT clm_ENABLE_ADDRESS_SANITIZER AND NOT clm_ENABLE_THREAD_SANITIZER)
        message(WARNING "** You need asan or tsan enabled for meaningful fuzz testing")
    endif()

    # add_subdirectory(fuzz_test)
endif()

# Ensure set debugger environment for msvc and vs targets
if(MSVC)
    get_all_installable_targets(all_targets)
    message("** all_targets=${all_targets}")
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
    "${CMAKE_PROJECT_NAME}-${CMAKE_PROJECT_VERSION}-${GIT_SHORT_SHA}-${CMAKE_SYSTEM_NAME}-${CMAKE_BUILD_TYPE}-${CMAKE_CXX_COMPILER_ID}-${CMAKE_C_COMPILER-ID}-${CMAKE_CXX_COMPILER_VERSION}-${CMAKE_C_COMPILER_VERSION}"
)

include(CPack)
