set(sources
  token.cpp
)

set(headers
  token.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(yet-another-json-parser STATIC ${sources} ${headers})
