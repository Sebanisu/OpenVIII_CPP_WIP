//
// Created by pcvii on 12/27/2020.
//
#ifndef VIIIARCHIVE_CAM_HPP
#define VIIIARCHIVE_CAM_HPP
#include "CamFrame.hpp"
#include "CamHeader.hpp"
#include "FileSection.hpp"
#include "open_viii/tools/Tools.hpp"
#include "tl/write.hpp"
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <vector>

namespace open_viii::pak {

struct Cam
{
  // Public data
  CamHeader             header{};
  std::vector<CamFrame> frames{};

  Cam() = default;

  Cam(std::istream &is, const std::size_t &frame_count)
    : header(tools::read_val<CamHeader>(is))
  {
    frames.resize(frame_count);
    tools::read_val(is, frames);
  }

  Cam(std::istream &is, const FileSection &fs)
    : Cam(
        [&fs](std::istream &in_is) -> std::istream & {
          in_is.seekg(fs.offset);
          return in_is;
        }(is),
        fs.frames)
  {}

  explicit Cam(std::istream &is)
    : Cam(is, [&is]() {
        const auto cur = is.tellg();
        is.seekg(0, std::ios::end);
        const auto sz = is.tellg() - cur;
        is.seekg(cur, std::ios::beg);
        return static_cast<std::size_t>(sz) / sizeof(CamFrame);
      }())
  {}

  // Iteration helpers
  auto
    begin() const noexcept
  {
    return frames.begin();
  }
  auto
    end() const noexcept
  {
    return frames.end();
  }
  auto
    size() const noexcept
  {
    return frames.size();
  }

  // Immutable-style modifier
  [[nodiscard]] Cam
    with_frames(std::vector<CamFrame> &&in_frames) const noexcept
  {
    Cam ret{};
    ret.header
      = header.with_count(static_cast<std::uint16_t>(in_frames.size()));
    ret.frames = std::move(in_frames);
    return ret;
  }

  void
    save(const std::filesystem::path &path) const
  {
    std::ofstream fp(path, std::ios::out | std::ios::trunc | std::ios::binary);
    tl::write::append(fp, header);
    tl::write::append(fp, frames);
  }
};

}// namespace open_viii::pak

// fmt formatter
template<>
struct fmt::formatter<open_viii::pak::Cam>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::pak::Cam &v, FormatContext &ctx) const
  {
    auto out   = ctx.out();

    // header
    out        = fmt::format_to(out, "{}\n{{", v.header);

    // frames
    bool first = true;
    for (const auto &f : v.frames) {
      if (!first)
        out = fmt::format_to(out, ",");
      first = false;

      out   = fmt::format_to(out, "{}\n", f);
    }

    return fmt::format_to(out, "}}");
  }
};
#endif// VIIIARCHIVE_CAM_HPP
