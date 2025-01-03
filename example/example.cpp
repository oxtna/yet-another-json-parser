#include "token.h"
#include "tokenizer.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <utility>
#include <algorithm>

using namespace yajp;

void print_help(const char* program_name)
{
    std::cout << "Usage: " << program_name << " FILE" << std::endl;
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

    Tokenizer tokenizer(std::move(content));
    auto tokens = tokenizer.all();

    for (const auto& token : tokens)
    {
        std::cout << token.to_string() << '\n';
    }
    std::cout << std::endl;

    return !std::any_of(tokens.begin(), tokens.end(), [](const Token& token) {
        return token.type() == Token::Type::Invalid;
    });
}
