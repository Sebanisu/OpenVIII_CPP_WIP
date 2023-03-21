//
// Created by pcvii on 3/21/2023.
//

#ifndef OPENVIII_CPP_WIP_FIFLFSFILEITERATOR_HPP
#define OPENVIII_CPP_WIP_FIFLFSFILEITERATOR_HPP
#include "FIFLFS.hpp"
#include "FIFLFS_sentinel.hpp"
namespace open_viii::archive {
template<bool is_nested>
class FIFLFSFileFetcher
{
public:
  using value_type = std::vector<char>;
  FIFLFSFileFetcher(
    std::uint32_t            file_id,
    std::string_view         map_name,
    const FIFLFS<is_nested> &fiflfs_false)
    : m_file_id(file_id), m_file_name(map_name), m_fiflfs_false(fiflfs_false)
  {}
  FIFLFSFileFetcher(
    const std::pair<std::uint32_t, std::string> &pair,
    const FIFLFS<is_nested>                     &fiflfs_false)
    : m_file_id(pair.first), m_file_name(pair.second),
      m_fiflfs_false(fiflfs_false)
  {}

  FIFLFSFileFetcher(const FIFLFSFileFetcher &other)
    : m_file_id(other.m_file_id), m_file_name(other.m_file_name),
      m_fiflfs_false(other.m_fiflfs_false)
  {}

  FIFLFSFileFetcher &
    operator=(const FIFLFSFileFetcher &other)
  {
    if (this != &other) {
      m_file_id      = other.m_file_id;
      m_file_name    = other.m_file_name;
      m_fiflfs_false = other.m_fiflfs_false;
    }
    return *this;
  }
  operator value_type()
  {
    return get();
  };
  FI
    get_file_info() const
  {
    return m_fiflfs_false.get().get_entry_by_index(m_file_id);
  }
  value_type
    get() const
  {
    return m_fiflfs_false.get().get_entry_buffer(get_file_info());
  }

private:
  std::uint32_t                                   m_file_id;
  std::string_view                                m_file_name;
  std::reference_wrapper<const FIFLFS<is_nested>> m_fiflfs_false;
};
template<bool is_nested>
class FIFLFSFileIterator
{
public:
  using value_type        = FIFLFSFileFetcher<is_nested>;
  using difference_type   = std::ptrdiff_t;
  using iterator_category = std::random_access_iterator_tag;
  // using const_iterator    = FIFLFS_Iterator<false>;

  FIFLFSFileIterator()    = default;
  FIFLFSFileIterator(
    const open_viii::archive::FIFLFS<is_nested> &archive,
    std::size_t                                  index = 0)
    : m_fiflfs_false(archive),
      m_pair_id_names(m_fiflfs_false.get().get_all_pairs_from_fl({})),
      m_current_index(index)
  {}
  bool
    operator==(const FIFLFSFileIterator &other) const
  {
    return &m_fiflfs_false.get() == &other.m_fiflfs_false.get()
        && m_current_index == other.m_current_index
        && m_pair_id_names == other.m_pair_id_names;
  }
  bool
    operator==(const FIFLFS_sentinel) const
  {
    return m_current_index == std::ranges::size(m_pair_id_names);
  }

  FIFLFSFileIterator &
    operator+=(difference_type n)
  {
    m_current_index = static_cast<std::size_t>(
      static_cast<difference_type>(m_current_index) + n);
    return *this;
  }

  friend FIFLFSFileIterator
    operator+(FIFLFSFileIterator it, difference_type n)
  {
    return it += n;
  }

  friend FIFLFSFileIterator
    operator+(difference_type n, FIFLFSFileIterator it)
  {
    return it += n;
  }

  FIFLFSFileIterator &
    operator-=(difference_type n)
  {
    m_current_index = static_cast<std::size_t>(
      static_cast<difference_type>(m_current_index) - n);
    return *this;
  }

  friend FIFLFSFileIterator
    operator-(FIFLFSFileIterator it, difference_type n)
  {
    return it -= n;
  }

  friend difference_type
    operator-(FIFLFSFileIterator const &lhs, FIFLFSFileIterator const &rhs)
  {
    return static_cast<difference_type>(lhs.m_current_index)
         - static_cast<difference_type>(rhs.m_current_index);
  }
  friend difference_type
    operator-(FIFLFS_sentinel const &, FIFLFSFileIterator const &rhs)
  {
    return static_cast<difference_type>(rhs.m_pair_id_names.size())
         - static_cast<difference_type>(rhs.m_current_index);
  }
  friend difference_type
    operator-(FIFLFSFileIterator const &lhs, FIFLFS_sentinel const &)
  {
    return static_cast<difference_type>(lhs.m_current_index)
         - static_cast<difference_type>(lhs.m_pair_id_names.size());
  }
  value_type
    operator[](difference_type n) const
  {
    return *(*this + n);
  }

  FIFLFSFileIterator &
    operator++()
  {
    ++m_current_index;
    return *this;
  }

  FIFLFSFileIterator
    operator++(int)
  {
    FIFLFSFileIterator old = *this;
    ++(*this);
    return old;
  }
  FIFLFSFileIterator &
    operator--()
  {
    --m_current_index;
    return *this;
  }

  FIFLFSFileIterator
    operator--(int)
  {
    FIFLFSFileIterator old = *this;
    --(*this);
    return old;
  }

  const std::pair<std::uint32_t, std::string> &
    operator+() const
  {
    return m_pair_id_names[m_current_index];
  }

  value_type
    operator*() const
  {
    return { m_pair_id_names[m_current_index], m_fiflfs_false };
  }

  bool
    operator<(const FIFLFSFileIterator &other) const
  {
    return &m_fiflfs_false.get() == &other.m_fiflfs_false.get()
        && m_current_index < other.m_current_index;
  }

  bool
    operator<=(const FIFLFSFileIterator &other) const
  {
    return *this == other || *this < other;
  }

  bool
    operator>(const FIFLFSFileIterator &other) const
  {
    return !(*this <= other);
  }

  bool
    operator>=(const FIFLFSFileIterator &other) const
  {
    return !(*this < other);
  }

  bool
    operator<(const FIFLFS_sentinel &) const
  {
    return m_current_index < m_pair_id_names.size();
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
  static const inline FIFLFS<is_nested>              m_tmp{};
  std::reference_wrapper<const FIFLFS<is_nested>>    m_fiflfs_false{ m_tmp };
  std::vector<std::pair<std::uint32_t, std::string>> m_pair_id_names{};
  std::size_t                                        m_current_index{};
};
template<>
inline typename FIFLFS<false>::iterator
  FIFLFS<false>::begin() const
{
  return typename FIFLFS<false>::iterator{ *this };
}
template<>
inline typename FIFLFS<false>::iterator
  FIFLFS<false>::cbegin() const
{
  return typename FIFLFS<false>::iterator{ *this };
}

static_assert(std::movable<FIFLFSFileFetcher<true>>);
static_assert(std::movable<FIFLFSFileFetcher<false>>);
static_assert(std::movable<FIFLFSFileIterator<false>>);
static_assert(std::movable<FIFLFSFileIterator<true>>);
static_assert(std::copyable<FIFLFSFileFetcher<true>>);
static_assert(std::copyable<FIFLFSFileFetcher<false>>);
static_assert(std::copyable<FIFLFSFileIterator<false>>);
static_assert(std::copyable<FIFLFSFileIterator<true>>);
static_assert(std::random_access_iterator<FIFLFSFileIterator<false>>);
static_assert(std::random_access_iterator<FIFLFSFileIterator<true>>);
static_assert(std::sentinel_for<FIFLFS_sentinel, FIFLFSFileIterator<false>>);
static_assert(std::sentinel_for<FIFLFS_sentinel, FIFLFSFileIterator<true>>);
static_assert(
  std::sized_sentinel_for<FIFLFS_sentinel, FIFLFSFileIterator<false>>);

static_assert(std::ranges::random_access_range<FIFLFS<false>>);
static_assert(std::ranges::sized_range<FIFLFS<false>>);
static_assert(std::ranges::random_access_range<std::invoke_result_t<
                decltype(&FIFLFS<true>::file_range),
                const FIFLFS<true> *>>);
static_assert(std::ranges::sized_range<std::invoke_result_t<
                decltype(&FIFLFS<true>::file_range),
                const FIFLFS<true> *>>);
}// namespace open_viii::archive
#endif// OPENVIII_CPP_WIP_FIFLFSFILEITERATOR_HPP
