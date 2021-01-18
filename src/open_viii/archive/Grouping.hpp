//
// Created by pcvii on 1/11/2021.
//

#ifndef VIIIARCHIVE_GROUPING_HPP
#define VIIIARCHIVE_GROUPING_HPP

#include "FI.hpp"
#include "FL.hpp"
#include "FS.hpp"
namespace open_viii::archive {
template<std::ranges::contiguous_range T> struct [[maybe_unused]] Grouping
{
private:
  std::filesystem::path m_path{};
  std::size_t m_offset{};
  std::size_t m_size{};// if forced otherwise 0;
  T m_data{};
  std::string m_base{};
  std::filesystem::path m_nested_path{};

public:
  /**
   * get path to file containing archive
   * @return
   */
  [[nodiscard]] const std::filesystem::path &path() const noexcept
  {
    return m_path;
  }
  /**
   * set path to file containing archive
   * @param value
   */
  void path(const std::filesystem::path &value) noexcept
  {
    m_path = value;
  }
  /**
   * get offset in bytes to start
   * @return
   */
  [[nodiscard]] const std::size_t &offset() const noexcept
  {
    return m_offset;
  }
  /**
   * set offset in bytes to start
   * @return
   */
  void offset(const std::size_t &value) noexcept
  {
    m_offset = value;
  }
  /**
   * get Size of file / also defaults size if value is 0.
   * @return size_t
   */
  [[nodiscard]] std::size_t size() const noexcept
  {
    if (m_size == 0U) {
      if (!std::ranges::empty(data())) {
        return std::ranges::size(data());
      } else if (std::filesystem::exists(path())) {
        return std::filesystem::file_size(path());
      }
    }
    return m_size;
  }
  /**
   * set Size of file
   * @param value
   */
  void size(const std::size_t &value) noexcept
  {
    m_size = value;
  }
  /**
   * get loaded data buffer
   * @return
   */
  [[nodiscard]] const T &data() const noexcept
  {
    return m_data;
  }
  /**
   * set data buffer
   * @param value
   */
  T & data(T &&value) noexcept
  {
    return m_data = std::move(value);
  }
  /**
   * stem of filename upper cased
   * @return
   */
  [[nodiscard]] const std::string &base() const noexcept
  {
    return m_base;
  }
  /**
   * set base name
   * @param value
   */
  void base(const std::string &value) noexcept
  {
    m_base = value;
  }
  /**
   * set base name = stem of path
   */
  [[maybe_unused]] void get_base_name() noexcept
  {
    base(tools::get_base_name(m_path));
  }
  /**
   * get path inside file
   * @return
   */
  [[nodiscard]] const std::filesystem::path &nested_path() const noexcept
  {
    return m_nested_path;
  }
  /**
   * set path inside file
   */
  void nested_path(const std::filesystem::path &value) noexcept
  {
    m_nested_path = value;
  }
  /**
   * convert to bool
   * @return true means value is set and loaded.
   */
  explicit operator bool() const
  {
    return (!std::ranges::empty(m_path) && std::filesystem::exists(m_path))
           || !std::ranges::empty(m_data)
           || (!std::ranges::empty(m_path) && (m_offset > 0 || m_size > 0));
  }
};
}// namespace open_viii::archive
#endif// VIIIARCHIVE_GROUPING_HPP
