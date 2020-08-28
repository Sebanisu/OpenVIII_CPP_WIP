//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXHEADER2_H
#define VIIIARCHIVE_TEXHEADER2_H
#include <cstdint>

namespace OpenVIII::Graphics {
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
};
}// namespace OpenVIII::Graphics

#endif// VIIIARCHIVE_TEXHEADER2_H
