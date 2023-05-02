#pragma once

#include <string_view>
#include <string>
#include <iostream>
#include <map>
#include <type_traits>
#include <concepts.h>

namespace TypeLib
{
    template< char ... C>
    class type_string
    {
    private:
        static constexpr char val_[sizeof...(C) + 1] = { C... , '\0' };
    public:
        static consteval std::string_view value()
        {
            return std::string_view(val_, sizeof...(C));
        }
    };

    // fnv1a hash

    static constexpr std::uint32_t Prime = 0x01000193; //   16777619
    static constexpr std::uint32_t Seed = 0x811C9DC5; // 2166136261
  
    inline constexpr std::uint32_t string_hash(std::string_view s, std::uint32_t hash = Seed)
    {  
        if (s.empty())
        {
            return 0;
        }
        if (1 == s.length())
        {
            return (static_cast<unsigned char>(s.front()) ^ hash) * Prime;
        }
        return string_hash(s.substr(1), string_hash(s.substr(0, 1), hash));
    }

    template< std::integral V>
    class ValueT
    {
    public:
        using   data_type = V;
        static  constexpr data_type empty_value = std::numeric_limits<data_type>::min();

        constexpr ValueT() : val_(empty_value)
        {
        }
        constexpr ValueT(data_type val) : val_(val)
        {
        }
        constexpr ValueT(const ValueT&) = default;

        constexpr operator data_type() const
        {
            return val_;
        }
        constexpr bool empty() const
        {
            return empty_value == val_;
        }
        constexpr bool empty()
        {
            return empty_value == val_;
        }
        void clear()
        {
            val_ = empty_value;
        }
        std::string toString() const
        {
            return std::to_string(val_);
        }
        std::string toString()
        {
            return std::as_const(*this).toString();
        }
        ValueT& fromString(std::string_view  s)
        {
            auto rc = std::from_chars(s.begin(), s.end(), val_);
            return *this;
        }
    private:
        data_type val_;
    };

}

#define TYPE_STRING(STR)                                                                                    \
decltype                                                                                                    \
(                                                                                                           \
    std::declval                                                                                            \
    <                                                                                                       \
        decltype                                                                                            \
        (                                                                                                   \
            []<std::size_t ... idx >( std::index_sequence<idx...> ) -> TypeLib::type_string< STR##sv[idx]...>   \
            {                                                                                               \
                return TypeLib::type_string< STR##sv[idx]...>{};                                                \
            }                                                                                               \
        )                                                                                                   \
    >()                                                                                                     \
    ( std::declval< std::make_index_sequence< sizeof(STR) - 1 > >() )                                       \
) 