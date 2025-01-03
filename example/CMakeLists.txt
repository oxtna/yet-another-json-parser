set(example_sources
  example.cpp
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)

set(example_target "yet-another-json-parser-example")

add_executable(${example_target} ${example_sources})
target_link_libraries(${example_target} yet-another-json-parser)
