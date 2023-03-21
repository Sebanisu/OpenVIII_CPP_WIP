//
// Created by pcvii on 3/21/2023.
//

#ifndef OPENVIII_CPP_WIP_FIFLFSARCHIVEITERATOR_HPP
#define OPENVIII_CPP_WIP_FIFLFSARCHIVEITERATOR_HPP
#include "FIFLFS_sentinel.hpp"
#include "FIFLFS.hpp"
namespace open_viii::archive {
class FIFLFSArchiveFetcher
{
public:
  using value_type = FIFLFS<false>;
  FIFLFSArchiveFetcher(
    std::string_view    map_name,
    const FIFLFS<true> &fiflfs_true)
    : m_map_name(map_name), m_fiflfs_true(fiflfs_true)
  {}

  FIFLFSArchiveFetcher(const FIFLFSArchiveFetcher &other)
    : m_map_name(other.m_map_name), m_fiflfs_true(other.m_fiflfs_true)
  {}

  FIFLFSArchiveFetcher &
    operator=(const FIFLFSArchiveFetcher &other)
  {
    if (this != &other) {
      m_map_name    = other.m_map_name;
      m_fiflfs_true = other.m_fiflfs_true;
    }
    return *this;
  }
  operator value_type()
  {
    return get();
  };
  value_type
    get() const
  {
    return m_fiflfs_true.get().get_archive_with_nested(
      { m_map_name },
      [](auto &&) {
        return true;
      });
  }

private:
  std::string_view                           m_map_name;
  std::reference_wrapper<const FIFLFS<true>> m_fiflfs_true;
};
class FIFLFSArchiveIterator
{
public:
  using value_type        = FIFLFSArchiveFetcher;
  using difference_type   = std::ptrdiff_t;
  //  using pointer           = open_viii::archive::FIFLFS<false> *;
  //  using reference         = open_viii::archive::FIFLFS<false> &;
  using iterator_category = std::random_access_iterator_tag;
  // using const_iterator    = FIFLFSArchiveIterator;
  FIFLFSArchiveIterator() = default;
  FIFLFSArchiveIterator(
    const open_viii::archive::FIFLFS<true> &archive,
    std::size_t                             index = 0)
    : m_fiflfs_true(archive), m_map_names(m_fiflfs_true.get().map_data()),
      m_current_index(index)
  {}
  bool
    operator==(const FIFLFSArchiveIterator &other) const
  {
    return &m_fiflfs_true.get() == &other.m_fiflfs_true.get()
        && m_current_index == other.m_current_index
        && m_map_names == other.m_map_names;
  }
  bool
    operator==(const FIFLFS_sentinel) const
  {
    return m_current_index == std::ranges::size(m_map_names);
  }

  FIFLFSArchiveIterator &
    operator+=(difference_type n)
  {
    m_current_index = static_cast<std::size_t>(
      static_cast<difference_type>(m_current_index) + n);
    return *this;
  }

  friend FIFLFSArchiveIterator
    operator+(FIFLFSArchiveIterator it, difference_type n)
  {
    return it += n;
  }

  friend FIFLFSArchiveIterator
    operator+(difference_type n, FIFLFSArchiveIterator it)
  {
    return it += n;
  }

  FIFLFSArchiveIterator &
    operator-=(difference_type n)
  {
    m_current_index -= static_cast<std::size_t>(
      static_cast<difference_type>(m_current_index) - n);
    return *this;
  }

  friend FIFLFSArchiveIterator
    operator-(FIFLFSArchiveIterator it, difference_type n)
  {
    return it -= n;
  }

  friend difference_type
    operator-(
      FIFLFSArchiveIterator const &lhs,
      FIFLFSArchiveIterator const &rhs)
  {
    return static_cast<difference_type>(lhs.m_current_index)
         - static_cast<difference_type>(rhs.m_current_index);
  }

  friend difference_type
    operator-(FIFLFS_sentinel const &, FIFLFSArchiveIterator const &rhs)
  {
    return static_cast<difference_type>(rhs.m_map_names.size())
         - static_cast<difference_type>(rhs.m_current_index);
  }
  friend difference_type
    operator-(FIFLFSArchiveIterator const &lhs, FIFLFS_sentinel const &)
  {
    return static_cast<difference_type>(lhs.m_current_index)
         - static_cast<difference_type>(lhs.m_map_names.size());
  }

  value_type
    operator[](difference_type n) const
  {
    return *(*this + n);
  }

  FIFLFSArchiveIterator &
    operator++()
  {
    ++m_current_index;
    return *this;
  }

  FIFLFSArchiveIterator
    operator++(int)
  {
    FIFLFSArchiveIterator old = *this;
    ++(*this);
    return old;
  }
  FIFLFSArchiveIterator &
    operator--()
  {
    --m_current_index;
    return *this;
  }

  FIFLFSArchiveIterator
    operator--(int)
  {
    FIFLFSArchiveIterator old = *this;
    --(*this);
    return old;
  }

  const std::string &
    operator+() const
  {
    return m_map_names[m_current_index];
  }

  value_type
    operator*() const
  {
    return { operator+(), m_fiflfs_true };
  }
  bool
    operator<(const FIFLFSArchiveIterator &other) const
  {
    return &m_fiflfs_true.get() == &other.m_fiflfs_true.get()
        && m_current_index < other.m_current_index;
  }

  bool
    operator<=(const FIFLFSArchiveIterator &other) const
  {
    return *this == other || *this < other;
  }

  bool
    operator>(const FIFLFSArchiveIterator &other) const
  {
    return !(*this <= other);
  }

  bool
    operator>=(const FIFLFSArchiveIterator &other) const
  {
    return !(*this < other);
  }
  bool
    operator<(const FIFLFS_sentinel &) const
  {
    return m_current_index < m_map_names.size();
  }

  bool
    operator<=(const FIFLFS_sentinel &other) const
  {
    return *this == other || *this < other;
  }

  bool
    operator>(const FIFLFS_sentinel &other) const
  {
    return !(*this <= other);
  }

  bool
    operator>=(const FIFLFS_sentinel &other) const
  {
    return !(*this < other);
  }

private:
  static const inline FIFLFS<true>           m_tmp{};
  std::reference_wrapper<const FIFLFS<true>> m_fiflfs_true{ m_tmp };
  std::vector<std::string>                   m_map_names{};
  std::size_t                                m_current_index{};
};
template<>
inline typename FIFLFS<true>::iterator
  FIFLFS<true>::begin() const
{
  return typename FIFLFS<true>::iterator{ *this };
}
template<>
inline typename FIFLFS<true>::iterator
  FIFLFS<true>::cbegin() const
{
  return typename FIFLFS<true>::iterator{ *this };
}
static_assert(std::movable<FIFLFSArchiveFetcher>);
static_assert(std::copyable<FIFLFSArchiveFetcher>);
static_assert(std::movable<FIFLFSArchiveIterator>);
static_assert(std::copyable<FIFLFSArchiveIterator>);
static_assert(std::random_access_iterator<FIFLFSArchiveIterator>);
static_assert(std::sentinel_for<FIFLFS_sentinel, FIFLFSArchiveIterator>);
static_assert(std::sized_sentinel_for<FIFLFS_sentinel, FIFLFSArchiveIterator>);
static_assert(std::ranges::random_access_range<FIFLFS<true>>);
static_assert(std::ranges::sized_range<FIFLFS<true>>);

}

#endif// OPENVIII_CPP_WIP_FIFLFSARCHIVEITERATOR_HPP
