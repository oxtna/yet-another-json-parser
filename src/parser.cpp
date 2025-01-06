#include "parser.h"
#include "tokenizer.h"
#include <vector>
#include <utility>

namespace yajp
{

Value Parser::parse(const std::string& string)
{
    reset();
    Tokenizer tokenizer(std::move(string));
    std::vector<Token> tokens = tokenizer.all();
    State state = State::Initial;
    for (const auto& token : tokens)
    {
        if (token.type() == Token::Type::Invalid)
        {
            break;
        }
        consume(token);
        if (_current_state == State::Error)
        {
            break;
        }
    }
    if (_current_state != State::End)
    {
        throw ParserError("Invalid JSON");
    }
    return std::move(_global_value);
}

Value Parser::parse(std::string&& string)
{
    reset();
    Tokenizer tokenizer(std::move(string));
    std::vector<Token> tokens = tokenizer.all();
    for (const auto& token : tokens)
    {
        if (token.type() == Token::Type::Invalid)
        {
            break;
        }
        consume(token);
        if (_current_state == State::Error)
        {
            break;
        }
    }
    if (_current_state != State::End)
    {
        throw ParserError("Invalid JSON");
    }
    return std::move(_global_value);
}

Value Parser::parse_lazy(const std::string& string)
{
    reset();
    Tokenizer tokenizer(std::move(string));
    for (Token token = tokenizer.next(); token.type() != Token::Type::End; token = tokenizer.next())
    {
        if (token.type() == Token::Type::Invalid)
        {
            break;
        }
        consume(token);
        if (_current_state == State::Error)
        {
            break;
        }
    }
    if (_current_state != State::End)
    {
        throw ParserError("Invalid JSON");
    }
    return std::move(_global_value);
}

Value Parser::parse_lazy(std::string&& string)
{
    reset();
    Tokenizer tokenizer(std::move(string));
    for (Token token = tokenizer.next(); token.type() != Token::Type::End; token = tokenizer.next())
    {
        if (token.type() == Token::Type::Invalid)
        {
            break;
        }
        consume(token);
        if (_current_state == State::Error)
        {
            break;
        }
    }
    if (_current_state != State::End)
    {
        throw ParserError("Invalid JSON");
    }
    return std::move(_global_value);
}

void Parser::reset()
{
    _current_state = State::Initial;
    _global_value = nullptr;
    _depth_stack = {};
    _key_stack = {};
}

void Parser::consume(const Token& token)
{
    State next_state = State::Error;
    switch (_current_state)
    {
    case State::Initial: {
        switch (token.type())
        {
        case Token::Type::String:
            _global_value = std::move(strip_string_quotes(token.value()));
            next_state = State::End;
            break;
        case Token::Type::Number:
            _global_value = std::stod(token.value());
            next_state = State::End;
            break;
        case Token::Type::KeywordTrue:
            _global_value = true;
            next_state = State::End;
            break;
        case Token::Type::KeywordFalse:
            _global_value = false;
            next_state = State::End;
            break;
        case Token::Type::KeywordNull:
            _global_value = nullptr;
            next_state = State::End;
            break;
        case Token::Type::LeftBrace:
            _global_value = Value::Object();
            _depth_stack.push(&_global_value);
            next_state = State::Object;
            break;
        case Token::Type::LeftBracket:
            _global_value = Value::Array();
            _depth_stack.push(&_global_value);
            next_state = State::Array;
            break;
        default:
            break;
        }
        break;
    }
    case State::Object: {
        switch (token.type())
        {
        case Token::Type::RightBrace:
            _depth_stack.pop();
            if (_depth_stack.empty())
            {
                next_state = State::End;
            }
            else if (_depth_stack.top()->type() == Value::Type::Array)
            {
                next_state = State::ArrayValue;
            }
            else  // _depth_stack.top()->type() == Value::Type::Object
            {
                next_state = State::ObjectValue;
            }
            break;
        case Token::Type::String:
            _key_stack.push(strip_string_quotes(token.value()));
            next_state = State::ObjectKey;
            break;
        default:
            break;
        }
        break;
    }
    case State::ObjectKey: {
        switch (token.type())
        {
        case Token::Type::Colon:
            next_state = State::ObjectColon;
            break;
        default:
            break;
        }
        break;
    }
    case State::ObjectColon: {
        switch (token.type())
        {
        case Token::Type::String:
            _depth_stack.top()->get<Value::Object>().emplace(
                _key_stack.top(), strip_string_quotes(token.value()));
            _key_stack.pop();
            next_state = State::ObjectValue;
            break;
        case Token::Type::Number:
            _depth_stack.top()->get<Value::Object>().emplace(
                _key_stack.top(), std::stod(token.value()));
            _key_stack.pop();
            next_state = State::ObjectValue;
            break;
        case Token::Type::KeywordTrue:
            _depth_stack.top()->get<Value::Object>().emplace(_key_stack.top(), true);
            _key_stack.pop();
            next_state = State::ObjectValue;
            break;
        case Token::Type::KeywordFalse:
            _depth_stack.top()->get<Value::Object>().emplace(_key_stack.top(), false);
            _key_stack.pop();
            next_state = State::ObjectValue;
            break;
        case Token::Type::KeywordNull:
            _depth_stack.top()->get<Value::Object>().emplace(_key_stack.top(), nullptr);
            _key_stack.pop();
            next_state = State::ObjectValue;
            break;
        case Token::Type::LeftBrace:
            _depth_stack.top()->get<Value::Object>().emplace(_key_stack.top(), Value::Object());
            _depth_stack.push(&_depth_stack.top()->get<Value::Object>().at(_key_stack.top()));
            _key_stack.pop();
            next_state = State::Object;
            break;
        case Token::Type::LeftBracket:
            _depth_stack.top()->get<Value::Object>().emplace(_key_stack.top(), Value::Array());
            _depth_stack.push(&_depth_stack.top()->get<Value::Object>().at(_key_stack.top()));
            _key_stack.pop();
            next_state = State::Array;
            break;
        default:
            break;
        }
        break;
    }
    case State::ObjectValue: {
        switch (token.type())
        {
        case Token::Type::Comma:
            next_state = State::ObjectComma;
            break;
        case Token::Type::RightBrace:
            _depth_stack.pop();
            if (_depth_stack.empty())
            {
                next_state = State::End;
            }
            else if (_depth_stack.top()->type() == Value::Type::Array)
            {
                next_state = State::ArrayValue;
            }
            else  // _depth_stack.top()->type() == Value::Type::Object
            {
                next_state = State::ObjectValue;
            }
            break;
        default:
            break;
        }
        break;
    }
    case State::ObjectComma: {
        switch (token.type())
        {
        case Token::Type::String:
            _key_stack.push(strip_string_quotes(token.value()));
            next_state = State::ObjectKey;
            break;
        default:
            break;
        }
        break;
    }
    case State::Array: {
        switch (token.type())
        {
        case Token::Type::RightBracket:
            _depth_stack.pop();
            if (_depth_stack.empty())
            {
                next_state = State::End;
                break;
            }
            else if (_depth_stack.top()->type() == Value::Type::Array)
            {
                next_state = State::ArrayValue;
            }
            else  //_depth_stack.top()->type() == Value::Type::Object
            {
                next_state = State::ObjectValue;
            }
            break;
        case Token::Type::String:
            _depth_stack.top()->get<Value::Array>().emplace_back(
                strip_string_quotes(token.value()));
            next_state = State::ArrayValue;
            break;
        case Token::Type::Number:
            _depth_stack.top()->get<Value::Array>().emplace_back(std::stod(token.value()));
            next_state = State::ArrayValue;
            break;
        case Token::Type::KeywordTrue:
            _depth_stack.top()->get<Value::Array>().emplace_back(true);
            next_state = State::ArrayValue;
            break;
        case Token::Type::KeywordFalse:
            _depth_stack.top()->get<Value::Array>().emplace_back(false);
            next_state = State::ArrayValue;
            break;
        case Token::Type::KeywordNull:
            _depth_stack.top()->get<Value::Array>().emplace_back(nullptr);
            next_state = State::ArrayValue;
            break;
        case Token::Type::LeftBrace:
            _depth_stack.top()->get<Value::Array>().emplace_back(Value::Object());
            _depth_stack.push(&_depth_stack.top()->get<Value::Array>().back());
            next_state = State::Object;
            break;
        case Token::Type::LeftBracket:
            _depth_stack.top()->get<Value::Array>().emplace_back(Value::Array());
            _depth_stack.push(&_depth_stack.top()->get<Value::Array>().back());
            next_state = State::Array;
            break;
        default:
            break;
        }
        break;
    }
    case State::ArrayValue: {
        switch (token.type())
        {
        case Token::Type::Comma:
            next_state = State::ArrayComma;
            break;
        case Token::Type::RightBracket:
            _depth_stack.pop();
            if (_depth_stack.empty())
            {
                next_state = State::End;
                break;
            }
            else if (_depth_stack.top()->type() == Value::Type::Array)
            {
                next_state = State::ArrayValue;
            }
            else  //_depth_stack.top()->type() == Value::Type::Object
            {
                next_state = State::ObjectValue;
            }
            break;
        default:
            break;
        }
        break;
    }
    case State::ArrayComma: {
        switch (token.type())
        {
        case Token::Type::String:
            _depth_stack.top()->get<Value::Array>().emplace_back(
                strip_string_quotes(token.value()));
            next_state = State::ArrayValue;
            break;
        case Token::Type::Number:
            _depth_stack.top()->get<Value::Array>().emplace_back(std::stod(token.value()));
            next_state = State::ArrayValue;
            break;
        case Token::Type::KeywordTrue:
            _depth_stack.top()->get<Value::Array>().emplace_back(true);
            next_state = State::ArrayValue;
            break;
        case Token::Type::KeywordFalse:
            _depth_stack.top()->get<Value::Array>().emplace_back(false);
            next_state = State::ArrayValue;
            break;
        case Token::Type::KeywordNull:
            _depth_stack.top()->get<Value::Array>().emplace_back(nullptr);
            next_state = State::ArrayValue;
            break;
        case Token::Type::LeftBrace:
            _depth_stack.top()->get<Value::Array>().emplace_back(Value::Object());
            _depth_stack.push(&_depth_stack.top()->get<Value::Array>().back());
            next_state = State::Object;
            break;
        case Token::Type::LeftBracket:
            _depth_stack.top()->get<Value::Array>().emplace_back(Value::Array());
            _depth_stack.push(&_depth_stack.top()->get<Value::Array>().back());
            next_state = State::Array;
            break;
        default:
            break;
        }
        break;
    }
    case State::End: {
        switch (token.type())
        {
        case Token::Type::End: {
            next_state = State::End;
            break;
        }
        default:
            break;
        }
        break;
    }
    case State::Error:
    default:
        break;
    }
    _current_state = next_state;
}

std::string Parser::strip_string_quotes(const std::string& string) const
{
    return string.substr(1, string.size() - 2);
}

}
