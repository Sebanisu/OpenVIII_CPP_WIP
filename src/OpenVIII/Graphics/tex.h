//
// Created by pcvii on 8/27/2020.
//

#ifndef VIIIARCHIVE_TEX_H
#define VIIIARCHIVE_TEX_H
#include "color.h"
#include "ppm.h"
#include <span>
#include <cstdint>
#include <cstring>
#include <ranges>
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

}
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
}
/**
 * texHeader2 contains extra values
 */
struct texHeader2
{
public:
  // Header 2
  /**
   * Color Key Array Flag
   */
  std::uint32_t COLOR_KEY_ARRAY_FLAG{};
  /**
   * Runtime Data 2
   */
  std::uint32_t RUNTIME_DATA2{};
  /**
   * Reference Alpha
   */
  std::uint32_t REFERENCE_ALPHA{};
  /**
   * Runtime Data 3
   */
  std::uint32_t RUNTIME_DATA3{};
  /**
   * Unknown 6
   */
  std::uint32_t UNKNOWN6{};
  /**
   * Palette Index (Runtime Data)
   */
  std::uint32_t PALETTE_INDEX{};
  /**
   * Runtime Data 4
   */
  std::uint32_t RUNTIME_DATA4{};
  /**
   * Runtime Data 5
   */
  std::uint32_t RUNTIME_DATA5{};
  /**
   * Unknown 7
   */
  std::uint32_t UNKNOWN7{};
  /**
   * Unknown 8
   */
  std::uint32_t UNKNOWN8{};
  /**
   * Unknown 9
   */
  std::uint32_t UNKNOWN9{};
  /**
   * Unknown 10
   */
  std::uint32_t UNKNOWN10{};
}
/**
 * This appends to texHeader2 if the file is version 2. used by ff8 only.
 */
struct texHeader2_Version2
{
  /**
   * Unknown 11 (TEX version 2 only)
   */
  std::uint32_t UNKNOWN11{};
};

//{
//  // Palette Entry (BGRA)
//  /**
//   * Blue
//   */
//  std::uint8_t BLUE{};
//  /**
//   * Green
//   */
//  std::uint8_t GREEN{};
//  /**
//   * Red
//   */
//  std::uint8_t RED{};
//  /**
//   * Alpha
//   */
//  std::uint8_t ALPHA{};
//};
/**
 * @see https://github.com/MaKiPL/FF8_Rinoa_s_Toolset/blob/master/SerahToolkit_SharpGL/FF8_Core/TEX.cs
 * @see https://github.com/myst6re/vincent_tim/blob/master/TexFile.cpp
 * @see https://github.com/niemasd/PyFF7/blob/master/PyFF7/tex.py
 */
struct tex
{
private:
  texHeader texHeader_{};
  texPixelFormatHeader texPixelFormatHeader_{};
  texHeader2 texHeader2_{};
  texHeader2_Version2 texHeader2_Version2_{};
  std::span <const char> palette{};
  std::span <const char> imageData{};
public:
  tex() = default;
  explicit tex(std::span<const char> buffer)
  {
    const auto process = [&buffer](auto & member)
    {
           const auto localSpan = buffer.subspan(0,sizeof(member));
           std::memcpy(&member, std::ranges::data(localSpan),sizeof(member));
           buffer = buffer.subspan(sizeof(member));
    };
    process(texHeader_);
    process(texPixelFormatHeader_);
    process(texHeader2_);
    if(texHeader_.VERSION>=2) {
        process(texHeader2_Version2_);
    }
    
  }
};
}
#endif// VIIIARCHIVE_TEX_H
