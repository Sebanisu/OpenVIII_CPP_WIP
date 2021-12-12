// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_TEXHEADER_HPP
#define VIIIARCHIVE_TEXHEADER_HPP
namespace open_viii::graphics {
/**
 * Header see version to know if the extra value is present in header 2. Tells
 * how the data is laid out in the file.
 */
struct TexHeader
{
private:
  /**
   *Version (must be 1 for ff7 or 2 for ff8)
   */
  std::uint32_t m_version{};
  /**
   * Unknown 1
   */
  std::uint32_t m_unknown_1{};
  /**
   * Color Key Flag
   */
  std::uint32_t m_color_key_flag{};
  /**
   * Unknown 2
   */
  std::uint32_t m_unknown_2{};
  /**
   * Unknown 3
   */
  std::uint32_t m_unknown_3{};
  /**
   * Minimum Number of Bits per Color
   */
  std::uint32_t m_min_bits_per_color{};
  /**
   * Maximum Number of Bits per Color
   */
  std::uint32_t m_max_bits_per_color{};
  /**
   * Minimum Number of Alpha Bits
   */
  std::uint32_t m_min_alpha_bits{};
  /**
   * Maximum Number of Alpha Bits
   */
  std::uint32_t m_max_alpha_bits{};
  /**
   * Minimum Number of Bits per Pixel
   */
  std::uint32_t m_min_bits_per_pixel{};
  /**
   * Maximum Number of Bits per Pixel
   */
  std::uint32_t m_max_bits_per_pixel{};
  /**
   * Unknown 4
   */
  std::uint32_t m_unknown_4{};
  /**
   * Number of Palettes
   */
  std::uint32_t m_num_palettes{};
  /**
   * Number of Colors per Palette
   */
  std::uint32_t m_num_colors_per_palette{};
  /**
   * Bit Depth
   */
  std::uint32_t m_bit_depth{};
  /**
   * Image Width
   */
  std::uint32_t m_image_width{};
  /**
   * Image Height
   */
  std::uint32_t m_image_height{};
  /**
   * Bytes per Row (usually ignored and assumed to be bytes per pixel * width)
   */
  std::uint32_t m_bytes_per_row{};
  /**
   * Unknown 5
   */
  std::uint32_t m_unknown_5{};
  /**
   * Palette Flag (indicates the presence of a palette)
   */
  std::uint32_t m_palette_flag{};
  /**
   * Bits per Index (always 0 for non_paletted images)
   */
  std::uint32_t m_bits_per_index{};
  /**
   * Indexed_to_8bit Flag (never used in FF7)
   */
  std::uint32_t m_indexed_to_8_bit_flag{};
  /**
   * Palette Size (always number of palettes * colors per palette)
   */
  std::uint32_t m_palette_size{};
  /**
   * Number of Colors per Palette again (may be 0 sometimes; the first value
   * will be used anyway)
   */
  std::uint32_t m_num_colors_per_palette_dup{};
  /**
   * Runtime Data 1 (ignored on load)
   */
  std::uint32_t m_runtime_data_1{};
  /**
   * Number of Bits per Pixel
   */
  std::uint32_t m_bits_per_pixel{};
  /**
   * Number of Bytes per Pixel
   */
  std::uint32_t m_bytes_per_pixel{};
  /**
   * Test for valid values.
   * @return
   */
public:
  // Header
  /**
   *Version (must be 1 for ff7 or 2 for ff8)
   */
  [[nodiscard]] auto
    version() const noexcept
  {
    return m_version;
  }
  /**
   * Unknown 1
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    unknown_1() const noexcept
  {
    return m_unknown_1;
  }
  /**
   * Color Key Flag
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    color_key_flag() const noexcept
  {
    return m_color_key_flag;
  }
  /**
   * Unknown 2
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    unknown_2() const noexcept
  {
    return m_unknown_2;
  }
  /**
   * Unknown 3
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    unknown_3() const noexcept
  {
    return m_unknown_3;
  }
  /**
   * Minimum Number of Bits per Color
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    min_bits_per_color() const noexcept
  {
    return m_min_bits_per_color;
  }
  /**
   * Maximum Number of Bits per Color
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    max_bits_per_color() const noexcept
  {
    return m_max_bits_per_color;
  }
  /**
   * Minimum Number of Alpha Bits
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    min_alpha_bits() const noexcept
  {
    return m_min_alpha_bits;
  }
  /**
   * Maximum Number of Alpha Bits
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    max_alpha_bits() const noexcept
  {
    return m_max_alpha_bits;
  }
  /**
   * Minimum Number of Bits per Pixel
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    min_bits_per_pixel() const noexcept
  {
    return m_min_bits_per_pixel;
  }
  /**
   * Maximum Number of Bits per Pixel
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    max_bits_per_pixel() const noexcept
  {
    return m_max_bits_per_pixel;
  }
  /**
   * Unknown 4
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    unknown_4() const noexcept
  {
    return m_unknown_4;
  }
  /**
   * Number of Palettes
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    num_palettes() const noexcept
  {
    return m_num_palettes;
  }
  /**
   * Number of Colors per Palette
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    num_colors_per_palette() const noexcept
  {
    return m_num_colors_per_palette;
  }
  /**
   * Bit Depth
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    bit_depth() const noexcept
  {
    return m_bit_depth;
  }
  /**
   * Image Width
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    image_width() const noexcept
  {
    return m_image_width;
  }
  /**
   * Image Height
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    image_height() const noexcept
  {
    return m_image_height;
  }
  /**
   * Bytes per Row (usually ignored and assumed to be bytes per pixel * width)
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    bytes_per_row() const noexcept
  {
    return m_bytes_per_row;
  }
  /**
   * Unknown 5
   */
  [[nodiscard]] auto
    unknown_5() const noexcept
  {
    return m_unknown_5;
  }
  /**
   * Palette Flag (indicates the presence of a palette)
   */
  [[maybe_unused]] [[nodiscard]] bool
    palette_flag() const noexcept
  {
    return m_palette_flag != 0;
  }
  /**
   * Bits per Index (always 0 for non_paletted images)
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    bits_per_index() const noexcept
  {
    return m_bits_per_index;
  }
  /**
   * Indexed_to_8bit Flag (never used in FF7)
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    indexed_to_8_bit_flag() const noexcept
  {
    return m_indexed_to_8_bit_flag;
  }
  /**
   * Palette Size (always number of palettes * colors per palette)
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    palette_size() const noexcept
  {
    return m_palette_size;
  }
  /**
   * Number of Colors per Palette again (may be 0 sometimes; the first value
   * will be used anyway)
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    num_colors_per_palette_dup() const noexcept
  {
    return m_num_colors_per_palette_dup;
  }
  /**
   * Runtime Data 1 (ignored on load)
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    runtime_data_1() const noexcept
  {
    return m_runtime_data_1;
  }
  /**
   * Number of Bits per Pixel
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    bits_per_pixel() const noexcept
  {
    return m_bits_per_pixel;
  }
  /**
   * Number of Bytes per Pixel
   */
  [[maybe_unused]] [[nodiscard]] const auto &
    bytes_per_pixel() const noexcept
  {
    return m_bytes_per_pixel;
  }
  /**
   * Area of image
   * @return Height * Width
   */
  [[maybe_unused]] [[nodiscard]] std::size_t
    image_area() const noexcept
  {
    return static_cast<std::size_t>(m_image_width)
         * static_cast<std::size_t>(m_image_height);
  }
  /**
   * Test for valid values.
   * @return
   */
  [[nodiscard]] bool
    check() const noexcept
  {
    static constexpr auto bpp1  = 1U;
    static constexpr auto bpp2  = 2U;
    static constexpr auto bpp3  = 3U;
    static constexpr auto bpp4  = 4U;
    static constexpr auto bpp8  = 8U;
    static constexpr auto bpp16 = 16U;
    static constexpr auto bpp24 = 24U;
    static constexpr auto bpp32 = 32U;
    return (m_version == 1 || m_version == 2)
        && ((m_bits_per_pixel == bpp8 && m_bytes_per_pixel == bpp1)
            || (m_bits_per_pixel == bpp16 && m_bytes_per_pixel == bpp2)
            || (m_bits_per_pixel == bpp24 && m_bytes_per_pixel == bpp3)
            || (m_bits_per_pixel == bpp32 && m_bytes_per_pixel == bpp4));
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TEXHEADER_HPP
