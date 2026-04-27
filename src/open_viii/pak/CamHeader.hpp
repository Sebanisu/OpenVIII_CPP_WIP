//
// Created by pcvii on 12/31/2020.
//
#ifndef VIIIARCHIVE_CAMHEADER_HPP
#define VIIIARCHIVE_CAMHEADER_HPP
#include <array>
#include <cstdint>
#include <fmt/format.h>
#include <ranges>
#include <string_view>
#include <type_traits>

namespace open_viii::pak {

struct CamHeader
{
  // Public data (trivial access)
  std::array<char, 3U>              start{ 'F', '8', 'P' };
  std::array<char, 3U>              unk{};
  std::uint16_t                     count{};

  static constexpr std::string_view EXPECTED_START{ "F8P" };
  static constexpr auto             EXPECTED_SIZE{ 8U };

  constexpr CamHeader() = default;

  [[nodiscard]] constexpr bool
    valid_start() const noexcept
  {
    return std::ranges::equal(start, EXPECTED_START);
  }

  // Immutable-style modifiers
  [[nodiscard]] constexpr CamHeader
    with_start(std::array<char, 3U> v) const noexcept
  {
    auto ret  = *this;
    ret.start = v;
    return ret;
  }

  [[nodiscard]] constexpr CamHeader
    with_unk(std::array<char, 3U> v) const noexcept
  {
    auto ret = *this;
    ret.unk  = v;
    return ret;
  }

  [[nodiscard]] constexpr CamHeader
    with_count(std::uint16_t v) const noexcept
  {
    auto ret  = *this;
    ret.count = v;
    return ret;
  }
};

static_assert(CamHeader().valid_start());
static_assert(std::is_trivially_copyable_v<CamHeader>);
static_assert(std::is_default_constructible_v<CamHeader>);
static_assert(sizeof(CamHeader) == CamHeader::EXPECTED_SIZE);

}// namespace open_viii::pak

// fmt formatter
template<>
struct fmt::formatter<open_viii::pak::CamHeader>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const open_viii::pak::CamHeader &v, FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{{}{}{},{}{}{},{} }}",
      v.start[0],
      v.start[1],
      v.start[2],
      v.unk[0],
      v.unk[1],
      v.unk[2],
      v.count);
  }
};
#endif// VIIIARCHIVE_CAMHEADER_HPP
