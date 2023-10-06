function(clm_check_libfuzzer_support result)
    set(
        libfuzzer_test_source
        "
        #include <cstdint>

        extern \"C\" int LLVMFuzzerTestOneInput(const std::uint8_t *data, std::size_t size) {
            return 0;
        }
        "
    )

    include(CheckCXXSourceCompiles)

    set(CMAKE_REQUIRED_FLAGS "-fsanitize=fuzzer")
    set(CMAKE_REQUIRED_LINK_OPTIONS "-fsanitize=fuzzer")

    check_cxx_source_compiles("${libfuzzer_test_source}" ${result})
endfunction()
