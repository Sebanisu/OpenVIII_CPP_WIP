//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXHEADER2_H
#define VIIIARCHIVE_TEXHEADER2_H
#include <cstdint>

namespace open_viii::graphics {
/**
 * texHeader2 contains extra values
 */
struct TexHeader2
{
private:
  /**
   * Color Key Array Flag
   */
  std::uint32_t m_color_key_array_flag{};
  /**
   * Runtime Data 2
   */
  std::uint32_t m_runtime_data_2{};
  /**
   * Reference Alpha
   */
  std::uint32_t m_reference_alpha{};
  /**
   * Runtime Data 3
   */
  std::uint32_t m_runtime_data_3{};
  /**
   * Unknown 6
   */
  std::uint32_t m_unknown_6{};
  /**
   * Palette Index (Runtime Data)
   */
  std::uint32_t m_palette_index{};
  /**
   * Runtime Data 4
   */
  std::uint32_t m_runtime_data_4{};
  /**
   * Runtime Data 5
   */
  std::uint32_t m_runtime_data_5{};
  /**
   * Unknown 7
   */
  std::uint32_t m_unknown_7{};
  /**
   * Unknown 8
   */
  std::uint32_t m_unknown_8{};
  /**
   * Unknown 9
   */
  std::uint32_t m_unknown_9{};
  /**
   * Unknown 10
   */
  std::uint32_t m_unknown_10{};
public:
  // Header 2
  /**
   * Color Key Array Flag
   */
  [[maybe_unused]] [[nodiscard]] auto color_key_array_flag() const noexcept
{
return m_color_key_array_flag;
}
  /**
   * Runtime Data 2
   */
  [[maybe_unused]] [[nodiscard]] auto runtime_data_2() const noexcept
{
return m_runtime_data_2;
}
  /**
   * Reference Alpha
   */
  [[maybe_unused]] [[nodiscard]] auto reference_alpha() const noexcept
{
return m_reference_alpha;
}
  /**
   * Runtime Data 3
   */
  [[maybe_unused]] [[nodiscard]] auto runtime_data_3() const noexcept
{
return m_runtime_data_3;
}
  /**
   * Unknown 6
   */
  [[maybe_unused]] [[nodiscard]] auto unknown_6() const noexcept
{
return m_unknown_6;
}
  /**
   * Palette Index (Runtime Data)
   */
  [[maybe_unused]] [[nodiscard]] auto palette_index() const noexcept
{
return m_palette_index;
}
  /**
   * Runtime Data 4
   */
  [[maybe_unused]] [[nodiscard]] auto runtime_data_4() const noexcept
{
return m_runtime_data_4;
}
  /**
   * Runtime Data 5
   */
  [[maybe_unused]] [[nodiscard]] auto runtime_data_5() const noexcept
{
return m_runtime_data_5;
}
  /**
   * Unknown 7
   */
  [[maybe_unused]] [[nodiscard]] auto unknown_7() const noexcept
{
return m_unknown_7;
}
  /**
   * Unknown 8
   */
  [[maybe_unused]] [[nodiscard]] auto unknown_8() const noexcept
{
return m_unknown_8;
}
  /**
   * Unknown 9
   */
  [[maybe_unused]] [[nodiscard]] auto unknown_9() const noexcept
{
return m_unknown_9;
}
  /**
   * Unknown 10
   */
  [[maybe_unused]] [[nodiscard]] auto unknown_10() const noexcept
{
return m_unknown_10;
}
};
}// namespace open_viii::graphics

#endif// VIIIARCHIVE_TEXHEADER2_H
