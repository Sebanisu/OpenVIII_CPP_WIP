#ifndef CC1E517B_6340_4FF2_B4A0_BE029215A6E6
#define CC1E517B_6340_4FF2_B4A0_BE029215A6E6
#include "Map.hpp"
#include <fmt/format.h>
#include <limits>
#include <ostream>

namespace open_viii::graphics::background {
/**
 * @brief Represents a normalized tile with various properties.
 *
 * This class defines a normalized representation of a source tile with texture,
 * palette, and positional data. It includes properties for layer
 * identification, blending mode, and animation details. It excludes:
 * - **Destination X, Y, Z**: These are excluded since similar/duplicate tiles
 * do not rely on specific destination coordinates.
 */
struct NormalizedSourceTile
{
public:
  using impl_type = NormalizedSourceTile;
  TexIdBuffer m_tex_id_buffer
    = {};///< Buffer holding texture ID, blending, depth, and draw flags.
  PaletteID m_palette_id
    = {};///< Identifier for the palette associated with the tile.
  Point<std::uint8_t> m_source_xy
    = {};///< Source position (x, y) of the tile within the texture.
  LayerID    m_layer_id = {};///< Identifier for the layer this tile belongs to.
  BlendModeT m_blend_mode = BlendModeT::none;///< Blending mode of the tile.
  std::uint8_t m_animation_id
    = (std::numeric_limits<std::uint8_t>::
         max)();///< Identifier for the animation (default: no animation).
  std::uint8_t m_animation_state = {};///< Current animation state of the tile.

  /// @brief Default constructor.
  constexpr NormalizedSourceTile() = default;

  /// @brief Compares two tiles for equality or ordering.
  constexpr auto
    operator<=>(const NormalizedSourceTile &) const = default;

  /**
   * @brief Constructs a normalized tile from a generic tile object.
   *
   * @tparam Tile The type of the tile object, requiring methods to access
   * properties like texture ID, source position, etc.
   * @param tile The tile object to initialize this normalized tile from.
   */
  constexpr NormalizedSourceTile(const is_tile auto &tile)
  {
    m_tex_id_buffer = m_tex_id_buffer.with_id(tile.texture_id())
                        .with_blend(tile.blend())
                        .with_depth(tile.depth())
                        .with_draw(tile.draw());
    m_palette_id    = m_palette_id.with_id(tile.palette_id());
    assert(
      std::cmp_less(
        tile.source_x(),
        (std::numeric_limits<std::uint8_t>::max)()));
    assert(
      std::cmp_less(
        tile.source_y(),
        (std::numeric_limits<std::uint8_t>::max)()));
    m_source_xy
      = Point<std::uint8_t>{ static_cast<std::uint8_t>(tile.source_x()),
                             static_cast<std::uint8_t>(tile.source_y()) };
    m_layer_id        = m_layer_id.with_id(tile.layer_id());
    m_blend_mode      = tile.blend_mode();
    m_animation_id    = tile.animation_id();
    m_animation_state = tile.animation_state();
  }

  /**
   * @brief Assigns values from a generic tile object to this normalized
   * tile.
   *
   * @tparam Tile The type of the tile object.
   * @param tile The tile object to assign from.
   * @return Reference to the updated normalized tile.
   */
  constexpr NormalizedSourceTile &
    operator=(const is_tile auto &tile)
  {
    m_tex_id_buffer = m_tex_id_buffer.with_id(tile.texture_id())
                        .with_blend(tile.blend())
                        .with_depth(tile.depth())
                        .with_draw(tile.draw());
    m_palette_id    = m_palette_id.with_id(tile.palette_id());
    assert(
      std::cmp_less(
        tile.source_x(),
        (std::numeric_limits<std::uint8_t>::max)()));
    assert(
      std::cmp_less(
        tile.source_y(),
        (std::numeric_limits<std::uint8_t>::max)()));
    m_source_xy
      = Point<std::uint8_t>{ static_cast<std::uint8_t>(tile.source_x()),
                             static_cast<std::uint8_t>(tile.source_y()) };
    m_layer_id        = m_layer_id.with_id(tile.layer_id());
    m_blend_mode      = tile.blend_mode();
    m_animation_id    = tile.animation_id();
    m_animation_state = tile.animation_state();
    return *this;
  }

  /// @brief Default copy constructor.
  NormalizedSourceTile(const NormalizedSourceTile &) = default;

  /// @brief Default copy assignment operator.
  NormalizedSourceTile &
    operator=(const NormalizedSourceTile &)              = default;

  /// @brief Default move constructor.
  NormalizedSourceTile(NormalizedSourceTile &&) noexcept = default;

  /// @brief Default move assignment operator.
  NormalizedSourceTile &
    operator=(NormalizedSourceTile &&) noexcept = default;
};

/**
 * @brief Represents a normalized animated tile with reduced properties.
 *
 * This class defines a representation of an animated tile, focusing on key
 * properties like texture, palette, source position, layer, and animation ID.
 * It excludes several properties that are unnecessary or inconsistent for
 * animations:
 *
 * - **Destination X, Y, Z**: These are excluded since animated tiles do not
 * rely on specific destination coordinates.
 * - **Animation State**: Removed because only the animation ID is used to
 * distinguish animations; the state is not relevant for this representation.
 * - **Blend Mode and Blend Other**: Excluded as some animations did not have
 * consistent blends.
 * - **Palette for Animated Tiles**: When an animation ID is present, the
 * palette is set to a unused (maximum value) because some animations have
 * frames with different palettes.
 */
struct NormalizedSourceAnimatedTile
{
public:
  TexIdBuffer m_tex_id_buffer = {};///< Buffer holding texture ID, depth, and
                                   ///< draw flags (blending excluded).
  PaletteID   m_palette_id    = {};///< Identifier for the palette (some
                                   ///< animations use a default palette).
  Point<std::uint8_t> m_source_xy
    = {};///< Source position (x, y) of the tile within the texture.
  LayerID m_layer_id = {};///< Identifier for the layer this tile belongs to.
  std::uint8_t m_animation_id
    = (std::numeric_limits<std::uint8_t>::max)();///< Animation ID (default:
                                                 ///< no animation).

  /// @brief Default constructor.
  constexpr NormalizedSourceAnimatedTile() = default;

  /// @brief Compares two animated tiles for equality or ordering.
  constexpr auto
    operator<=>(const NormalizedSourceAnimatedTile &) const = default;

  /**
   * @brief Constructs a normalized animated tile from a
   * `NormalizedSourceTile`.
   *
   * A value of `255` for `tile.m_animation_id` is normally treated as the
   * sentinel value meaning "no animation". In that case the original palette
   * ID is preserved.
   *
   * Otherwise, animated tiles force the palette ID to the packed-field
   * sentinel value (`15`), since the animation ID field is limited to
   * 4 bits.
   *
   * @param tile The source tile to initialize this animated tile from.
   */
  constexpr NormalizedSourceAnimatedTile(const NormalizedSourceTile &tile)
    : m_tex_id_buffer(tile.m_tex_id_buffer.with_blend(0)),
      m_palette_id([&]() constexpr {
        if (tile.m_animation_id != (std::numeric_limits<std::uint8_t>::max)()) {
          return PaletteID{}.with_id(
            (std::numeric_limits<std::uint8_t>::max)());
        }

        return tile.m_palette_id;
      }()),
      m_source_xy(tile.m_source_xy), m_layer_id(tile.m_layer_id),
      m_animation_id(tile.m_animation_id)
  {}

  /**
   * @brief Constructs a normalized animated tile from a generic tile object.
   *
   * @tparam Tile The type of the tile object, requiring methods to access
   * properties like texture ID, source position, etc.
   * @param tile The tile object to initialize this animated tile from.
   */
  constexpr NormalizedSourceAnimatedTile(const is_tile auto &tile)
    : m_tex_id_buffer{ TexIdBuffer{}
                         .with_id(tile.texture_id())
                         .with_depth(tile.depth())
                         .with_draw(tile.draw()) },
      m_palette_id{
        tile.animation_id() != (std::numeric_limits<std::uint8_t>::max)()
          ? PaletteID{}.with_id((std::numeric_limits<std::uint8_t>::max)())
          : PaletteID{}.with_id(tile.palette_id())
      },
      m_source_xy{ Point<std::uint8_t>{
        static_cast<std::uint8_t>(tile.source_x()),
        static_cast<std::uint8_t>(tile.source_y()) } },
      m_layer_id{ LayerID{}.with_id(tile.layer_id()) },
      m_animation_id{ tile.animation_id() }
  {
    assert(
      std::cmp_less(
        tile.source_x(),
        (std::numeric_limits<std::uint8_t>::max)()));
    assert(
      std::cmp_less(
        tile.source_y(),
        (std::numeric_limits<std::uint8_t>::max)()));
  }

  /**
   * @brief Assigns values from a `NormalizedSourceTile` to this animated
   * tile.
   *
   * @param tile The source tile to assign from.
   * @return Reference to the updated animated tile.
   */
  constexpr NormalizedSourceAnimatedTile &
    operator=(const NormalizedSourceTile &tile)
  {
    m_tex_id_buffer = tile.m_tex_id_buffer.with_blend(0);
    m_palette_id
      = tile.m_animation_id != (std::numeric_limits<std::uint8_t>::max)()
        ? PaletteID{}.with_id((std::numeric_limits<std::uint8_t>::max)())
        : tile.m_palette_id;
    m_source_xy    = tile.m_source_xy;
    m_layer_id     = tile.m_layer_id;
    m_animation_id = tile.m_animation_id;
    return *this;
  }

  /**
   * @brief Assigns values from a generic tile object to this animated tile.
   *
   * @tparam Tile The type of the tile object.
   * @param tile The tile object to assign from.
   * @return Reference to the updated animated tile.
   */
  constexpr NormalizedSourceAnimatedTile &
    operator=(const is_tile auto &tile)
  {
    m_tex_id_buffer = TexIdBuffer{}
                        .with_id(tile.texture_id())
                        .with_depth(tile.depth())
                        .with_draw(tile.draw());
    m_palette_id
      = tile.animation_id() != (std::numeric_limits<std::uint8_t>::max)()
        ? PaletteID{}.with_id((std::numeric_limits<std::uint8_t>::max)())
        : PaletteID{}.with_id(tile.palette_id());
    m_source_xy
      = Point<std::uint8_t>{ static_cast<std::uint8_t>(tile.source_x()),
                             static_cast<std::uint8_t>(tile.source_y()) };
    m_layer_id     = LayerID{}.with_id(tile.layer_id());
    m_animation_id = tile.animation_id();
    assert(
      std::cmp_less(
        tile.source_x(),
        (std::numeric_limits<std::uint8_t>::max)()));
    assert(
      std::cmp_less(
        tile.source_y(),
        (std::numeric_limits<std::uint8_t>::max)()));
    return *this;
  }

  /// @brief Default copy constructor.
  constexpr NormalizedSourceAnimatedTile(const NormalizedSourceAnimatedTile &)
    = default;

  /// @brief Default copy assignment operator.
  constexpr NormalizedSourceAnimatedTile &
    operator=(const NormalizedSourceAnimatedTile &) = default;

  /// @brief Default move constructor.
  constexpr NormalizedSourceAnimatedTile(
    NormalizedSourceAnimatedTile &&) noexcept = default;

  /// @brief Default move assignment operator.
  constexpr NormalizedSourceAnimatedTile &
    operator=(NormalizedSourceAnimatedTile &&) noexcept = default;
};
}// namespace open_viii::graphics::background

template<>
struct fmt::formatter<open_viii::graphics::background::NormalizedSourceTile>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(
      const open_viii::graphics::background::NormalizedSourceTile &tile,
      FormatContext                                               &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "NormalizedSourceTile{{ "
      "tex_id={}, "
      "blend={}, "
      "depth={}, "
      "draw={}, "
      "palette_id={}, "
      "source_xy=({}, {}), "
      "layer_id={}, "
      "blend_mode={}, "
      "animation_id={}, "
      "animation_state={} "
      "}}",
      tile.m_tex_id_buffer.id(),
      tile.m_tex_id_buffer.blend(),
      tile.m_tex_id_buffer.depth().raw(),
      tile.m_tex_id_buffer.draw(),
      tile.m_palette_id.id(),
      tile.m_source_xy.x(),
      tile.m_source_xy.y(),
      tile.m_layer_id.id(),
      static_cast<std::uint32_t>(tile.m_blend_mode),
      tile.m_animation_id,
      tile.m_animation_state);
  }
};

template<>
struct fmt::formatter<
  open_viii::graphics::background::NormalizedSourceAnimatedTile>
{
  constexpr auto
    parse(format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(
      const open_viii::graphics::background::NormalizedSourceAnimatedTile &tile,
      FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "NormalizedSourceAnimatedTile{{ "
      "tex_id={}, "
      "blend={}, "
      "depth={}, "
      "draw={}, "
      "palette_id={}, "
      "source_xy=({}, {}), "
      "layer_id={}, "
      "animation_id={} "
      "}}",
      tile.m_tex_id_buffer.id(),
      tile.m_tex_id_buffer.blend(),
      tile.m_tex_id_buffer.depth().raw(),
      tile.m_tex_id_buffer.draw(),
      tile.m_palette_id.id(),
      tile.m_source_xy.x(),
      tile.m_source_xy.y(),
      tile.m_layer_id.id(),
      tile.m_animation_id);
  }
};

namespace open_viii::graphics::background {
inline std::ostream &
  operator<<(
    std::ostream                                                &os,
    const open_viii::graphics::background::NormalizedSourceTile &tile)
{
  return os << fmt::format("{}", tile);
}

inline std::ostream &
  operator<<(
    std::ostream                                                        &os,
    const open_viii::graphics::background::NormalizedSourceAnimatedTile &tile)
{
  return os << fmt::format("{}", tile);
}
}// namespace open_viii::graphics::background
#endif /* CC1E517B_6340_4FF2_B4A0_BE029215A6E6 */
