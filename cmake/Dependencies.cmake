# ----------------------------------------------------------------------------------------------------- #
# `Dependencies.cmake`
# Allows dependency setup and configuration
# ----------------------------------------------------------------------------------------------------- #

include(CPM.cmake)

function(clm_setup_dependencies)
    # if(NOT TARGET fmtlib::fmtlib)
    # cpmaddpackage("gh:fmtlib/fmt#9.10")
    # endif()

    # if(NOT TARGET spdlog::spdlog)
    # cpmaddpackage(
    # NAME
    # spdlog
    # VERSION
    # 1.11.0
    # GITHUB_REPOSITORY
    # "gabime/spdlog"
    # OPTIONS
    # "SPDLOG_FMT_EXTERNAL ON"
    # )
    # endif()
endfunction()
