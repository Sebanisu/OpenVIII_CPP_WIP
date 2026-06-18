//
// Created by pcvii on 9/6/2022.
//

#ifndef OPEN_VIII_GRAPHICS_BACKGROUND__UNIQUIFYPUPU_HPP
#define OPEN_VIII_GRAPHICS_BACKGROUND__UNIQUIFYPUPU_HPP

#include "PupuID.hpp"
#include <map>

namespace open_viii::graphics::background
{
/**
 * @brief Generates unique PupuIDs for tiles that otherwise share the same
 *        base PupuID.
 *
 * Tiles that have the same raw PupuID but appear at different coordinates
 * are assigned incrementing offsets so they can be distinguished during
 * deswizzle and reconstruction operations.
 *
 * This class is stateful and intended for single-pass processing.
 * Each logical tile should only be processed once per instance lifetime.
 * Re-processing the same tile multiple times may advance the uniqueness
 * offset unexpectedly.
 *
 * Typical usage is:
 * - perform one traversal over all tiles
 * - cache/store the resulting PupuIDs
 * - reuse the cached values for later comparisons
 */
class UniquifyPupu
{
public:
  /**
   * @brief Key used to uniquely identify a tile occurrence.
   *
   * The base PupuID alone is insufficient because multiple tiles may share
   * the same texture data while existing at different map positions.
   */
  struct PupuKey
  {
    /**
     * @brief Base PupuID generated directly from the tile.
     */
    PupuID pupu_id = {};

    /**
     * @brief Tile X coordinate.
     */
    std::int16_t x = {};

    /**
     * @brief Tile Y coordinate.
     */
    std::int16_t y = {};

    /**
     * @brief Default three-way comparison operator.
     */
    constexpr auto operator<=>(const PupuKey &) const noexcept = default;
  };

  /**
   * @brief Generates or retrieves a unique PupuID for a tile.
   *
   * The returned PupuID is stable for the lifetime of this
   * UniquifyPupu instance provided the same tile is not processed
   * repeatedly in separate logical passes.
   *
   * @param tile_const Tile to uniquely identify.
   * @return Unique PupuID for the tile.
   */
  PupuID operator()(const is_tile auto &tile_const);

private:
  /**
   * @brief Tracks uniqueness offsets for previously encountered tiles.
   *
   * The value represents the uniqueness index assigned to the key.
   */
  std::map<PupuKey, std::uint8_t> m_pupu_map = {};
};

}// namespace open_viii::graphics::background

#endif// OPEN_VIII_GRAPHICS_BACKGROUND__UNIQUIFYPUPU_HPP