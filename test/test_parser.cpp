#include "value.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <limits>
#include <cstddef>
#include <cmath>

using namespace yajp;

bool equals(const Value& first, const Value& second)
{
    if (first.type() != second.type())
    {
        return false;
    }
    switch (first.type())
    {
    case Value::Type::Null:
        break;
    case Value::Type::Bool: {
        auto first_bool = first.get<Value::Type::Bool>();
        auto second_bool = second.get<Value::Type::Bool>();
        if (first_bool != second_bool)
        {
            return false;
        }
        break;
    }
    case Value::Type::String: {
        const auto& first_string = first.get<Value::Type::String>();
        const auto& second_string = first.get<Value::Type::String>();
        if (first_string != second_string)
        {
            return false;
        }
        break;
    }
    case Value::Type::Number: {
        auto first_number = first.get<Value::Type::Number>();
        auto second_number = second.get<Value::Type::Number>();
        if (std::abs(first_number - second_number) >
            std::numeric_limits<double>::epsilon() * std::max({1.0, first_number, second_number}))
        {
            return false;
        }
        break;
    }
    case Value::Type::Array: {
        const auto& first_array = first.get<Value::Type::Array>();
        const auto& second_array = second.get<Value::Type::Array>();
        if (first_array.size() != second_array.size())
        {
            return false;
        }
        for (std::size_t i{0}; i < first_array.size(); i++)
        {
            if (!equals(first_array[i], second_array[i]))
            {
                return false;
            }
        }
        break;
    }
    case Value::Type::Object: {
        const auto& first_object = first.get<Value::Type::Object>();
        const auto& second_object = second.get<Value::Type::Object>();
        if (first_object.size() != second_object.size())
        {
            return false;
        }
        for (auto first_it = first_object.begin(), second_it = second_object.begin();
             first_it != first_object.end();
             first_it++, second_it++)
        {
            const auto& [first_key, first_value] = *first_it;
            const auto& [second_key, second_value] = *second_it;
            if (first_key != second_key)
            {
                return false;
            }
            if (!equals(first_value, second_value))
            {
                return false;
            }
        }
        break;
    }
    }
    return true;
}

bool test(const std::string& test_data, const Value& test_target)
{
    Parser parser;
    Value value = parser.parse(test_data);
    if (!equals(value, test_target))
    {
        return false;
    }
    value = parser.parse_lazy(test_data);
    if (!equals(value, test_target))
    {
        return false;
    }
    return true;
}

int main()
{
    bool failed_any = false;
    std::string test_data = R"(null)";
    Value test_target(nullptr);
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 1.\n";
    }

    test_data = R"(true)";
    test_target = true;
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 2.\n";
    }

    test_data = R"(false)";
    test_target = false;
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 3.\n";
    }

    test_data = R"("string")";
    test_target = "string";
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 4.\n";
    }

    test_data = R"(1234)";
    test_target = 1234.0;
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 5.\n";
    }

    test_data = R"(21.37)";
    test_target = 21.37;
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 6.\n";
    }

    test_data = R"([])";
    test_target = Value::Array();
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 7.\n";
    }

    test_data = R"([true, false, null])";
    test_target = Value::Array({Value(true), Value(false), Value(nullptr)});
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 8.\n";
    }

    test_data = R"(["string1", "string2", "string3"])";
    test_target = Value::Array({Value("string1"), Value("string2"), Value("string3")});
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 9.\n";
    }

    test_data = R"([[], null, {}])";
    test_target = Value::Array({Value(Value::Array()), Value(nullptr), Value(Value::Object())});
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 10.\n";
    }

    test_data = R"({})";
    test_target = Value::Object();
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 11.\n";
    }

    test_data = R"({"key1": null, "key2": true, "key3": 3, "key4": "value4"})";
    test_target = Value::Object({
        {"key1", Value(nullptr)},
        {"key2", Value(true)},
        {"key3", Value(3.0)},
        {"key4", Value("value4")},
    });
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 12.\n";
    }

    test_data = R"({"key1": [], "key2": null, "key3": {}})";
    test_target = Value::Object({
        {"key1", Value(Value::Array())},
        {"key2", Value(nullptr)},
        {"key3", Value(Value::Object())},
    });
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 13.\n";
    }

    test_data = R"({"outerKey": {"innerKey": [[1, {}], [2, {}]]}})";
    test_target = Value::Object({
        {
            "outerKey",
            Value(Value::Object({
                {"innerKey",
                 Value(Value::Array({
                     Value(Value::Array({
                         Value(1.0),
                         Value(Value::Object()),
                     })),
                     Value(Value::Array({
                         Value(2.0),
                         Value(Value::Object()),
                     })),
                 }))},
            })),
        },
    });
    if (!test(test_data, test_target))
    {
        failed_any = true;
        std::cerr << "Failed test case 14.\n";
    }

    return failed_any ? -1 : 0;
}
