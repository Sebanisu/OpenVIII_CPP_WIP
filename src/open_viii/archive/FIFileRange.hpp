/**
* @file FIFileRange.hpp
* @author pcvii
* @date 3/21/2023
* @brief A container class for reading FI records from a file.
*/
#ifndef OPENVIII_CPP_WIP_FIFILERANGE_HPP
#define OPENVIII_CPP_WIP_FIFILERANGE_HPP
#include "FI.hpp"
namespace open_viii::archive {
/**
 * @class FIFileRange
 * @brief Provides random access to FI records stored in a file.
 */
class FIFileRange
{
public:/**
* @class iterator
* @brief Random access iterator for FI records.
      */
  class iterator
  {
  public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = FI;
    using difference_type   = std::ifstream::off_type;
    using pointer           = void;
    using reference         = void;
    static constexpr difference_type value_size
      = static_cast<difference_type>(sizeof(FI));
    iterator() = default;
    /**
     * @brief Construct iterator from ifstream and offset.
     * @param ifs ifstream reference.
     * @param offset Offset in file.
     */
    explicit iterator(std::ifstream &ifs, difference_type offset = 0)
      : m_ifs(&ifs), m_offset(offset)
    {}

    iterator &
      operator++()
    {
      m_offset += value_size;
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
      m_offset -= value_size;
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
      return iterator(*m_ifs, m_offset + other.m_offset);
    }

    iterator
      operator+(difference_type n) const
    {
      return iterator(*m_ifs, m_offset + n * value_size);
    }

    iterator friend
      operator+(difference_type n, iterator other)
    {
      return iterator(*other.m_ifs, other.m_offset + n * value_size);
    }

    iterator
      operator-(difference_type n) const
    {
      return iterator(*m_ifs, m_offset - n * value_size);
    }

    difference_type
      operator-(const iterator &other) const
    {
      return (m_offset - other.m_offset) / value_size;
    }

    iterator &
      operator+=(difference_type n)
    {
      m_offset += n * value_size;
      return *this;
    }

    iterator &
      operator-=(difference_type n)
    {
      m_offset -= n * value_size;
      return *this;
    }

    value_type
      operator*() const
    {
      FI fi = {};
      m_ifs->seekg(m_offset);
      *m_ifs >> fi;
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
      return m_ifs == other.m_ifs && m_offset == other.m_offset;
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
      return m_ifs == other.m_ifs && m_offset <= other.m_offset;
    }
    bool
      operator>(const iterator &other) const
    {
      return m_offset > other.m_offset;
    }
    bool
      operator>=(const iterator &other) const
    {
      return m_ifs == other.m_ifs && m_offset >= other.m_offset;
    }

  private:
    std::ifstream *m_ifs{};
    std::streamoff m_offset{};
  };

  /**
   * @brief Get iterator pointing to beginning of FI records.
   * @return iterator pointing to first FI record.
   */
  iterator
    begin()
  {
    open();
    return iterator(m_file_stream, m_begin);
  }

  /**
   * @brief Get iterator pointing past the end of FI records.
   * @return iterator pointing past last FI record.
   */
  iterator
    end()
  {
    open();
    // We need to seek to the end of the file to get the offset

    if (m_end > 0) {
      return iterator(m_file_stream, m_end);
    }
    std::streamoff offset = m_file_stream.seekg(0, std::ios::end).tellg();
    return iterator(m_file_stream, offset);
  }
  iterator::value_type
    operator[](iterator::difference_type n)
  {
    if (!m_file_stream) {
      open();
    }
    FI fi = {};
    m_file_stream.seekg(m_begin + (n * iterator::value_size));
    m_file_stream >> fi;
    return fi;
  }
  /**
   * @brief Construct FIFileRange with file path.
   * @param in_file_path Input file path.
   */
  FIFileRange(std::filesystem::path in_file_path)
    : m_file_path(std::move(in_file_path))
  {}

    /**
     * @brief Construct FIFileRange with file path, begin, and size.
     * @param in_file_path Input file path.
     * @param in_begin Beginning offset of range.
     * @param in_size Size of range.
     */
  FIFileRange(
    std::filesystem::path     in_file_path,
    iterator::difference_type in_begin,
    iterator::difference_type in_size)
    : m_file_path(std::move(in_file_path)), m_begin(in_begin),
      m_end(in_begin + in_size)
  {}

private:
  /**
   * @brief Open the file stream if not already open.
   */
  void
    open()
  {
    if (!m_file_stream) {
      m_file_stream.open(m_file_path, std::ios::binary);
    }
  }
  std::filesystem::path   m_file_path   = {};
  std::ifstream           m_file_stream = {};
  std::ifstream::off_type m_begin       = {};
  std::ifstream::off_type m_end         = {};
};

/**
 * Verify that the iterator type for FIFileRange meets the requirements of a
 * random access iterator.
 */
static_assert(std::random_access_iterator<FIFileRange::iterator>);

/**
 * Verify that the type for FIFileRange meets the requirements of a random
 * access range.
 */
static_assert(std::ranges::random_access_range<FIFileRange>);
}// namespace open_viii::archive
#endif// OPENVIII_CPP_WIP_FIFILERANGE_HPP
