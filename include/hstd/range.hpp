#pragma once

#include <cstddef>  // ptrdiff_t
#include <iterator>  // std::distance
#include <type_traits>  // std::is_arithmetic, std::conditional
#include <utility>  // std::forward, std::declval



namespace hstd
{
  template< typename Iterator1, typename Iterator2 = Iterator1 >
  struct basic_range
  {
   public:
      using is_arithmetic_type = std::is_arithmetic<Iterator1>;

      using difference_type =
        std::conditional_t<
          is_arithmetic_type::value,
          decltype( std::declval<Iterator2>() - std::declval<Iterator1>() ),
          ptrdiff_t
        >;

   public:
    basic_range( Iterator1 first, Iterator2 last )
      : m_first( first ), m_last( last )
    {}

    basic_range() : m_first(), m_last() {}

    template< typename Iterator3, typename Iterator4 >
    basic_range( basic_range< Iterator3, Iterator4 > that_range )
      : m_first( that_range.begin() ), m_last( that_range.end() )
    {}

   public:
    Iterator1 begin() const noexcept { return m_first; }
    Iterator2 end() const noexcept { return m_last; }

    auto rbegin() const noexcept -> std::reverse_iterator<Iterator2> {
      return std::reverse_iterator<Iterator2>( m_last );
    }
    auto rend() const noexcept -> std::reverse_iterator<Iterator1> {
      return std::reverse_iterator<Iterator1>( m_first );
    }

    difference_type size() const noexcept {
      return tagged_size( m_first, m_last, is_arithmetic_type() );
    }

    bool empty() const noexcept { return m_first == m_last; }

   public:
    template< typename Iterator3, typename Iterator4 >
    bool operator == (
        basic_range<Iterator3, Iterator4> const & that_range
      ) const noexcept
    {
      return m_first == that_range.m_first and m_last == that_range.m_last;
    }

   private:
    static auto tagged_size( Iterator1 first, Iterator2 last,
                             std::true_type ) noexcept {
      return last - first;
    }
    static auto tagged_size( Iterator1 first, Iterator2 last,
                             std::false_type ) noexcept {
      return std::distance<Iterator1>( first, last );
    }

   private:
    Iterator1 m_first;
    Iterator2 m_last;
  };
}  // namespace hstd

namespace hstd
{
  using char_range = basic_range< char* >;
  using cchar_range = basic_range< char const * >;

  using int8_range = basic_range< int8_t >;
  using int16_range = basic_range< int16_t >;
}  // namespace hstd


namespace hstd
{
  template< typename Iterator1, typename Iterator2 >
  auto range( Iterator1 first, Iterator2 last )
    -> basic_range<Iterator1, Iterator2>
  {
      return basic_range<Iterator1, Iterator2>( first, last );
  }

  template< typename T >
  auto range( T && obj )
  {
      return range( std::begin( std::forward<T>(obj) ),
                    std::end( std::forward<T>(obj) ) );
  }
}  // namespace hstd

namespace hstd
{
  template< typename Iterator1, typename Iterator2 >
  auto begin( mystd::basic_range< Iterator1, Iterator2 > range )
      -> Iterator1
  {
    return range.begin();
  }

  template< typename Iterator1, typename Iterator2 >
  auto end( mystd::basic_range< Iterator1, Iterator2 > range )
      -> Iterator2
  {
    return range.end();
  }

  template< typename Iterator1, typename Iterator2 >
  auto cbegin( mystd::basic_range< Iterator1, Iterator2 > range )
      -> Iterator1
  {
    return range.cbegin();
  }

  template< typename Iterator1, typename Iterator2 >
  auto cend( mystd::basic_range< Iterator1, Iterator2 > range )
      -> Iterator2
  {
    return range.cend();
  }
}  // namespace hstd

// Todo: enable it only for [random access]/[contiguous|since c++17] iterators.
template< typename OStream, typename Iterator1, typename Iterator2 >
OStream & operator << ( OStream & out,
                        mystd::basic_range<Iterator1, Iterator2> range )
{
  return out.write( range.begin(), range.size() );
}

// Todo: enable it only for [random access]/[contiguous|since c++17] iterators.
template< typename OStream, typename Iterator1, typename Iterator2 >
OStream && operator << ( OStream && out,
                         mystd::basic_range<Iterator1, Iterator2> range )
{
  return std::move( out.write( range.begin(), range.size() ) );
}
