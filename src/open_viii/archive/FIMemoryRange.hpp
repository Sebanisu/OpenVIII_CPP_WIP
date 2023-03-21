//
// Created by pcvii on 3/21/2023.
//

#ifndef OPENVIII_CPP_WIP_FIMEMORYRANGE_HPP
#define OPENVIII_CPP_WIP_FIMEMORYRANGE_HPP
#include "FI.hpp"
namespace open_viii::archive {
class FIMemoryRange
{
public:
  class iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = FI;
    using difference_type   = std::ptrdiff_t;
    // using pointer           = const FI *;
    // using reference         = const FI &;
    static constexpr difference_type value_size
      = static_cast<difference_type>(sizeof(FI));
    iterator() = default;
    explicit iterator(
      std::span<const char> in_buffer,
      std::size_t           in_offset = 0)
      : m_buffer_iterator(in_buffer), m_offset(in_offset)
    {}

    iterator &
      operator++()
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) + value_size);
      return *this;
    }

    iterator
      operator++(int)
    {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    iterator &
      operator--()
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) - value_size);
      return *this;
    }

    iterator
      operator--(int)
    {
      iterator tmp(*this);
      --(*this);
      return tmp;
    }
    iterator
      operator+(iterator other) const
    {
      return iterator(m_buffer_iterator, m_offset + other.m_offset);
    }

    iterator
      operator+(difference_type n) const
    {
      return iterator(
        m_buffer_iterator,
        static_cast<std::size_t>(
          static_cast<difference_type>(m_offset) + n * value_size));
    }

    iterator friend
      operator+(difference_type n, iterator other)
    {
      return iterator(
        other.m_buffer_iterator,
        static_cast<std::size_t>(
          static_cast<difference_type>(other.m_offset) + n * value_size));
    }

    iterator
      operator-(difference_type n) const
    {
      return iterator(
        m_buffer_iterator,
        static_cast<std::size_t>(
          static_cast<difference_type>(m_offset) - n * value_size));
    }

    difference_type
      operator-(const iterator &other) const
    {
      return static_cast<difference_type>(m_offset - other.m_offset)
           / value_size;
    }

    iterator &
      operator+=(difference_type n)
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) + n * value_size);
      return *this;
    }

    iterator &
      operator-=(difference_type n)
    {
      m_offset = static_cast<std::size_t>(
        static_cast<difference_type>(m_offset) - n * value_size);

      return *this;
    }

    value_type
      operator*() const
    {
      FI         fi  = {};
      const auto tmp = m_buffer_iterator.subspan(m_offset, sizeof(FI));
      std::memcpy(&fi, tmp.data(), tmp.size());
      return fi;
    }
    //    const FI *
    //      operator->() const
    //    {
    //      return &m_fi;
    //    }
    value_type
      operator[](difference_type n) const
    {
      return *(*this + n);
    }

    bool
      operator==(const iterator &other) const
    {
      return same_range(other) && m_offset == other.m_offset;
    }
    bool
      operator!=(const iterator &other) const
    {
      return !(*this == other);
    }
    bool
      operator<(const iterator &other) const
    {
      return m_offset < other.m_offset;
    }
    bool
      operator<=(const iterator &other) const
    {
      return same_range(other) && m_offset <= other.m_offset;
    }
    bool
      operator>(const iterator &other) const
    {
      return m_offset > other.m_offset;
    }
    bool
      operator>=(const iterator &other) const
    {
      return same_range(other) && m_offset >= other.m_offset;
    }

  private:
    bool
      same_range(const iterator &other) const
    {
      // std::ranges::equal(m_buffer_iterator, other.m_buffer_iterator)
      return m_buffer_iterator.data() == other.m_buffer_iterator.data()
          && m_buffer_iterator.size() == other.m_buffer_iterator.size();
    }
    std::span<const char> m_buffer_iterator{};
    std::size_t           m_offset{};
    // FI             m_fi{};
  };

  iterator::value_type
    operator[](std::size_t n) const
  {
    FI         fi  = {};
    const auto tmp = m_buffer.subspan(n * sizeof(FI));
    std::memcpy(&fi, tmp.data(), tmp.size());
    return fi;
  }
  // The begin function returns an FIInputIterator that points to the beginning
  // of the file
  iterator
    begin() const
  {
    return iterator(m_buffer);
  }

  // The end function returns an FIInputIterator that points past the end of the
  // file
  iterator
    end() const
  {
    return iterator(m_buffer, m_buffer.size());
  }

  FIMemoryRange(std::span<const char> in_buffer)
    : m_buffer(std::move(in_buffer))
  {}

private:
  std::span<const char> m_buffer = {};
};

/**
 * Verify that the iterator type for FIMemoryRange meets the requirements of a random access iterator.
 */
static_assert(std::random_access_iterator<FIMemoryRange::iterator>);

/**
 * Verify that the type for FIMemoryRange meets the requirements of a random access range.
 */
static_assert(std::ranges::random_access_range<FIMemoryRange>);
}
#endif// OPENVIII_CPP_WIP_FIMEMORYRANGE_HPP
