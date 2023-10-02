set(
    clm_libc_dirs
    clm_carena
    clm_cstring
    clm_ctest
)

foreach(${clm_libc_lib} IN LISTS ${clm_libc_dirs})
    add_subdirectory(${clm_libc_lib})
endforeach()
