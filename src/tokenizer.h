#pragma once
#include "token.h"
#include <string>
#include <vector>
#include <string_view>

namespace yajp
{

class Tokenizer
{
  public:
    explicit Tokenizer(const std::string& input);
    explicit Tokenizer(std::string&& input);

    Token next();
    std::vector<Token> all();

  private:
    std::string _input;
    std::string::iterator _position;

    Token scan_number(const char* first);
    Token scan_string(const char* first);
    Token scan_keyword_true(const char* first);
    Token scan_keyword_false(const char* first);
    Token scan_keyword_null(const char* first);

    static constexpr bool is_whitespace(char c);
    static constexpr bool is_control(char c);
    static constexpr bool is_escape(char c);
    static constexpr bool is_hex(char c);
};

}
