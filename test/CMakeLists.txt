macro(inject_dll_to_please_windows test_name)
  file(TO_CMAKE_PATH "$ENV{PATH}" PATH)
  list(APPEND PATH $<TARGET_FILE_DIR:ELAPSED::elapsed>)
  list(APPEND PATH $<TARGET_FILE_DIR:ATHR::athr>)
  string(REPLACE ";" "\\;" PATH "${PATH}")
  set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "PATH=${PATH}")
endmacro()

function(athr_add_test name)
  add_executable(${name} ${name}.c)
  target_link_libraries(${name} PRIVATE ATHR::athr)
  if(NOT name STREQUAL "deprecated")
    target_compile_options(${name} PRIVATE ${WARNING_FLAGS})
  else()
    if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
      target_compile_options(${name} PRIVATE /wd5105)
    endif()
  endif()
  target_compile_features(${name} PRIVATE c_std_11)
  add_test(NAME ${name} COMMAND ${name} -E environment)
  inject_dll_to_please_windows(${name})
endfunction()

athr_add_test(brake)
athr_add_test(example0)
athr_add_test(example1)
athr_add_test(example2)
athr_add_test(example3)
athr_add_test(example4)
athr_add_test(deprecated)
