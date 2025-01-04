#include "value.h"
#include "parser.h"
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <utility>

using namespace yajp;

void print_help(const char* program_name)
{
    std::cout << "Usage: " << program_name << " FILE" << std::endl;
}

std::string value_to_string(const Value& value, const std::string& separator = "\n")
{
    std::ostringstream oss;
    switch (value.type())
    {
    case Value::Type::Null:
        oss << "null";
        break;
    case Value::Type::Number:
        oss << value.get<Value::Type::Number>();
        break;
    case Value::Type::String:
        oss << value.get<Value::Type::String>();
        break;
    case Value::Type::Bool:
        oss << std::boolalpha << value.get<Value::Type::Bool>()
            << std::resetiosflags(std::cout.boolalpha);
        break;
    case Value::Type::Object:
        for (const auto& [k, v] : value.get<Value::Type::Object>())
        {
            oss << "Key=" << k << ", Value=";
            if (v.type() == Value::Type::Array)
            {
                oss << "Array(" << &v << ')';
            }
            else if (v.type() == Value::Type::Object)
            {
                oss << "Object(" << &v << ')';
            }
            else
            {
                oss << value_to_string(v);
            }
            oss << separator;
        }
        break;
    case Value::Type::Array:
        for (const auto& v : value.get<Value::Type::Array>())
        {
            oss << "Value=";
            if (v.type() == Value::Type::Array)
            {
                oss << "Array(" << &v << ')';
            }
            else if (v.type() == Value::Type::Object)
            {
                oss << "Object(" << &v << ')';
            }
            else
            {
                oss << value_to_string(v);
            }
            oss << separator;
        }
        break;
    default:
        throw std::logic_error("Invalid Value Type");
    }
    return oss.str();
}

int main(int argc, const char** argv)
{
    if (argc != 2 || *argv[1] == '\0')
    {
        print_help(argv[0]);
        return -1;
    }

    std::string content;
    std::ifstream filestream;
    filestream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        filestream.open(argv[1]);
        std::ostringstream stringstream;
        stringstream << filestream.rdbuf();
        filestream.close();
        content = stringstream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    Parser parser;
    Value json = parser.parse(std::move(content));
    try
    {
        std::cout << value_to_string(json);
        std::cout << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}
