#pragma once

#include <string_view>
#include <string>
#include <iostream>
#include <map>
#include <concepts>
#include <limits>
#include <utility>
#include <type_traits>

namespace TypeLib
{
    namespace TypeConcepts
    {
        template< class V >
        concept simple_value = requires (V & value, std::string_view value_str)
        {
            typename V::data_type;
            requires std::default_initializable<V>;
            requires std::constructible_from<typename V::data_type>;
            requires std::equality_comparable<V>;

            requires std::convertible_to<V, typename V::data_type>;

            { value.toString() }->std::convertible_to<std::string>;
            value.fromString(value_str);
            requires(std::is_const_v<V> || requires(V & value) { value.clear(); });

        };

        template< class F >
        concept simple_field = requires(F & field, std::string_view value_str)
        {
            typename F::value_type;
            requires simple_value<typename F::value_type>;

            requires std::is_trivially_copyable_v< F>;            
            typename F::name_t;
            typename F::unit_t;
            typename std::integral_constant< std::int32_t, F::tag() >;
            

            { field.empty() }->std::convertible_to<bool>;
            field.value();

            { field.toString() }->std::convertible_to<std::string>;
            field.fromString(value_str);
            requires( std::is_const_v<F> || requires(F & field) { field.clear(); } );

        };
    }

}

template< TypeLib::TypeConcepts::simple_field F>
constexpr bool operator ==(const F& lhs, const F& rhs)
{
    return lhs.value() == rhs.value();
}

template< TypeLib::TypeConcepts::simple_field F>
requires std::totally_ordered< typename F::value_type>
constexpr bool operator > (const F& lhs, const F& rhs)
{
    return lhs.value() > rhs.value();
}
template< TypeLib::TypeConcepts::simple_field F>
requires std::totally_ordered< typename F::value_type>
constexpr bool operator >= (const F& lhs, const F& rhs)
{
    return lhs.value() >= rhs.value();
}
template< TypeLib::TypeConcepts::simple_field F>
requires std::totally_ordered< typename F::value_type>
constexpr bool operator < (const F& lhs, const F& rhs)
{
    return lhs.value() < rhs.value();
}
template< TypeLib::TypeConcepts::simple_field F>
requires std::totally_ordered< typename F::value_type>
constexpr bool operator <= (const F& lhs, const F& rhs)
{
    return lhs.value() <= rhs.value();
}
template< TypeLib::TypeConcepts::simple_field F>
requires std::totally_ordered< typename F::value_type>
constexpr bool operator != (const F& lhs, const F& rhs)
{
    return lhs.value() != rhs.value();
}

template< class OSTREAM, TypeLib::TypeConcepts::simple_field F>
OSTREAM& operator << (OSTREAM& os, const F& field)
{
    os << field.toString();
    return os;
}

class F1 : private ZZZ::Int32_t
{
public:
    using value_type = ZZZ::Int32_t;
    using name_t = TYPE_STRING("F1");
    using unit_t = TYPE_STRING("");

    static consteval std::uint32_t tag()
    {
        return 123;
    }
    static consteval std::string_view name()
    {
        return name_t::value();
    }
    using value_type::value_type;
    
    constexpr F1(const F1&) = default;

    template< ZZZ::TypeConcepts::simple_field Arg>
    requires (std::same_as<F1::unit_t, typename Arg::unit_t>&& std::constructible_from< F1::value_type, typename Arg::value_type>)
    constexpr F1(Arg&& arg) : value_type(arg.value())
    {
    }

    using value_type::clear;
    using value_type::empty;
    using value_type::toString;
    using value_type::fromString;

    value_type& value()
    {
        return *this;
    }
    const value_type& value() const
    {
        return *this;
    }
};


class F3_enum
{
public:
    enum class data_type : int { Empty = -1, True = 1, False = 0 };
    static  constexpr data_type empty_value = data_type::Empty;

    constexpr F3_enum() = default;

    template <data_type V > using enum_constant = std::integral_constant < data_type, V>;


    using Name2Value = boost::mp11::mp_list
        <
        boost::mp11::mp_list< TYPE_STRING("Empty"), enum_constant< data_type::Empty > >,
        boost::mp11::mp_list< TYPE_STRING("True"), enum_constant< data_type::True  > >,
        boost::mp11::mp_list< TYPE_STRING("False"), enum_constant< data_type::False > >
        >;
    static_assert(boost::mp11::mp_is_map< Name2Value >::value, "Name2Value");

    using Value2Name = boost::mp11::mp_list
        <
        boost::mp11::mp_list< enum_constant< data_type::Empty >, TYPE_STRING("Empty")    >,
        boost::mp11::mp_list< enum_constant< data_type::True  >, TYPE_STRING("True")     >,
        boost::mp11::mp_list< enum_constant< data_type::False >, TYPE_STRING("False")    >
        >;
    static_assert(boost::mp11::mp_is_map< Value2Name >::value, "Value2Name");

    constexpr F3_enum(data_type val) : val_(val)
    {
    }
    constexpr F3_enum(const F3_enum&) = default;

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
    std::string_view toStringView() const
    {
        using namespace boost::mp11;
        switch (val_)
        {
        case data_type::Empty:
            return "Empty"sv;
            //return mp_second< mp_map_find<Value2Name, enum_constant< data_type::Empty > > >::value();
        case data_type::True:
            return "True"sv;
            //return mp_second< mp_map_find<Value2Name, enum_constant< data_type::True > > >::value();
        case data_type::False:
            return "False"sv;
            //return mp_second< mp_map_find<Value2Name, enum_constant< data_type::False > > >::value();
        default:
            break;

        }
        return ""sv;
    }
    std::string_view toStringView()
    {
        return std::as_const(*this).toStringView();
    }

    std::string toString() const
    {
        return std::string(toStringView());
    }
    std::string toString()
    {
        return std::string(std::as_const(*this).toStringView());
    }

    F3_enum & fromString(std::string_view s)
    {
        switch (ZZZ::string_hash(s))
        {
        case ZZZ::string_hash("Empty"sv):
            if (s == "Empty"sv)
            {
                val_ = data_type::Empty;
            }
            break;
        case ZZZ::string_hash("True"sv):
            if (s == "True"sv)
            {
                val_ = data_type::True;
            }
            break;
        case ZZZ::string_hash("False"sv):
            if (s == "False"sv)
            {
                val_ = data_type::False;
            }
            break;
        default:
            // empty ?
            break;
        }
        return *this;
    }

private:
    data_type val_ = empty_value;
};

class F3 : private F3_enum
{
public:
    using name_t = TYPE_STRING("F3");
    using unit_t = TYPE_STRING("");

    

    static consteval std::uint32_t tag()
    {
        return 1235;
    }
    static consteval std::string_view name()
    {
        return name_t::value();
    }
    using value_type = F3_enum;
   
    using value_type::value_type;

    constexpr F3(const F3&) = default;

    template< ZZZ::TypeConcepts::simple_field Arg>
        requires (std::same_as<F2::unit_t, typename Arg::unit_t>&& std::constructible_from< F2::value_type, typename Arg::value_type>)
    constexpr F3(Arg&& arg) : value_type(arg.value())
    {
    }

    using value_type::clear;
    using value_type::empty;
    using value_type::toString;
    using value_type::fromString;

    value_type& value()
    {
        return *this;
    }
    const value_type& value() const
    {
        return *this;
    }
    
};







