//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXHEADER_H
#define VIIIARCHIVE_TEXHEADER_H
#include <cstdint>

namespace OpenVIII::Graphics {
/**
 * Header see version to know if the extra value is present in header 2. Tells how the data is laid out in the file.
 */

struct texHeader
{
public:
  // Header
  /**
   *Version (must be 1 for ff7 or 2 for ff8)
   */
  std::uint32_t VERSION{};
  /**
   * Unknown 1
   */
  std::uint32_t UNKNOWN1{};
  /**
   * Color Key Flag
   */
  std::uint32_t COLOR_KEY_FLAG{};
  /**
   * Unknown 2
   */
  std::uint32_t UNKNOWN2{};
  /**
   * Unknown 3
   */
  std::uint32_t UNKNOWN3{};
  /**
   * Minimum Number of Bits per Color
   */
  std::uint32_t MIN_BITS_PER_COLOR{};
  /**
   * Maximum Number of Bits per Color
   */
  std::uint32_t MAX_BITS_PER_COLOR{};
  /**
   * Minimum Number of Alpha Bits
   */
  std::uint32_t MIN_ALPHA_BITS{};
  /**
   * Maximum Number of Alpha Bits
   */
  std::uint32_t MAX_ALPHA_BITS{};
  /**
   * Minimum Number of Bits per Pixel
   */
  std::uint32_t MIN_BITS_PER_PIXEL{};
  /**
   * Maximum Number of Bits per Pixel
   */
  std::uint32_t MAX_BITS_PER_PIXEL{};
  /**
   * Unknown 4
   */
  std::uint32_t UNKNOWN4{};
  /**
   * Number of Palettes
   */
  std::uint32_t NUM_PALETTES{};
  /**
   * Number of Colors per Palette
   */
  std::uint32_t NUM_COLORS_PER_PALETTE{};
  /**
   * Bit Depth
   */
  std::uint32_t BIT_DEPTH{};
  /**
   * Image Width
   */
  std::uint32_t IMAGE_WIDTH{};
  /**
   * Image Height
   */
  std::uint32_t IMAGE_HEIGHT{};
  /**
   * Bytes per Row (usually ignored and assumed to be bytes per pixel * width)
   */
  std::uint32_t BYTES_PER_ROW{};
  /**
   * Unknown 5
   */
  std::uint32_t UNKNOWN5{};
  /**
   * Palette Flag (indicates the presence of a palette)
   */
  std::uint32_t PALETTE_FLAG{};
  /**
   * Bits per Index (always 0 for non_paletted images)
   */
  std::uint32_t BITS_PER_INDEX{};
  /**
   * Indexed_to_8bit Flag (never used in FF7)
   */
  std::uint32_t INDEXED_TO_8BIT_FLAG{};
  /**
   * Palette Size (always number of palettes * colors per palette)
   */
  std::uint32_t PALETTE_SIZE{};
  /**
   * Number of Colors per Palette again (may be 0 sometimes; the first value will be used anyway)
   */
  std::uint32_t NUM_COLORS_PER_PALETTE_DUP{};
  /**
   * Runtime Data 1 (ignored on load)
   */
  std::uint32_t RUNTIME_DATA1{};
  /**
   * Number of Bits per Pixel
   */
  std::uint32_t BITS_PER_PIXEL{};
  /**
   * Number of Bytes per Pixel
   */
  std::uint32_t BYTES_PER_PIXEL{};
  /**
   * Test for valid values.
   * @return
   */
  bool Check()
  {
    static constexpr auto _1Bpp = 1U;
    static constexpr auto _2Bpp = 2U;
    static constexpr auto _3Bpp = 3U;
    static constexpr auto _4Bpp = 4U;
    static constexpr auto _8bpp = 8U;
    static constexpr auto _16bpp = 16U;
    static constexpr auto _24bpp = 24U;
    static constexpr auto _32bpp = 32U;
    return (VERSION == 1 || VERSION == 2)
           && ((BITS_PER_PIXEL == _8bpp && BYTES_PER_PIXEL == _1Bpp)
               || (BITS_PER_PIXEL == _16bpp && BYTES_PER_PIXEL == _2Bpp)
               || (BITS_PER_PIXEL == _24bpp && BYTES_PER_PIXEL == _3Bpp)
               || (BITS_PER_PIXEL == _32bpp && BYTES_PER_PIXEL == _4Bpp));
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TEXHEADER_H
