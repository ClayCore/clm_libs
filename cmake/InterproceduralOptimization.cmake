macro(clm_enable_ipo)
    include(CheckIPOSupported)

    check_ipo_supported(RESULT clm_ipo_result OUTPUT clm_ipo_output)

    if(clm_ipo_result)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    else()
        message(SEND_ERROR ";; IPO is not supported: ${output}")
    endif()
endmacro()
