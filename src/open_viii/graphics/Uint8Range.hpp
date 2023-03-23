//
// Created by pcvii on 3/22/2023.
//

#ifndef OPENVIII_CPP_WIP_UINT8RANGE_HPP
#define OPENVIII_CPP_WIP_UINT8RANGE_HPP
#include "Bit4Values.hpp"
#include <concepts>
#include <iterator>
#include <ranges>
#include <utility>
#include <vector>
namespace open_viii::graphics {
template<std::ranges::bidirectional_range Range>
  requires std::same_as<std::ranges::range_value_t<Range>, Bit4Values>
class Uint8Range
{
private:
  Range m_input_range;

public:
  Uint8Range(const Range &input_range) : m_input_range(input_range) {}

  class iterator
  {
  private:
    using inner_iterator = std::ranges::iterator_t<Range>;
    inner_iterator m_inner_it;
    bool           m_first;

  public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type        = std::uint8_t;
    using difference_type   = std::ptrdiff_t;
    using pointer           = value_type *;
    using reference         = value_type;
    using inner_value_type
      = std::remove_cvref_t<std::ranges::range_value_t<Range>>;

    iterator() = default;

    iterator(const inner_iterator &inner_it, bool first = true)
      : m_inner_it(inner_it), m_first(first)
    {}

    reference
      operator*() const
    {
      const auto[first,second] = *m_inner_it;
      return m_first ? first : second;
    }

    iterator &
      operator++()
    {
      if (m_first) {
        m_first = false;
      }
      else {
        m_first = true;
        ++m_inner_it;
      }
      return *this;
    }

    iterator
      operator++(int)
    {
      iterator temp = *this;
      ++(*this);
      return temp;
    }

    iterator &
      operator--()
    {
      if (m_first) {
        --m_inner_it;
        m_first = false;
      }
      else {
        m_first = true;
      }
      return *this;
    }

    iterator
      operator--(int)
    {
      iterator temp = *this;
      --(*this);
      return temp;
    }

    bool
      operator==(const iterator &other) const
    {
      return m_inner_it == other.m_inner_it && m_first == other.m_first;
    }

    bool
      operator!=(const iterator &other) const
    {
      return !(*this == other);
    }
  };

  iterator
    begin()
  {
    return iterator(std::ranges::begin(m_input_range));
  }

  iterator
    end()
  {
    return iterator(std::ranges::end(m_input_range), false);
  }

  iterator
    begin() const
  {
    return iterator(std::ranges::cbegin(m_input_range));
  }

  iterator
    end() const
  {
    return iterator(std::ranges::cend(m_input_range), false);
  }

  iterator
    cbegin() const
  {
    return iterator(std::ranges::cbegin(m_input_range));
  }

  iterator
    cend() const
  {
    return iterator(std::ranges::cend(m_input_range), false);
  }
};
static_assert(
  std::bidirectional_iterator<Uint8Range<std::vector<Bit4Values>>::iterator>);
static_assert(
  std::ranges::bidirectional_range<Uint8Range<std::vector<Bit4Values>>>);
}// namespace open_viii::graphics
#endif// OPENVIII_CPP_WIP_UINT8RANGE_HPP
