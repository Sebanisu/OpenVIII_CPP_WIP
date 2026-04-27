//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_FILESECTION_HPP
#define VIIIARCHIVE_FILESECTION_HPP
#include "FileSectionTypeT.hpp"
#include <cstdint>
#include <fmt/format.h>
#include <string>
#include <string_view>

namespace open_viii::pak {

/**
 * Collection of data related to a file section of a pak file.
 */
struct FileSection
{
  // Public data
  std::string      file_name{};
  std::uint32_t    frames{};
  std::int64_t     offset{};
  std::uint32_t    size{};
  std::string_view type{};

  constexpr FileSection() = default;

  FileSection(
    std::string      in_file_name,
    std::uint32_t    in_frames,
    std::int64_t     in_offset,
    std::uint32_t    in_size,
    std::string_view in_type)
    : file_name(std::move(in_file_name)), frames(in_frames), offset(in_offset),
      size(in_size), type(in_type)
  {}
};

}// namespace open_viii::pak

// fmt formatter
template<>
struct fmt::formatter<open_viii::pak::FileSection>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::pak::FileSection &v, FormatContext &ctx) const
  {
    // Preserve original behavior: only print if valid
    if (!v.file_name.empty() && v.size > 0) {
      return fmt::format_to(
        ctx.out(),
        "|{}||{}||{:x}||{}||{}\n",
        v.file_name,
        v.frames,
        v.offset,
        v.size,
        v.type);
    }
    return ctx.out();
  }
};
#endif// VIIIARCHIVE_FILESECTION_HPP
