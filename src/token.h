#pragma once

namespace yajp
{

class Token
{
  public:
    const char* value;

    Token();
    Token(const char* value);
};

}
