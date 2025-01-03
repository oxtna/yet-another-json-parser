#include "token.h"
#include <utility>
#include <sstream>

namespace yajp
{

Token::Token() : _type(Type::Invalid), _value()
{}

Token::Token(Type type, const std::string& value) : _type(type), _value(value)
{}

Token::Token(Type type, std::string&& value) : _type(type), _value(std::move(value))
{}

Token::Type Token::type() const
{
    return this->_type;
}

std::string Token::value() const
{
    return this->_value;
}

std::string Token::to_string() const
{
    std::ostringstream stream;
    stream << "Token(type=" << type_to_string(this->_type) << ", value=`" << this->_value << "`)";
    return stream.str();
}

constexpr const char* Token::type_to_string(Type type)
{
    return _type_str[static_cast<std::array<const char*, 12>::size_type>(type)];
}

}
