function(athr_add_test name)
    add_executable(${name} ${name}.c)
    target_link_libraries(${name} PRIVATE ATHR::athr)
    add_test(NAME ${name} COMMAND ${name} -E environment)

    # Ugly hacky to make the tests work on Windows.
    file(TO_CMAKE_PATH "$ENV{PATH}" PATH)
    list(APPEND PATH $<TARGET_FILE_DIR:ATHR::athr>)
    string(REPLACE ";" "\\;" PATH "${PATH}")
    set_tests_properties(${name} PROPERTIES ENVIRONMENT "PATH=${PATH}")
endfunction()

athr_add_test(example1)
athr_add_test(example2)
athr_add_test(example3)
athr_add_test(example4)
athr_add_test(deprecated)
