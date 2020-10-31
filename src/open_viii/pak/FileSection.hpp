//
// Created by pcvii on 10/30/2020.
//

#ifndef VIIIARCHIVE_FILESECTION_HPP
#define VIIIARCHIVE_FILESECTION_HPP
#include "FileSectionType.hpp"
#include <cinttypes>
#include <iostream>
#include <string>

namespace open_viii {
struct FileSection
{
public:
  /// <summary>
  /// Output FileName;
  /// </summary>
  std::string FileName{};

  /// <summary>
  /// Frame Count
  /// </summary>
  std::uint32_t Frames{};

  /// <summary>
  /// Location of Data
  /// </summary>
  std::int64_t Offset{};

  /// <summary>
  /// Size of Data
  /// </summary>
  std::uint32_t Size{};

  /// <summary>
  /// Type of file in Section
  /// </summary>
  std::string_view Type{};

  friend std::ostream &operator<<(std::ostream &os, const FileSection &file_section)
  {
    return os << "(Filename)=" << file_section.FileName << ", (Frames)=" << file_section.Frames
              << ", (Offset)=" << file_section.Offset << ", (Size)=" << file_section.Size
              << ", (Type)=" << file_section.Type << '\n';
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_FILESECTION_HPP
