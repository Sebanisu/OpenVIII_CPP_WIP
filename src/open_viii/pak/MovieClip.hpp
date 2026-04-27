//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_MOVIECLIP_HPP
#define VIIIARCHIVE_MOVIECLIP_HPP
#include "Cam.hpp"
#include "FileSection.hpp"
#include <fmt/format.h>
#include <utility>

namespace open_viii::pak {

struct MovieClip
{
  // Public data
  FileSection bink_high{};
  FileSection bink_low{};
  Cam         cam{};
  FileSection cam_fs{};

  void
    swap_bink() noexcept
  {
    std::swap(bink_high, bink_low);
  }
};

}// namespace open_viii::pak

// fmt formatter
template<>
struct fmt::formatter<open_viii::pak::MovieClip>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::pak::MovieClip &v, FormatContext &ctx) const
  {
    auto       out           = ctx.out();

    const auto write_section = [&](const auto &section) {
      if constexpr (requires { section.size(); }) {
        if (section.size() == 0)
          return;
      }

      out = fmt::format_to(out, "|-\n{}", section);
    };

    write_section(v.bink_high);
    write_section(v.bink_low);
    write_section(v.cam);

    return out;
  }
};
#endif// VIIIARCHIVE_MOVIECLIP_HPP
