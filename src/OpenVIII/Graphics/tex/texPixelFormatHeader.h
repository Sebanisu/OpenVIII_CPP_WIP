//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXPIXELFORMATHEADER_H
#define VIIIARCHIVE_TEXPIXELFORMATHEADER_H
#include <cstdint>

namespace OpenVIII::Graphics {
/**
 * Pixel format header. This tells how the colors are laid out. These are usually always the same.
 */
struct texPixelFormatHeader
{
public:
  // Pixel Format
  /**
   * Number of Red Bits
   */
  std::uint32_t NUM_RED_BITS{};
  /**
   * Number of Green Bits
   */
  std::uint32_t NUM_GREEN_BITS{};
  /**
   * Number of Blue Bits
   */
  std::uint32_t NUM_BLUE_BITS{};
  /**
   * Number of Alpha Bits
   */
  std::uint32_t NUM_ALPHA_BITS{};
  /**
   * Red Bitmask
   */
  std::uint32_t RED_BITMASK{};
  /**
   * Green Bitmask
   */
  std::uint32_t GREEN_BITMASK{};
  /**
   * Blue Bitmask
   */
  std::uint32_t BLUE_BITMASK{};
  /**
   * Alpha Bitmask
   */
  std::uint32_t ALPHA_BITMASK{};
  /**
   * Red Shift
   */
  std::uint32_t RED_SHIFT{};
  /**
   * Green Shift
   */
  std::uint32_t GREEN_SHIFT{};
  /**
   * Blue Shift
   */
  std::uint32_t BLUE_SHIFT{};
  /**
   * Alpha Shift
   */
  std::uint32_t ALPHA_SHIFT{};
  /**
   * 8 _ Number of Red Bits (always ignored)
   */
  std::uint32_t _8_MINUS_NUM_RED_BITS{};
  /**
   * 8 _ Number of Green Bits (always ignored)
   */
  std::uint32_t _8_MINUS_NUM_GREEN_BITS{};
  /**
   * 8 _ Number of Blue Bits (always ignored)
   */
  std::uint32_t _8_MINUS_NUM_BLUE_BITS{};
  /**
   * 8 _ Number of Alpha Bits (always ignored)
   */
  std::uint32_t _8_MINUS_NUM_ALPHA_BITS{};
  /**
   * Red Max
   */
  std::uint32_t RED_MAX{};
  /**
   * Green Max
   */
  std::uint32_t GREEN_MAX{};
  /**
   * Blue Max
   */
  std::uint32_t BLUE_MAX{};
  /**
   * Alpha Max
   */
  std::uint32_t ALPHA_MAX{};
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TEXPIXELFORMATHEADER_H
