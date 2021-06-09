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
#ifndef VIIIARCHIVE_TEX_HPP
#define VIIIARCHIVE_TEX_HPP
#include "Color.hpp"
#include "Ppm.hpp"
#include "open_viii/graphics/tex/TexHeader.hpp"
#include "open_viii/graphics/tex/TexHeader2.hpp"
#include "open_viii/graphics/tex/TexHeader2_Version2.hpp"
#include "open_viii/graphics/tex/TexPixelFormatHeader.hpp"
#include <cstdint>
#include <cstring>
#include <ranges>
#include <span>
namespace open_viii::graphics {
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
 * @see
 * https://github.com/MaKiPL/FF8_Rinoa_s_Toolset/blob/master/SerahToolkit_SharpGL/FF8_Core/TEX.cs
 * @see https://github.com/myst6re/vincent_tim/blob/master/TexFile.cpp
 * @see https://github.com/niemasd/PyFF7/blob/master/PyFF7/tex.py
 */
struct [[maybe_unused]] Tex
{
private:
  TexHeader             m_tex_header{};
  TexPixelFormatHeader  m_tex_pixel_format_header{};
  TexHeader2            m_tex_header2{};
  TexHeader2Version2    m_tex_header2_version2{};
  std::span<const char> m_palette_data{};
  std::span<const char> m_image_data{};
  [[nodiscard]] auto
    get_color_from_palette(std::uint32_t row, std::uint8_t key) const
  {
    if (m_tex_header.num_palettes() == 0
        || key > m_tex_header.num_colors_per_palette()) {
      return Color32{};
    }
    if (row > m_tex_header.num_palettes()) {
      row = 0U;
    }
    const auto palette_span =
      std::span(reinterpret_cast<const Color32<2, 1, 0, 3> *>(
                  std::ranges::data(m_palette_data)),
                m_tex_header.num_colors_per_palette()
                  * m_tex_header.num_palettes())
        .subspan(row * m_tex_header.num_colors_per_palette());
    return Color32<0, 1, 2, 3>(palette_span[key]);
  }
  [[nodiscard]] auto
    size_of_palette() const noexcept
  {
    return m_tex_header.palette_size() * sizeof(std::uint32_t);
  }// I think this is correct.
  [[nodiscard]] auto
    palette_locator() const noexcept
  {
    static constexpr auto version1size = 0xECU;
    static constexpr auto version2size = 0xF0U;
    if (m_tex_header.version() >= 2) {
      return version2size;
    }
    return version1size;
  }
  [[nodiscard]] auto
    texture_locator() const noexcept
  {
    return size_of_palette() + palette_locator();
  }
  void
    reset()
  {
    m_tex_header              = {};
    m_tex_pixel_format_header = {};
    m_tex_header2             = {};
    m_tex_header2_version2    = {};
    m_palette_data            = {};
    m_image_data              = {};
  }

public:
  Tex() = default;
  [[maybe_unused]] explicit Tex(std::span<const char> buffer)
  {
    auto       buffer_backup = buffer;
    bool       fail          = false;
    const auto process       = [&buffer, &fail](auto &member) {
      if (sizeof(member) > std::ranges::size(buffer)) {
        fail = true;
        return;
      }
      const auto local_span = buffer.subspan(0, sizeof(member));
      std::memcpy(&member, std::ranges::data(local_span), sizeof(member));
      buffer = buffer.subspan(sizeof(member));
    };
    process(m_tex_header);
    if (!m_tex_header.check() || fail) {
      reset();
      return;
    }
    process(m_tex_pixel_format_header);
    if (fail) {
      reset();
      return;
    }
    process(m_tex_header2);
    if (m_tex_header.version() >= 2) {
      process(m_tex_header2_version2);
      if (fail) {
        reset();
        return;
      }
    }
    // std::cout << std::hex << TextureLocator() << ", " << PaletteLocator() <<
    // std::dec << '\n';
    if (m_tex_header.palette_flag()) {
      if (std::ranges::size(buffer_backup)
          < size_of_palette() + palette_locator()) {
        reset();
        return;
      }
      m_palette_data =
        buffer_backup.subspan(palette_locator(), size_of_palette());
    }
    if (std::ranges::size(buffer_backup) < texture_locator()) {
      reset();
      return;
    }
    m_image_data = buffer_backup.subspan(texture_locator());
  }
  [[maybe_unused]] [[nodiscard]] auto
    get_colors([[maybe_unused]] std::uint32_t palette_row = 0U) const
  {
    std::vector<Color32<0, 1, 2, 3>> ret{};
    ret.reserve(m_tex_header.image_area());
    if (m_tex_header.palette_flag()) {
      for (const auto &i : m_image_data) {
        ret.emplace_back(
          get_color_from_palette(palette_row, static_cast<std::uint8_t>(i)));
      }
    } else {
      static constexpr auto bpp16 = 16U;
      static constexpr auto bpp32 = 32U;
      switch (m_tex_header.bits_per_pixel()) {
      case bpp16: {
        //        std::vector<color16> tmp(std::ranges::size(ret));
        //        auto size_ = std::min(
        //          std::min(m_tex_header.IMAGE_HEIGHT *
        //          m_tex_header.IMAGE_WIDTH * sizeof(color16),
        //          std::ranges::size(imageData)), std::ranges::size(tmp) *
        //          sizeof(color16));
        //        std::memcpy(std::ranges::data(tmp),
        //        std::ranges::data(imageData), size_);
        //
        //        for (const auto &i : tmp) { ret.emplace_back(i); }
        auto c16data = std::span(
          reinterpret_cast<const Color16 *>(std::ranges::data(m_image_data)),
          std::ranges::size(m_image_data) / sizeof(Color16));
        for (const auto &i : c16data) {
          ret.emplace_back(i);
        }
        break;
      }
        //      case 24: {
        //        break;
        //      }
      case bpp32: {// untested
        auto c32data = std::span(reinterpret_cast<const Color32<2, 1, 0, 3> *>(
                                   std::ranges::data(m_image_data)),
                                 std::ranges::size(m_image_data)
                                   / sizeof(Color32<2, 1, 0, 3>));
        for (const auto &i : c32data) {
          ret.emplace_back(i);
        }
        break;
      }
      }
    }
    return ret;
  };
  [[maybe_unused]] void
    save(std::string_view filename) const
  {
    if (m_tex_header.num_palettes() == 0) {
      Ppm::save(get_colors(),
                m_tex_header.image_width(),
                m_tex_header.image_height(),
                filename);
    } else {
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i = 0; i < m_tex_header.num_palettes(); i++) {
        auto ss = std::stringstream{};
        ss << (path.parent_path() / path.stem()).string() << '_' << i
           << path.extension().string();
        Ppm::save(get_colors(i),
                  m_tex_header.image_width(),
                  m_tex_header.image_height(),
                  ss.str());
      }
    }
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Tex &t)
  {
    return os << "{Version: " << t.m_tex_header.version()
              << ", BPP: " << t.m_tex_header.bits_per_index()
              << ", Palette Count: " << t.m_tex_header.num_palettes()
              << ", Width: " << t.m_tex_header.image_width()
              << ", Height: " << t.m_tex_header.image_height() << "}\n";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TEX_HPP
