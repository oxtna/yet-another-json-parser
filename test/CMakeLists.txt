set(test_sources
  test_hello_world.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

foreach(test_file ${test_sources})

  cmake_path(GET test_file STEM test_name)

  add_executable(${test_name} ${test_file})
  target_link_libraries(${test_name} yet-another-json-parser)

  add_test(NAME ${test_name} COMMAND ${test_name})
  set_tests_properties(${test_name} PROPERTIES ENVIRONMENT "CTEST_OUTPUT_ON_FAILURE=1")

endforeach()
