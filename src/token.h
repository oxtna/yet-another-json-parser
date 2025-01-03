#pragma once
#include <string>
#include <array>

namespace yajp
{

class Token
{
  public:
    enum class Type
    {
        Invalid = 0,
        KeywordTrue = 1,
        KeywordFalse = 2,
        KeywordNull = 3,
        LeftBrace = 4,
        RightBrace = 5,
        LeftBracket = 6,
        RightBracket = 7,
        Comma = 8,
        Colon = 9,
        String = 10,
        Number = 11,
        End = 12,
    };

    Token();
    Token(Type type, const std::string& value);
    Token(Type type, std::string&& value);

    Type type() const;
    std::string value() const;

    std::string to_string() const;

    static constexpr const char* type_to_string(Type type);

  private:
    Type _type;
    std::string _value;

    static constexpr std::array<const char*, 13> _type_str = {
        "Invalid",
        "KeywordTrue",
        "KeywordFalse",
        "KeywordNull",
        "LeftBrace",
        "RightBrace",
        "LeftBracket",
        "RightBracket",
        "Comma",
        "Colon",
        "String",
        "Number",
        "End",
    };
};

}
