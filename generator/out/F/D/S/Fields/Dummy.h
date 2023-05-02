#include <F/D/S/Enums/Dummy.h>

namespace F::D::S
{
    class Dummy : private F::D::S::Dummy_enum
    {
        public:
        using value_type = F::D::S::Dummy_enum;
        using name_t = TYPE_STRING("Dummy");
        using unit_t = TYPE_STRING("");
    
        static consteval std::uint32_t tag()
        {
            return 12345;
        }

        static consteval std::string_view name()
        {
            return name_t::value();
        }

        using value_type::value_type;
    
        constexpr Dummy (const Dummy&) = default;
    
        template< TypeLib::TypeConcepts::simple_field Arg>
        requires (std::same_as<Dummy::unit_t, typename Arg::unit_t>&& std::constructible_from< Dummy::value_type, typename Arg::value_type>)
        constexpr Dummy (Arg&& arg) : value_type(arg.value())
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


