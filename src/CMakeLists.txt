set(sources
  token.cpp
  tokenizer.cpp
)

set(headers
  token.h
  tokenizer.h
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_library(yet-another-json-parser STATIC ${sources} ${headers})
