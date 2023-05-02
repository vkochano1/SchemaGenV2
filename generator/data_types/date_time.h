#include <utils.h>


namespace TypeLib
{

    template< class T >
    requires std::chrono::is_clock_v<T>
    class DateTimeT : public std::chrono::time_point<T>
    {
    public:
        using data_type = std::chrono::time_point<T>;

        DateTimeT() = default;
        DateTimeT(const DateTimeT &) = default;

        DateTimeT(const data_type& t) : data_type(t)
        {
        }
        DateTimeT(const typename data_type::duration & d) : data_type(d)
        {
        }     
        constexpr bool empty()
        {
            return this->time_since_epoch() == data_type::duration::zero();
        }
        constexpr bool empty() const
        {
            return this->time_since_epoch() == data_type::duration::zero();
        }

        void clear()
        {
            static_cast<data_type&>(*this) = data_type(data_type::duration::zero());
        }
        
        std::string toString() const
        {
            return std::format("{:L%F %T}", static_cast< const data_type &>(*this) );
        }
        std::string toString()
        {
            return std::as_const(*this).toString();
        }
        DateTimeT& fromString(std::string_view  s)
        {
            std::chrono::from_stream(std::istringstream(s), "%F %T", *this);
            return *this;
        }
        static decltype(auto) now()
        {
            return data_type::clock::now();
        }
        
    
    };
    using SystemDateTime = DateTimeT< std::chrono::system_clock>;
    
}