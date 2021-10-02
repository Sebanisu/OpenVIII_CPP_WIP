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
#include "open_viii/graphics/tex/TexHeader.hpp"
#include "open_viii/graphics/tex/TexHeader2.hpp"
#include "open_viii/graphics/tex/TexHeader2_Version2.hpp"
#include "open_viii/graphics/tex/TexPixelFormatHeader.hpp"
#include "Png.hpp"
#include "Ppm.hpp"
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
  TexHeader                                m_tex_header{};
  TexPixelFormatHeader                     m_tex_pixel_format_header{};
  TexHeader2                               m_tex_header2{};
  TexHeader2Version2                       m_tex_header2_version2{};
  std::vector<Color32<ColorLayoutT::BGRA>> m_palette_data{};
  std::variant<std::vector<char>,
               std::vector<Color16<ColorLayoutT::ABGR>>,
               std::vector<Color32<ColorLayoutT::BGRA>>>
    m_image_data{};
  [[nodiscard]] auto
    get_color_from_palette(std::uint32_t row, std::uint8_t key) const
  {
    if (m_tex_header.num_palettes() == 0
        || key > m_tex_header.num_colors_per_palette()) {
      return Color32<ColorLayoutT::BGRA>{};
    }
    if (row > m_tex_header.num_palettes()) {
      row = 0U;
    }
    const auto palette_span
      = std::span(m_palette_data)
          .subspan(row * m_tex_header.num_colors_per_palette());
    return palette_span[key];
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
    if (m_tex_header.palette_flag()) {
      if (std::ranges::size(buffer_backup)
          < size_of_palette() + palette_locator()) {
        reset();
        return;
      }
      m_palette_data.resize(size_of_palette()
                            / sizeof(Color32<ColorLayoutT::BGRA>));
      const auto palette_span
        = buffer_backup.subspan(palette_locator(), size_of_palette());
      std::memcpy(m_palette_data.data(),
                  palette_span.data(),
                  palette_span.size());
    }
    if (std::ranges::size(buffer_backup) < texture_locator()) {
      reset();
      return;
    }
    const auto image_span = buffer_backup.subspan(texture_locator());
    switch (m_tex_header.bits_per_pixel()) {
    case 8: {
      m_image_data = [&image_span]() -> std::vector<char> {
        std::vector<char> image(image_span.size());
        std::memcpy(image.data(), image_span.data(), image_span.size());
        return image;
      }();
      break;
    }
    case 16: {
      m_image_data
        = [&image_span]() -> std::vector<Color16<ColorLayoutT::ABGR>> {
        std::vector<Color16<ColorLayoutT::ABGR>> image(
          image_span.size() / sizeof(Color16<ColorLayoutT::ABGR>));
        std::memcpy(image.data(), image_span.data(), image_span.size());
        return image;
      }();
      break;
    }
    case 32: {
      m_image_data
        = [&image_span]() -> std::vector<Color32<ColorLayoutT::BGRA>> {
        std::vector<Color32<ColorLayoutT::BGRA>> image(
          image_span.size() / sizeof(Color32<ColorLayoutT::BGRA>));
        std::memcpy(image.data(), image_span.data(), image_span.size());
        return image;
      }();
      break;
    }
    }
  }
  [[maybe_unused]] [[nodiscard]] auto
    get_colors([[maybe_unused]] std::uint32_t palette_row = 0U) const
  {
    std::vector<Color32<ColorLayoutT::RGBA>> ret{};
    switch (m_image_data.index()) {
    case 0: {
      if (m_tex_header.palette_flag()) {
        ret.reserve(m_tex_header.image_area());
        for (const auto &i : std::get<0>(m_image_data)) {
          ret.emplace_back(
            get_color_from_palette(palette_row, static_cast<std::uint8_t>(i)));
        }
      }
      break;
    }
    case 1:
    case 2: {
      std::visit(
        [&ret](const auto &data) {
          using value_type = typename std::decay_t<decltype(data)>::value_type;
          if constexpr (Color<value_type>) {
            ret.reserve(data.size());
            for (const auto &i : data) {
              ret.emplace_back(i);
            }
          }
        },
        m_image_data);
      break;
    }
    }
    return ret;
  }
  [[maybe_unused]] void
    save(std::string_view filename) const
  {
    if (m_tex_header.num_palettes() == 0) {
      const auto data = get_colors();
      Ppm::save(data,
                m_tex_header.image_width(),
                m_tex_header.image_height(),
                filename);
      Png::save(data,
                m_tex_header.image_width(),
                m_tex_header.image_height(),
                filename,
                std::string{ filename });
    }
    else {
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i = 0; i != m_tex_header.num_palettes(); ++i) {
        auto ss = std::stringstream{};
        ss << (path.parent_path() / path.stem()).string() << '_' << +i << '_'
           << path.extension().string().substr(1);
        const auto data = get_colors(i);
        Ppm::save(data,
                  m_tex_header.image_width(),
                  m_tex_header.image_height(),
                  ss.str());
        Png::save(data,
                  m_tex_header.image_width(),
                  m_tex_header.image_height(),
                  ss.str(),
                  path.string());
      }
      const auto out_path = (path.parent_path() / path.stem()).string()
                          + "_clut" + path.extension().string();
      Ppm::save(m_palette_data,
                m_tex_header.num_colors_per_palette(),
                m_tex_header.num_palettes(),
                out_path);
      if (const auto saved_path
          = Png::save(m_palette_data,
                      m_tex_header.num_colors_per_palette(),
                      m_tex_header.num_palettes(),
                      out_path,
                      path.string());
          saved_path) {
        const Png    read_image = { *saved_path };
        assert(read_image.size() == m_palette_data.size());
//        auto       b1 = read_image.begin();
//        const auto e1 = read_image.end();
//        auto       b2 = m_palette_data.begin();
//        const auto e2 = m_palette_data.end();
//        for (; b1 != e1 && b2 != e2; (void)++b1, ++b2) {
//          assert(*b1 == *b2);
//        }
        Png::save(read_image,
                  m_tex_header.num_colors_per_palette(),
                  m_tex_header.num_palettes(),
                  *saved_path,
                  path.string());
      }
    }
  }
  [[nodiscard]] TexHeader
    tex_header() const noexcept
  {
    return m_tex_header;
  }
};
inline std::ostream &
  operator<<(std::ostream &os, const Tex &t)
{
  return os << "{Version: " << t.tex_header().version()
            << ", Bits Per Pixel: " << t.tex_header().bits_per_pixel()
            << ", Bits Per Index: " << t.tex_header().bits_per_index()
            << ", Bytes Per Pixel: " << t.tex_header().bytes_per_pixel()
            << ", Palette Count: " << t.tex_header().num_palettes()
            << ", Width: " << t.tex_header().image_width()
            << ", Height: " << t.tex_header().image_height() << "}\n";
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TEX_HPP
