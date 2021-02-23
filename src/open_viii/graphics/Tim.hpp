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
#ifndef VIIIARCHIVE_TIM_HPP
#define VIIIARCHIVE_TIM_HPP
#include "BPPT.hpp"
#include "Bit4Values.hpp"
#include "open_viii/graphics/Color.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include "open_viii/graphics/tim/TimClutHeader.hpp"
#include "open_viii/graphics/tim/TimHeader.hpp"
#include <sstream>
namespace open_viii::graphics {
/**
 * TIM, or PSX TIM, is an uncompressed raster image file format associated with
 * the Sony PlayStation family of video game consoles. It supports 4- and 8-bit
 * paletted images, and 15- and 24-bit full color images.
 * @see https://github.com/myst6re/vincent-tim
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 * @see http://www.raphnet.net/electronique/psx_adaptor/Playstation.txt
 * @see http://www.psxdev.net/forum/viewtopic.php?t=109
 * @see https://mrclick.zophar.net/TilEd/download/timgfx.txt
 * @see http://www.elisanet.fi/6581/PSX/doc/Playstation_Hardware.pdf
 * @see http://www.elisanet.fi/6581/PSX/doc/psx.pdf
 * @see http://www.romhacking.net/documents/31/
 * @see http://mrclick.zophar.net/TilEd/download/timgfx.txt
 * @see http://fileformats.archiveteam.org/wiki/TIM_(PlayStation_graphics)
 */
struct Tim
{
private:
  TimHeader            m_tim_header{};
  TimClutHeader        m_tim_clut_header{};
  std::vector<Color16> m_tim_clut_data{};
  TimImageHeader       m_tim_image_header{};
  std::variant<std::vector<Bit4Values>,
               std::vector<std::uint8_t>,
               std::vector<Color16>,
               std::vector<Color24<2U, 1U, 0U>>>
                     m_tim_image_data{};
  [[nodiscard]] auto size_of_image_data() const
  {
    return std::visit(
      [](const auto &variant) {
        return std::ranges::size(variant);
      },
      m_tim_image_data);
  }
  [[nodiscard]] Color16 get_color([[maybe_unused]] std::uint16_t row,
                                  [[maybe_unused]] std::uint8_t color_key) const
  {
    if (m_tim_clut_header.rectangle().width() == 0
        || m_tim_clut_header.rectangle().height() == 0) {
      return {};
    }
    const std::size_t offset = row * m_tim_clut_header.rectangle().width();
    return m_tim_clut_data.at(offset + color_key);
  }
public:
  Tim() = default;
  explicit Tim([[maybe_unused]] std::span<const char> buffer)
    : m_tim_header(get_tim_header(buffer)),
      m_tim_clut_header(get_tim_clut_header(buffer)),
      m_tim_clut_data(get_tim_clut_data(buffer)),
      m_tim_image_header(get_tim_image_header(buffer)),
      m_tim_image_data(get_tim_image_data(buffer))
  {}
  template<is_trivially_copyable_and_default_constructible trivialT>
  [[nodiscard]] static auto read_val(std::span<const char> &buffer)
  {
    if (std::ranges::size(buffer) < sizeof(trivialT)) {
      return trivialT();
    }
    const auto header = tools::read_val<trivialT>(buffer);
    buffer            = buffer.subspan(sizeof(trivialT));
    return header;
  }
  template<is_default_constructible_has_data_and_size rangeT>
  [[nodiscard]] static auto read_val(std::span<const char> &buffer,
                                     const auto             bytes)
  {
    if (std::ranges::size(buffer) < bytes) {
      return rangeT();
    }
    const auto header = tools::read_val<rangeT>(buffer, bytes);
    buffer            = buffer.subspan(bytes);
    return header;
  }
  [[nodiscard]] static TimHeader get_tim_header(std::span<const char> &buffer)
  {
    return read_val<TimHeader>(buffer);
  }
  [[nodiscard]] TimClutHeader
    get_tim_clut_header(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check()
        || !m_tim_header.bpp().color_lookup_table_present()) {
      return {};
    }
    return read_val<TimClutHeader>(buffer);
  }
  [[nodiscard]] std::vector<Color16>
    get_tim_clut_data(std::span<const char> &buffer) const
  {
    if (!m_tim_clut_header.check() || m_tim_clut_header.data_size() == 0U) {
      return {};
    }
    return read_val<std::vector<Color16>>(buffer,
                                          m_tim_clut_header.data_size());
  }
  [[nodiscard]] TimImageHeader
    get_tim_image_header(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check()) {
      return {};
    }
    return read_val<TimImageHeader>(buffer);
  }
  [[nodiscard]] decltype(m_tim_image_data)
    get_tim_image_data(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check() || m_tim_image_header.data_size() == 0) {
      return {};
    }
    switch (static_cast<int>(m_tim_header.bpp())) {
    case 4: {
      return read_val<std::vector<Bit4Values>>(buffer,
                                               m_tim_image_header.data_size());
    }
    case 8: {
      return read_val<std::vector<std::uint8_t>>(
        buffer, m_tim_image_header.data_size());
    }
    case 16: {
      return read_val<std::vector<Color16>>(buffer,
                                            m_tim_image_header.data_size());
    }
    case 24: {
      return read_val<std::vector<Color24<2, 1, 0>>>(
        buffer, m_tim_image_header.data_size());
    }
    }
    assert(false);
    return {};
  }
  [[nodiscard]] bool check() const
  {
    const size_t sizeOfImageData = size_of_image_data();
    const size_t a               = area();
    const auto   thc             = m_tim_header.check();
    const auto   bppc            = m_tim_header.bpp().check();
    const auto   chc             = m_tim_clut_header.check();
    return thc && (!bppc || chc)
           && (a == sizeOfImageData || a / 2 == sizeOfImageData
               || a * 2 == sizeOfImageData);
  }
  [[nodiscard]] std::size_t width() const
  {
    static constexpr auto bpp4_step{ 4 };
    static constexpr auto bpp8_step{ 2 };
    static constexpr auto bpp24_step{ 1.5 };
    if (m_tim_header.bpp().bpp4()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width()
                                      * bpp4_step);// 4pp
    }
    if (m_tim_header.bpp().bpp8()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width()
                                      * bpp8_step);// 8pp
    }
    if (m_tim_header.bpp().bpp16()) {
      return static_cast<std::size_t>(
        m_tim_image_header.rectangle().width());// 16bpp
    }
    if (m_tim_header.bpp().bpp24()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width()
                                      / bpp24_step);// 24 bpp
    }
    return {};// invalid value
  }
  [[nodiscard]] auto height() const
  {
    return m_tim_image_header.rectangle().height();
  }
  [[nodiscard]] std::size_t area() const
  {
    return width()
           * static_cast<std::size_t>(m_tim_image_header.rectangle().height());
  }
  [[maybe_unused]] [[nodiscard]] auto x() const
  {
    return m_tim_image_header.rectangle().x();
  }
  [[maybe_unused]] [[nodiscard]] auto y() const
  {
    return m_tim_image_header.rectangle().y();
  }
  [[maybe_unused]] [[nodiscard]] auto clut_x() const
  {
    return m_tim_clut_header.rectangle().x();
  }
  [[maybe_unused]] [[nodiscard]] auto clut_y() const
  {
    return m_tim_clut_header.rectangle().y();
  }
  [[nodiscard]] auto size() const
  {
    return sizeof(m_tim_header) + m_tim_clut_header.size()
           + m_tim_image_header.size();
  }
  [[nodiscard]] auto clut_rows() const
  {
    return m_tim_clut_header.rectangle().height();
  }
  [[maybe_unused]] [[nodiscard]] auto clut_colors() const
  {
    return m_tim_clut_header.rectangle().width();
  }
  friend std::ostream &operator<<(std::ostream &os, const Tim &input)
  {
    return os << '{' << input.m_tim_header << ", " << input.m_tim_clut_header
              << ", " << input.m_tim_image_header
              << ", Corrected Width: " << input.width() << '}';
  }
  template<Color dstT = Tim>
  [[nodiscard]] std::vector<dstT>
    get_colors([[maybe_unused]] std::uint16_t row = 0U) const
  {
    if (width() == 0 || height() == 0) {
      return {};
    }
    static constexpr auto bpp4  = 0;
    static constexpr auto bpp8  = 1;
    static constexpr auto bpp16 = 2;
    static constexpr auto bpp24 = 3;
    std::vector<dstT>     output{};
    const auto            out_size = area();
    output.reserve(out_size);
    switch (m_tim_image_data.index()) {
    case bpp4: {
      const size_t tim_image_data_size = size_of_image_data();
      const size_t half_area           = out_size / 2;
      assert(tim_image_data_size > 0U);
      assert(half_area == tim_image_data_size);
      std::ranges::for_each(
        std::get<0>(m_tim_image_data),
        [this, &output, &row](const Bit4Values &color_indexes) {
          const auto &[color1, color2] = color_indexes;
          output.emplace_back(get_color(row, color1));
          output.emplace_back(get_color(row, color2));
        });
      break;
    }
    case bpp8: {
      std::ranges::transform(std::get<1>(m_tim_image_data),
                             std::back_inserter(output),
                             [this, &row](const std::uint8_t color_index) {
                               return static_cast<dstT>(
                                 get_color(row, color_index));
                             });
      break;
    }
    case bpp16: {
      std::ranges::transform(std::get<2>(m_tim_image_data),
                             std::back_inserter(output),
                             [](const Color16 color) {
                               return static_cast<dstT>(color);
                             });
      break;
    }
    case bpp24: {
      std::ranges::transform(std::get<3>(m_tim_image_data),
                             std::back_inserter(output),
                             [](const Color24<2, 1, 0> color) {
                               return static_cast<dstT>(color);
                             });
      break;
    }
    }
    return output;
  }
  [[maybe_unused]] void save(std::string_view filename) const
  {
    if (clut_rows() == 0) {
      Ppm::save(get_colors<Color24<0, 1, 2>>(), width(), height(), filename);
    } else {
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i = 0; i < clut_rows(); i++) {
        const auto out_path = (path.parent_path() / path.stem()).string() + '_'
                              + std::to_string(i) + path.extension().string();
        Ppm::save(get_colors<Color24<0, 1, 2>>(i), width(), height(), out_path);
      }
    }
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIM_HPP