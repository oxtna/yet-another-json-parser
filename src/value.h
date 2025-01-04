#pragma once
#include <variant>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <cstddef>

// std::variant cannot be used in recursive definitions

namespace yajp
{

class Value
{
  public:
    using Null = std::nullptr_t;
    using Number = double;
    using String = std::string;
    using Bool = bool;
    using Object = std::map<std::string, Value>;
    using Array = std::vector<Value>;

    enum class Type
    {
        Null = 0,
        Number = 1,
        String = 2,
        Bool = 3,
        Object = 4,
        Array = 5,
    };

    Value() = default;
    Value(const Value&) = default;
    Value(Value&&) = default;

    template <typename T>
    explicit Value(T&& value) : _value(std::forward<T>(value))
    {}

    template <typename T>
    T& get()
    {
        return std::get<T>(_value);
    }

    template <typename T>
    const T& get() const
    {
        return std::get<T>(_value);
    }

    template <Type ValueType>
    constexpr decltype(auto) get() const
    {
        return std::get<static_cast<std::size_t>(ValueType)>(_value);
    }

    constexpr Type type() const { return static_cast<Type>(_value.index()); }

    Value& operator=(const Value&) = default;
    Value& operator=(Value&&) = default;

    template <typename T>
    Value& operator=(T&& value)
    {
        _value = std::move(value);
        return *this;
    }

  private:
    std::variant<
        std::nullptr_t, double, std::string, bool, std::map<std::string, Value>, std::vector<Value>>
        _value;
};

}
