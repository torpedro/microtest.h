foreach(name IN ITEMS assertions registration runner header diagnostics duplicates registration_conflicts colors)
  add_executable(microtest_${name} ${name}.cpp)
  target_link_libraries(microtest_${name} PRIVATE microtest::microtest)
  microtest_development_target(microtest_${name})
endforeach()
target_sources(microtest_registration PRIVATE registration_other.cpp)
target_sources(microtest_duplicates PRIVATE duplicates_other.cpp)
add_test(NAME registration_conflicts COMMAND microtest_registration_conflicts)
add_test(NAME duplicates COMMAND "${CMAKE_COMMAND}"
  "-DPROGRAM=$<TARGET_FILE:microtest_duplicates>"
  -P "${CMAKE_CURRENT_SOURCE_DIR}/check_duplicates.cmake")
foreach(mode IN ITEMS auto always never)
  add_test(NAME colors_${mode} COMMAND "${CMAKE_COMMAND}"
    "-DPROGRAM=$<TARGET_FILE:microtest_colors>" "-DMODE=${mode}"
    -P "${CMAKE_CURRENT_SOURCE_DIR}/check_colors.cmake")
endforeach()
add_test(NAME assertions COMMAND microtest_assertions)
add_test(NAME registration COMMAND microtest_registration)
add_test(NAME header COMMAND microtest_header)
add_test(NAME diagnostics COMMAND "${CMAKE_COMMAND}"
  "-DPROGRAM=$<TARGET_FILE:microtest_diagnostics>"
  "-DLOG_FILE=${CMAKE_CURRENT_BINARY_DIR}/diagnostics.log"
  -P "${CMAKE_CURRENT_SOURCE_DIR}/check_diagnostics.cmake")
add_test(NAME runner_success COMMAND "${CMAKE_COMMAND}"
  "-DPROGRAM=$<TARGET_FILE:microtest_runner>" -DEXPECTED_EXIT=0
  "-DEXPECTED_OUTPUT=All tests succeeded!" -P "${CMAKE_CURRENT_SOURCE_DIR}/check_runner.cmake")
add_test(NAME runner_failure COMMAND "${CMAKE_COMMAND}"
  "-DPROGRAM=$<TARGET_FILE:microtest_runner>" -DARGUMENTS=fail -DEXPECTED_EXIT=1
  "-DEXPECTED_OUTPUT=3 tests failed [(]75[.]00%[)]" -P "${CMAKE_CURRENT_SOURCE_DIR}/check_runner.cmake")
configure_file(check_consumer.cmake.in check_consumer.cmake @ONLY)
add_test(NAME installed_consumer COMMAND "${CMAKE_COMMAND}"
  "-DCONFIG=$<CONFIG>" -P "${CMAKE_CURRENT_BINARY_DIR}/check_consumer.cmake")
