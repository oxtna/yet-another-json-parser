#pragma once
#include "value.h"
#include "token.h"
#include <string>
#include <stack>
#include <vector>
#include <stdexcept>
#include <cstddef>

namespace yajp
{

class Parser
{
  public:
    Parser() = default;

    Value parse(const std::string& string);
    Value parse(std::string&& string);

    Value parse_lazy(const std::string& string);
    Value parse_lazy(std::string&& string);

  private:
    enum class State
    {
        Initial,
        End,
        Object,
        ObjectKey,
        ObjectColon,
        ObjectValue,
        ObjectComma,
        Array,
        ArrayValue,
        ArrayComma,
        Error,
    };

    // This stack is used to reference nested structures.
    std::stack<Value*, std::vector<Value*>> _depth_stack;
    // This stack is used to parse key value pairs for objects.
    std::stack<Value::String, std::vector<Value::String>> _key_stack;
    Value _global_value;
    State _current_state;

    void reset();
    void consume(const Token& token);
    Value::String strip_string_quotes(const Value::String& string) const;
};

class ParserError : std::runtime_error
{
  public:
    ParserError(const std::string& message) : std::runtime_error(message) {}

    ParserError(const char* message) : std::runtime_error(message) {}
};

}
