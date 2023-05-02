#include <TypeLib/Int32.h>

namespace F::D::S
{
    class ABC : private TypeLib::Int32
    {
        public:
        using value_type = TypeLib::Int32;
        using name_t = TYPE_STRING("ABC");
        using unit_t = TYPE_STRING("");
    
        static consteval std::uint32_t tag()
        {
            return 1234;
        }

        static consteval std::string_view name()
        {
            return name_t::value();
        }

        using value_type::value_type;
    
        constexpr ABC (const ABC&) = default;
    
        template< TypeLib::TypeConcepts::simple_field Arg>
        requires (std::same_as<ABC::unit_t, typename Arg::unit_t>&& std::constructible_from< ABC::value_type, typename Arg::value_type>)
        constexpr ABC (Arg&& arg) : value_type(arg.value())
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
}


