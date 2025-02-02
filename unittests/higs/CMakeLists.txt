file(GLOB_RECURSE higs_tests_sources *.cpp)

add_executable(higs_tests ${higs_tests_sources})
target_link_libraries(higs_tests higs GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(higs_tests)