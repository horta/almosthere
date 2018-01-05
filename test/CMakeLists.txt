include_directories(${TEST_SOURCE_DIR}/src)
limix_add_test(demo_slow ${PROJECT_NAME} demo_slow.c)
limix_add_test(demo_fast ${PROJECT_NAME} demo_fast.c)
