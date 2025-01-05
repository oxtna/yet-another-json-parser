#include "tokenizer.h"
#include <utility>
#include <cstddef>
#include <cctype>
#include <array>

namespace yajp
{

Tokenizer::Tokenizer(const std::string& input) : _input(input), _position(_input.begin())
{}

Tokenizer::Tokenizer(std::string&& input) : _input(std::move(input)), _position(_input.begin())
{}

Token Tokenizer::next()
{
    while (_position != _input.end() && is_whitespace(*_position))
    {
        _position++;
    }
    if (_position == _input.end())
    {
        return Token(Token::Type::End, {});
    }
    const char* first = &*_position;
    char c = *_position;
    _position++;
    switch (c)
    {
    case '{':
        return Token(Token::Type::LeftBrace, {first, 1});
    case '}':
        return Token(Token::Type::RightBrace, {first, 1});
    case '[':
        return Token(Token::Type::LeftBracket, {first, 1});
    case ']':
        return Token(Token::Type::RightBracket, {first, 1});
    case ',':
        return Token(Token::Type::Comma, {first, 1});
    case ':':
        return Token(Token::Type::Colon, {first, 1});
    case '"':
        return scan_string(first);
    case '-':
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        return scan_number(first);
    case 't':
        return scan_keyword_true(first);
    case 'f':
        return scan_keyword_false(first);
    case 'n':
        return scan_keyword_null(first);
    default:
    error:
        return Token(Token::Type::Invalid, {});
    }
}

std::vector<Token> Tokenizer::all()
{
    std::vector<Token> tokens;
    while (_position != _input.end())
    {
        tokens.emplace_back(next());
        if (tokens.back().type() == Token::Type::Invalid)
        {
            break;
        }
    }
    if (tokens.back().type() != Token::Type::Invalid && tokens.back().type() != Token::Type::End)
    {
        tokens.emplace_back(Token::Type::End, std::string());
    }
    return tokens;
}

Token Tokenizer::scan_number(const char* first)
{
    const char* c = first + 1;
    // numbers can only have a single zero at the start
    if ((*first == '0' && *c == '0') || (*first == '-' && *c == '0' && *(c + 1) == '0'))
    {
        return Token(Token::Type::Invalid, {});
    }
    while (std::isdigit(static_cast<unsigned char>(*c)))
    {
        c++;
    }
    // fraction
    if (*c == '.')
    {
        c++;
        while (std::isdigit(static_cast<unsigned char>(*c)))
        {
            c++;
        }
    }
    // exponent
    if (*c == 'e' || *c == 'E')
    {
        c++;
        if (*c == '-' || *c == '+')
        {
            c++;
        }
        while (std::isdigit(static_cast<unsigned char>(*c)))
        {
            c++;
        }
    }
    std::size_t length = c - first;
    _position += length - 1;
    return Token(Token::Type::Number, {first, length});
error:
    return Token(Token::Type::Invalid, {});
}

Token Tokenizer::scan_string(const char* first)
{
    const char* c = first + 1;
    while (*c != '"' && !is_control(*c))
    {
        if (*c == '\\')
        {
            c++;
            if (*c == 'u')
            {
                c++;
                for (int i = 0; i < 4; i++)
                {
                    if (!is_hex(*c))
                    {
                        // bad hex escape sequence
                        goto error;
                    }
                    c++;
                }
            }
            else if (is_escape(*c))
            {
                c++;
            }
            else
            {
                // bad escape sequence
                goto error;
            }
        }
        else
        {
            c++;
        }
    }
    if (*c == '"')
    {
        c++;
        std::size_t length = c - first;
        _position += length - 1;
        return Token(Token::Type::String, {first, length});
    }
error:
    return Token(Token::Type::Invalid, {});
}

Token Tokenizer::scan_keyword_true(const char* first)
{
    constexpr std::size_t length = 4;
    const char* c = first + 1;
    if (*c != 'r')
    {
        goto error;
    }
    c++;
    if (*c != 'u')
    {
        goto error;
    }
    c++;
    if (*c != 'e')
    {
        goto error;
    }
    _position += length - 1;
    return Token(Token::Type::KeywordTrue, {first, length});
error:
    return Token(Token::Type::Invalid, {});
}

Token Tokenizer::scan_keyword_false(const char* first)
{
    constexpr std::size_t length = 5;
    const char* c = first + 1;
    if (*c != 'a')
    {
        goto error;
    }
    c++;
    if (*c != 'l')
    {
        goto error;
    }
    c++;
    if (*c != 's')
    {
        goto error;
    }
    c++;
    if (*c != 'e')
    {
        goto error;
    }
    _position += length - 1;
    return Token(Token::Type::KeywordFalse, {first, length});
error:
    return Token(Token::Type::Invalid, {});
}

Token Tokenizer::scan_keyword_null(const char* first)
{
    constexpr std::size_t length = 4;
    const char* c = first + 1;
    if (*c != 'u')
    {
        goto error;
    }
    c++;
    if (*c != 'l')
    {
        goto error;
    }
    c++;
    if (*c != 'l')
    {
        goto error;
    }
    _position += length - 1;
    return Token(Token::Type::KeywordNull, {first, length});
error:
    return Token(Token::Type::Invalid, {});
}

constexpr bool Tokenizer::is_whitespace(char c)
{
    return std::isspace(static_cast<unsigned char>(c));
}

constexpr bool Tokenizer::is_control(char c)
{
    return std::iscntrl(static_cast<unsigned char>(c));
}

constexpr bool Tokenizer::is_escape(char c)
{
    return c == '"' || c == '\\' || c == '/' || c == 'b' || c == 'f' || c == 'n' || c == 'r' ||
           c == 't' || c == 'u';
}

constexpr bool Tokenizer::is_hex(char c)
{
    return std::isxdigit(static_cast<unsigned char>(c));
}

}
