
namespace F::D::S
{
class Dummy_enum
{
public:
    enum class data_type : char 
    {
         Empty = -1,
 True = 1,
 False = 0 
    };

    static  constexpr data_type empty_value = data_type::Empty;

    constexpr Dummy_enum() = default;

    template <data_type V > using enum_constant = std::integral_constant < data_type, V>;

    using Name2Value = boost::mp11::mp_list
    <
            boost::mp11::mp_list< TYPE_STRING("Empty"), enum_constant< data_type::Empty > >,
boost::mp11::mp_list< TYPE_STRING("True"), enum_constant< data_type::True > >,
boost::mp11::mp_list< TYPE_STRING("False"), enum_constant< data_type::False > >
    >;
    static_assert(boost::mp11::mp_is_map< Name2Value >::value, "Name2Value");

    using Value2Name = boost::mp11::mp_list
    <
            boost::mp11::mp_list< enum_constant< data_type::Empty >, TYPE_STRING("Empty"),
boost::mp11::mp_list< enum_constant< data_type::True >, TYPE_STRING("True"),
boost::mp11::mp_list< enum_constant< data_type::False >, TYPE_STRING("False")
    >;
    static_assert(boost::mp11::mp_is_map< Value2Name >::value, "Value2Name");

    constexpr Dummy_enum(data_type val) : val_(val)
    {
    }
    constexpr Dummy_enum(const Dummy_enum&) = default;

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
        case data_type::Empty: return "Empty"sv;
case data_type::True: return "True"sv;
case data_type::False: return "False"sv;
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

    Dummy_enum & fromString(std::string_view s)
    {
        switch (TypeLib::string_hash(s))
        {
         case TypeLib::string_hash("Empty"sv): if (s == "Empty"sv) { val_ = data_type::Empty;} break;
        
case TypeLib::string_hash("True"sv): if (s == "True"sv) { val_ = data_type::True;} break;
        
case TypeLib::string_hash("False"sv): if (s == "False"sv) { val_ = data_type::False;} break;
        
         default: break;
        }
        return *this;
    }

private:
    data_type val_ = empty_value;
};

}

