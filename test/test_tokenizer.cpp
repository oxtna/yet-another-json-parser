#include "token.h"
#include "tokenizer.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstddef>
#include <utility>

using namespace yajp;

bool test(std::string&& test_data, const std::vector<Token>& test_target)
{
    Tokenizer tokenizer(std::move(test_data));
    std::vector<Token> tokens = tokenizer.all();
    if (tokens.size() != test_target.size())
    {
        return false;
    }
    for (std::size_t i{0}; i < test_target.size(); i++)
    {
        if (tokens[i].type() != test_target[i].type() ||
            tokens[i].value() != test_target[i].value())
        {
            return false;
        }
    }
    return true;
}

int main()
{
    bool failed_any = false;
    std::string test_data(R"(
[
  {
    "id": 1,
    "name": "Leanne Graham",
    "username": "Bret",
    "email": "Sincere@april.biz",
    "address": {
      "street": "Kulas Light",
      "suite": "Apt. 556",
      "city": "Gwenborough",
      "zipcode": "92998-3874",
      "geo": {
        "lat": "-37.3159",
        "lng": "81.1496"
      }
    },
    "phone": "1-770-736-8031 x56442",
    "website": "hildegard.org",
    "company": {
      "name": "Romaguera-Crona",
      "catchPhrase": "Multi-layered client-server neural-net",
      "bs": "harness real-time e-markets"
    }
  }
]
    )");
    std::vector<Token> test_target = {
        Token(Token::Type::LeftBracket, "["),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::String, "\"id\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::Number, "1"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"name\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Leanne Graham\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"username\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Bret\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"email\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Sincere@april.biz\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"address\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::String, "\"street\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Kulas Light\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"suite\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Apt. 556\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"city\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Gwenborough\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"zipcode\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"92998-3874\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"geo\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::String, "\"lat\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"-37.3159\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"lng\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"81.1496\""),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"phone\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"1-770-736-8031 x56442\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"website\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"hildegard.org\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"company\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::String, "\"name\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Romaguera-Crona\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"catchPhrase\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"Multi-layered client-server neural-net\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"bs\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"harness real-time e-markets\""),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::RightBracket, "]"),
        Token(Token::Type::End, {}),
    };
    if (!test(std::move(test_data), test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 1.\n";
    }

    test_data = R"(
[
  {
    "albumId": 1,
    "id": 1,
    "title": "accusamus beatae ad facilis cum similique qui sunt",
    "url": "https://via.placeholder.com/600/92c952",
    "thumbnailUrl": "https://via.placeholder.com/150/92c952"
  },
  {
    "albumId": 1,
    "id": 2,
    "title": "reprehenderit est deserunt velit ipsam",
    "url": "https://via.placeholder.com/600/771796",
    "thumbnailUrl": "https://via.placeholder.com/150/771796"
  }
]
    )";
    test_target = {
        Token(Token::Type::LeftBracket, "["),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::String, "\"albumId\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::Number, "1"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"id\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::Number, "1"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"title\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"accusamus beatae ad facilis cum similique qui sunt\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"url\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"https://via.placeholder.com/600/92c952\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"thumbnailUrl\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"https://via.placeholder.com/150/92c952\""),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::String, "\"albumId\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::Number, "1"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"id\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::Number, "2"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"title\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"reprehenderit est deserunt velit ipsam\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"url\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"https://via.placeholder.com/600/771796\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"thumbnailUrl\""),
        Token(Token::Type::Colon, ":"),
        Token(Token::Type::String, "\"https://via.placeholder.com/150/771796\""),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::RightBracket, "]"),
        Token(Token::Type::End, {}),
    };
    if (!test(std::move(test_data), test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 2.\n";
    }

    test_data = R"([true, false, null, 1, {}])";
    test_target = {
        Token(Token::Type::LeftBracket, "["),
        Token(Token::Type::KeywordTrue, "true"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::KeywordFalse, "false"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::KeywordNull, "null"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::Number, "1"),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::LeftBrace, "{"),
        Token(Token::Type::RightBrace, "}"),
        Token(Token::Type::RightBracket, "]"),
        Token(Token::Type::End, {}),
    };
    if (!test(std::move(test_data), test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 3.\n";
    }

    test_data = R"(["\"", "\n", "\t", "\r", "\\"])";
    test_target = {
        Token(Token::Type::LeftBracket, "["),
        Token(Token::Type::String, "\"\\\"\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"\\n\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"\\t\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"\\r\""),
        Token(Token::Type::Comma, ","),
        Token(Token::Type::String, "\"\\\\\""),
        Token(Token::Type::RightBracket, "]"),
        Token(Token::Type::End, {}),
    };
    if (!test(std::move(test_data), test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 4.\n";
    }

    test_data = "";
    test_target = {Token(Token::Type::End, {})};
    if (!test(std::move(test_data), test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 5.\n";
    }

    return failed_any ? -1 : 0;
}
