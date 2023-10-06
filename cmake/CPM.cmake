set(CPM_DOWNLOAD_VERSION 0.38.1)

if(CPM_SOURCE_CACHE)
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
elseif(DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_DOWNLOAD_LOCATION "${CPM_SOURCE_CACHE}/cpm/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
else()
    set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")
endif()

get_filename_component(CPM_DOWNLOAD_LOCATION ${CPM_DOWNLOAD_LOCATION} ABSOLUTE)

function(clm_download_cpm)
    message(STATUS "** Downloading CPM.cmake to ${CPM_DOWNLOAD_LOCATION}")
    file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
endfunction()

if(NOT(EXISTS ${CPM_DOWNLOAD_LOCATION}))
    clm_download_cpm()
else()
    file(READ ${CPM_DOWNLOAD_LOCATION} clm_cpm_download_check)

    if("${clm_cpm_download_check}" STREQUAL "")
        clm_download_cpm()
    endif()

    unset(clm_cpm_download_check)
endif()

include(${CPM_DOWNLOAD_LOCATION})
