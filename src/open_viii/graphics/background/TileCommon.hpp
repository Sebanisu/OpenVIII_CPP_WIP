//
// Created by pcvii on 6/7/2021.
//

#ifndef OPENVIII_CPP_WIP_TILECOMMON_HPP
#define OPENVIII_CPP_WIP_TILECOMMON_HPP
#include "BlendModeT.hpp"
#include "BPPT.hpp"
#include "open_viii/graphics/Rectangle.hpp"
#include <cstring>
#include <fmt/format.h>
#include <iomanip>
namespace open_viii::graphics::background {
template<typename tileT>
struct TileCommon;

template<typename tileT>
std::ostream &
  operator<<(std::ostream &os, const TileCommon<tileT> &tile);

enum class TileCommonConstants : std::uint16_t
{

  height             = 16U,
  width              = height,
  texture_page_width = 128U,
  area               = height * width,
};
template<typename tileT>
struct TileCommon : tileT
{
private:
  using tileT::m_animation_id;
  using tileT::m_animation_state;
  using tileT::m_blend_mode;
  using tileT::m_layer_id;
  using tileT::m_palette_id;
  using tileT::m_source_xy;
  using tileT::m_tex_id_buffer;
  using tileT::m_xy;
  using tileT::m_z;
  using source_type = typename decltype(m_source_xy)::value_type;
  using output_type = typename decltype(m_xy)::value_type;
  using this_type   = TileCommon<tileT>;
  using constants_type =
    typename std::underlying_type_t<decltype(TileCommonConstants::height)>;
  static this_type
    copy_tile(const std::vector<char> &buffer)
  {
    auto tile = this_type{};
    std::memcpy(
      &tile,
      std::data(buffer),
      (std::min)(sizeof(tileT), std::size(buffer)));
    return tile;
  }

public:
  using impl_type = tileT;
  TileCommon()    = default;
  explicit TileCommon(const std::vector<char> &buffer)
    : TileCommon(copy_tile(buffer))
  {}
  using tileT::EXPECTED_SIZE;
  using tileT::FORCE_TYPE_VALUES;
  auto
    operator<=>(const this_type &) const = default;
  template<typename return_type = constants_type>
  [[nodiscard]] static constexpr return_type
    height() noexcept
  {
    return static_cast<return_type>(TileCommonConstants::height);
  }
  template<typename return_type = constants_type>
  [[nodiscard]] static constexpr return_type
    width() noexcept
  {
    return static_cast<return_type>(TileCommonConstants::width);
  }
  template<typename return_type = constants_type>
  [[nodiscard]] static constexpr return_type
    texture_page_width(BPPT bpp = {}) noexcept
  {
    auto temp = static_cast<int>(TileCommonConstants::texture_page_width);
    if (bpp.bpp8())
      return static_cast<return_type>(temp);
    if (bpp.bpp16())
      return static_cast<return_type>(temp / 2);
    // bpp4 or default
    return static_cast<return_type>(temp * 2);
  }
  template<typename return_type = constants_type>
  [[nodiscard]] static constexpr return_type
    area() noexcept
  {
    return static_cast<return_type>(TileCommonConstants::area);
  }
  [[nodiscard]] constexpr this_type
    with_source_xy(decltype(m_source_xy) in_source_xy) const noexcept
  {
    auto out        = *this;
    out.m_source_xy = in_source_xy;
    return out;
  }
  [[nodiscard]] constexpr this_type
    with_source_xy(source_type in_source_x, source_type in_source_y)
      const noexcept
  {
    auto out        = *this;
    out.m_source_xy = out.m_source_xy.with_x(in_source_x);
    out.m_source_xy = out.m_source_xy.with_y(in_source_y);
    return out;
  }
  [[nodiscard]] constexpr auto
    source_xy() const noexcept
  {
    return m_source_xy;
  }
  [[nodiscard]] constexpr this_type
    with_xy(decltype(m_xy) in_xy) const noexcept
  {
    auto out = *this;
    out.m_xy = in_xy;
    return out;
  }
  [[nodiscard]] constexpr this_type
    with_xy(output_type in_x, output_type in_y) const noexcept
  {
    auto out = *this;
    out.m_xy = decltype(m_xy){ in_x, in_y };
    return out;
  }
  [[nodiscard]] constexpr this_type
    shift_xy(decltype(m_xy) in_xy) const noexcept
  {
    auto out = *this;
    out.m_xy += in_xy;
    return out;
  }
  [[nodiscard]] constexpr this_type
    shift_xy(output_type in_x, output_type in_y) const noexcept
  {
    auto out = *this;
    out.m_xy += decltype(m_xy){ in_x, in_y };
    return out;
  }
  [[nodiscard]] constexpr auto
    xy() const noexcept
  {
    return m_xy;
  }
  [[nodiscard]] constexpr this_type
    with_x(output_type in_x) const noexcept
  {
    auto out = *this;
    out.m_xy = m_xy.with_x(in_x);
    return out;
  }
  [[nodiscard]] constexpr auto
    x() const noexcept
  {
    return m_xy.x();
  }
  [[nodiscard]] constexpr this_type
    with_y(output_type in_y) const noexcept
  {
    auto out = *this;
    out.m_xy = m_xy.with_y(in_y);
    return out;
  }
  [[nodiscard]] constexpr auto
    y() const noexcept
  {
    return m_xy.y();
  }
  [[nodiscard]] constexpr this_type
    with_z(decltype(m_z) in_z) const noexcept
  {
    auto out = *this;
    out.m_z  = in_z;
    return out;
  }
  [[nodiscard]] constexpr auto &
    z() const noexcept
  {
    return m_z;
  }
  [[nodiscard]] constexpr this_type
    with_texture_id(decltype(m_tex_id_buffer.id()) in_texture_id) const noexcept
  {
    auto out            = *this;
    out.m_tex_id_buffer = m_tex_id_buffer.with_id(in_texture_id);
    return out;
  }
  [[nodiscard]] constexpr auto
    texture_id() const noexcept
  {
    return m_tex_id_buffer.id();
  }
  [[nodiscard]] constexpr this_type
    with_blend(decltype(m_tex_id_buffer.blend()) in_blend) const noexcept
  {
    auto out            = *this;
    out.m_tex_id_buffer = m_tex_id_buffer.with_blend(in_blend);
    return out;
  }
  [[nodiscard]] constexpr auto
    blend() const noexcept
  {
    return m_tex_id_buffer.blend();
  }
  [[nodiscard]] constexpr this_type
    with_depth(decltype(m_tex_id_buffer.depth()) in_depth) const noexcept
  {
    auto out            = *this;
    out.m_tex_id_buffer = m_tex_id_buffer.with_depth(in_depth);
    return out;
  }
  [[nodiscard]] constexpr auto
    depth() const noexcept
  {
    return m_tex_id_buffer.depth();
  }
  [[nodiscard]] constexpr this_type
    with_draw(decltype(m_tex_id_buffer.draw()) in_draw) const noexcept
  {
    auto out            = *this;
    out.m_tex_id_buffer = m_tex_id_buffer.with_draw(in_draw);
    return out;
  }
  [[nodiscard]] constexpr bool
    draw() const noexcept
  {
    return m_tex_id_buffer.draw();
  }
  [[nodiscard]] constexpr this_type
    with_palette_id(decltype(m_palette_id.id()) in_palette_id) const noexcept
  {
    auto out         = *this;
    // const auto old_id = palette_id();
    out.m_palette_id = m_palette_id.with_id(in_palette_id);
    // const auto new_id = out.palette_id();
    //    if (old_id != new_id) {
    //      std::cout << +old_id << +new_id << std::endl;
    //    }
    return out;
  }
  [[nodiscard]] constexpr auto
    palette_id() const noexcept
  {
    return m_palette_id.id();
  }
  [[nodiscard]] constexpr this_type
    with_source_x(source_type in_source_x) const noexcept
  {
    auto out        = *this;
    out.m_source_xy = m_source_xy.with_x(in_source_x);
    return out;
  }
  [[nodiscard]] constexpr auto
    source_x() const noexcept
  {
    return m_source_xy.x();
  }
  [[nodiscard]] constexpr this_type
    with_source_y(source_type in_source_y) const noexcept
  {
    auto out        = *this;
    out.m_source_xy = m_source_xy.with_y(in_source_y);
    return out;
  }
  [[nodiscard]] constexpr auto
    source_y() const noexcept
  {
    return m_source_xy.y();
  }
  [[nodiscard]] constexpr this_type
    with_layer_id(decltype(m_layer_id.id()) in_layer_id) const noexcept
    requires(!std::is_const_v<decltype(this_type::m_layer_id)>)
  {
    auto out       = *this;
    out.m_layer_id = m_layer_id.with_id(in_layer_id);
    return out;
  }
  [[nodiscard]] constexpr auto
    layer_id() const noexcept
  {
    return m_layer_id.id();
  }
  [[nodiscard]] constexpr this_type
    with_blend_mode(decltype(m_blend_mode) in_blend_mode) const noexcept
    requires(!std::is_const_v<decltype(this_type::m_blend_mode)>)
  {
    auto out         = *this;
    out.m_blend_mode = in_blend_mode;
    return out;
  }
  [[nodiscard]] constexpr auto
    blend_mode() const noexcept
  {
    return m_blend_mode;
  }
  [[nodiscard]] constexpr this_type
    with_animation_id(decltype(m_animation_id) in_animation_id) const noexcept
    requires(!std::is_const_v<decltype(this_type::m_animation_id)>)
  {
    auto out           = *this;
    out.m_animation_id = in_animation_id;
    return out;
  }
  [[nodiscard]] constexpr auto
    animation_id() const noexcept
  {
    return m_animation_id;
  }
  [[nodiscard]] constexpr this_type
    with_animation_state(
      decltype(m_animation_state) in_animation_state) const noexcept
    requires(!std::is_const_v<decltype(this_type::m_animation_state)>)
  {
    auto out              = *this;
    out.m_animation_state = in_animation_state;
    return out;
  }
  [[nodiscard]] constexpr auto
    animation_state() const noexcept
  {
    return m_animation_state;
  }
  [[nodiscard]] constexpr auto
    source_rectangle() const noexcept
  {
    return Rectangle<source_type>{ m_source_xy.x(),
                                   m_source_xy.y(),
                                   width<source_type>(),
                                   height<source_type>() };
  }
  [[nodiscard]] constexpr auto
    output_rectangle() const noexcept
  {
    return Rectangle<output_type>{ m_xy.x(),
                                   m_xy.y(),
                                   width<output_type>(),
                                   height<output_type>() };
  }

  [[nodiscard]] std::string
    to_hex() const
  {
    std::array<std::uint8_t, sizeof(this_type)> raw{};
    std::memcpy(raw.data(), this, sizeof(this_type));

    std::string result;
    result.reserve(2U + raw.size() * 2U);

    result += "0x";

    for (const auto byte : raw) {
      fmt::format_to(std::back_inserter(result), "{:02X}", byte);
    }

    return result;
  }
};

/**
 * @brief Concept to determine if a type represents a tile.
 *
 * A type satisfies `is_tile` if it has a nested `impl_type` and is derived from
 * it.
 *
 * @tparam T The type to check.
 */
template<typename T>
concept is_tile = requires { typename std::remove_cvref_t<T>::impl_type; }
               && std::derived_from<
                    std::remove_cvref_t<T>,
                    typename std::remove_cvref_t<T>::impl_type>;

/**
 * @brief Concept to determine if a type is a range of tiles.
 *
 * A type satisfies `is_tiles` if it is a range and its elements satisfy
 * `is_tile`.
 *
 * @tparam T The type to check.
 */
template<typename T>
concept is_tiles = is_tile<std::ranges::range_value_t<std::remove_cvref_t<T>>>
                && std::ranges::range<std::remove_cvref_t<T>>;

/**
 * @brief Concept to determine if a type is a contiguous and sized range of
 * tiles.
 *
 * A type satisfies `is_contiguous_sized_tiles` if it meets the `is_tiles`
 * requirement and is both a contiguous range and a sized range.
 *
 * @tparam T The type to check.
 */
template<typename T>
concept is_contiguous_sized_tiles
  = is_tiles<std::remove_cvref_t<T>>
 && std::ranges::contiguous_range<std::remove_cvref_t<T>>
 && std::ranges::sized_range<std::remove_cvref_t<T>>;

/**
 * @brief Predicate that filters out sentinel/invalid tiles.
 *
 * A tile is considered valid when its x-coordinate does not match the
 * sentinel end marker value (`0x7FFF`).
 *
 * Intended for use with standard algorithms and ranges filters when
 * iterating tile collections that may contain terminator entries.
 */
struct NotInvalidTile
{
  /**
   * @brief Checks whether a tile is valid.
   *
   * @tparam T Tile type satisfying the `is_tile` concept.
   * @param tile Tile instance to evaluate.
   * @return `true` if the tile is not the sentinel/invalid tile.
   * @return `false` if the tile x-coordinate equals the sentinel value.
   */
  template<is_tile T>
  constexpr bool
    operator()(const T &tile) const noexcept
  {
    return (std::cmp_not_equal(tile.x(), s_end_x));
  }

private:
  /**
   * @brief Sentinel x-coordinate used to identify invalid/end tiles.
   */
  static constexpr std::uint16_t s_end_x = { 0x7FFFU };
};

}// namespace open_viii::graphics::background

template<typename tileT>
struct fmt::formatter<open_viii::graphics::background::TileCommon<tileT>>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(
      const open_viii::graphics::background::TileCommon<tileT> &tile,
      FormatContext                                            &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "\t  {}"
      ", Source: {}"
      ", Output: {}"
      ", Z: {}"
      ", Depth: {}"
      ", Palette ID: {}"
      ", Texture ID: {}"
      ", Layer ID: {}"
      ", Blend Mode: {}"
      ", Blend Other: {}"
      ", Animation ID: {}"
      ", Animation State: {}"
      ", Draw: {}",
      tile.to_hex(),
      tile.source_rectangle(),
      tile.output_rectangle(),
      tile.z(),
      tile.depth(),
      +tile.palette_id(),
      +tile.texture_id(),
      +tile.layer_id(),
      tile.blend_mode(),
      +tile.blend(),
      +tile.animation_id(),
      +tile.animation_state(),
      tile.draw());
  }
};
template<typename tileT>
inline std::ostream &
  open_viii::graphics::background::operator<<(
    std::ostream                                             &os,
    const open_viii::graphics::background::TileCommon<tileT> &tile)
{
  return os << fmt::format("{}", tile);
}
#endif// OPENVIII_CPP_WIP_TILECOMMON_HPP
