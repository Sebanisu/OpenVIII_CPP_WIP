//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_FILESECTION_HPP
#define VIIIARCHIVE_FILESECTION_HPP
#include "FileSectionTypeT.hpp"
#include <cinttypes>
#include <iostream>
#include <istream>
#include <string>
#include <utility>
namespace open_viii {
/**
 * Collection of data related to a file section of a pak file.
 */
struct FileSection
{
private:
  /**
   * Output FileName
   */
  std::string m_file_name{};
  /**
   * Frame Count
   */
  std::uint32_t m_frames{};
  /**
   * Location of Data
   */
  std::int64_t m_offset{};
  /**
   * Size of Data
   */
  std::uint32_t m_size{};
  /**
   * Type of file in Section
   */
  std::string_view m_type{};

public:
  constexpr FileSection() = default;
  FileSection(std::string             in_file_name,
              const std::uint32_t &   in_frames,
              const std::int64_t &    in_offset,
              const std::uint32_t &   in_size,
              const std::string_view &in_type)
    : m_file_name(std::move(in_file_name)),
      m_frames(in_frames),
      m_offset(in_offset),
      m_size(in_size),
      m_type(in_type)
  {}
  /**
   * Get Output FileName
   */
  [[nodiscard]] const auto &file_name() const noexcept
  {
    return m_file_name;
  }
  /**
   * Set Output FileName
   */
  void file_name(std::string in_file_name) noexcept
  {
    m_file_name = std::move(in_file_name);
  }
  /**
   * Get Frame Count
   */
  [[nodiscard]] constexpr auto frames() const noexcept
  {
    return m_frames;
  }
  /**
   * Get Mutable Frame Count
   */
  [[nodiscard]] auto &mutable_frames() noexcept
  {
    return m_frames;
  }
  /**
   * Set Frame Count
   */
  void frames(const std::uint32_t &in_frames) noexcept
  {
    m_frames = in_frames;
  }
  /**
   * Get Location of Data
   */
  [[nodiscard]] constexpr auto offset() const noexcept
  {
    return m_offset;
  }
  /**
   * Get Location of Data
   */
  void offset(const std::int64_t &in_offset) noexcept
  {
    m_offset = in_offset;
  }
  /**
   * Get Size of Data
   */
  [[nodiscard]] constexpr auto size() const noexcept
  {
    return m_size;
  }
  /**
   * Set Size of Data
   */
  void size(const std::uint32_t &in_size) noexcept
  {
    m_size = in_size;
  }
  /**
   * Get Type of file in Section
   */
  [[nodiscard]] constexpr auto type() const noexcept
  {
    return m_type;
  }
  /**
   * Set Type of file in Section
   */
  void type(const std::string_view &in_type) noexcept
  {
    m_type = in_type;
  }
  friend std::ostream &operator<<(std::ostream &     os,
                                  const FileSection &file_section)
  {
    if (!std::ranges::empty(file_section.file_name())
        && file_section.size() > 0) {
      return os << "|" << file_section.file_name() << "||"
                << file_section.frames() << "||" << std::hex
                << file_section.offset() << std::dec << "||"
                << file_section.size() << "||" << file_section.type() << '\n';
    }
    return os;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_FILESECTION_HPP
