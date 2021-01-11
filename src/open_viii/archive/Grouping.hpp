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
  mutable std::filesystem::path m_path{};
  mutable std::size_t m_offset{};
  mutable std::size_t m_size{};// if forced otherwise 0;
  mutable T m_data{};
  mutable std::string m_base{};
  mutable std::filesystem::path m_nested_path{};

public:
  [[nodiscard]] std::filesystem::path &path() const noexcept
  {
    return m_path;
  }
  [[nodiscard]] std::size_t &offset() const noexcept
  {
    return m_offset;
  }
  /**
   * Size of file
   * @return size_t
   */
  [[nodiscard]] std::size_t &size() const noexcept
  {
    if (m_size == 0U) {
      if (!std::ranges::empty(data())) {
        m_size = std::ranges::size(data());
      } else if (std::filesystem::exists(path())) {
        m_size = std::filesystem::file_size(path());
      }
    }
    return m_size;
  }
  [[nodiscard]] T &data() const noexcept
  {
    return m_data;
  }
  [[nodiscard]] std::string &base() const noexcept
  {
    return m_base;
  }
  [[nodiscard]] std::filesystem::path &nested_path() const noexcept
  {
    return m_nested_path;
  }

  //    void path(std::filesystem::path && value) const noexcept{ m_path =
  //    value; } void offset(std::size_t && value) const noexcept{ m_offset =
  //    value; } void size(std::size_t && value) const noexcept{ m_size =
  //    value; }// if forced otherwise 0; void data(T && value) const
  //    noexcept{ m_data = value; } void base(std::string && value) const
  //    noexcept{ m_base = value; } void nested_path(std::filesystem::path &&
  //    value) const noexcept{ m_nested_path = value; }


  void path(const std::filesystem::path &value) const noexcept
  {
    m_path = value;
  }
  void offset(const std::size_t &value) const noexcept
  {
    m_offset = value;
  }
  void size(const std::size_t &value) const noexcept
  {
    m_size = value;
  }
  void data(T &&value) const noexcept
  {
    m_data = std::move(value);
  }
  void base(const std::string &value) const noexcept
  {
    m_base = value;
  }
  void nested_path(const std::filesystem::path &value) const noexcept
  {
    m_nested_path = value;
  }

  //    // Assigns basename and returns it.
  [[maybe_unused]] void get_base_name() const noexcept
  {
    m_base = tools::get_base_name(m_path);
  }
  explicit operator bool() const
  {
    return (!std::ranges::empty(m_path) && std::filesystem::exists(m_path))
           || !std::ranges::empty(m_data)
           || (!std::ranges::empty(m_path) && (m_offset > 0 || m_size > 0));
  }
};
}// namespace open_viii::archive
#endif// VIIIARCHIVE_GROUPING_HPP
