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

#ifndef VIIIARCHIVE_TIM_H
#define VIIIARCHIVE_TIM_H
#include "BPPT.h"
#include "OpenVIII/Graphics/tim/timHeader.h"
#include "OpenVIII/Graphics/tim/timClutHeader.h"
#include "OpenVIII/Graphics/color.h"
#include "OpenVIII/Graphics/ppm.h"
#include <sstream>
#include "Bit4Values.h"
namespace open_viii::graphics {
/**
 * TIM, or PSX TIM, is an uncompressed raster image file format associated with the Sony PlayStation family of video
 * game consoles. It supports 4- and 8-bit paletted images, and 15- and 24-bit full color images.
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
  TimHeader m_tim_header{};
  TimClutHeader m_tim_clut_header{};
  std::span<const char> m_tim_clut_data{};
  TimImageHeader m_tim_image_header{};
  std::span<const char> m_tim_image_data{};

  [[nodiscard]] Color16 get_color([[maybe_unused]] std::uint16_t row, [[maybe_unused]] std::uint8_t color_key) const
  {
    if (m_tim_clut_header.rectangle().width() == 0 || m_tim_clut_header.rectangle().height() == 0) {
      return {};
    }
    // clangTidy says this function can be static which it cannot be static.
    const auto palette_span = std::span(
      reinterpret_cast<const Color16 *>(std::ranges::data(m_tim_clut_data)), m_tim_clut_header.rectangle().area())
                                .subspan(row * m_tim_clut_header.rectangle().width());
    return palette_span[color_key];
  }
  template<typename dstT> [[nodiscard]] dstT get_color_t(std::size_t index) const
  {
    // TODO remove pointer math
    dstT rv{};
    index *= sizeof(rv);
    if (index + sizeof(rv) >= m_tim_image_data.size()) {
      return {};
    }
    memcpy(&rv, std::ranges::data(m_tim_image_data.subspan(index)), sizeof(rv));
    return rv;
  }

public:
  Tim() = default;
  explicit Tim([[maybe_unused]] std::span<const char> buffer)
  {
    if (std::ranges::size(buffer) == 0) {
      return;
    }
    bool fail = false;
    const auto set_data = [&buffer, &fail]([[maybe_unused]] std::span<const char> &item, const std::size_t &bytes) {
      if (bytes > std::ranges::size(buffer)) {
        fail = true;
        return;
      }
      item = buffer.subspan(0U, bytes);
      buffer = buffer.subspan(bytes);
    };
    const auto get_value = [&buffer, &fail](auto &item) {
      const std::size_t sz = sizeof(item);
      if (sz > std::ranges::size(buffer)) {
        fail = true;
        return;
      }
      memcpy(&item, std::ranges::data(buffer), sz);
      buffer = buffer.subspan(sz);
    };

    get_value(m_tim_header);
    if (!m_tim_header.check() || fail) {
      m_tim_header = {};
      return;
    }
    if (m_tim_header.bpp().color_lookup_table_present()) {
      get_value(m_tim_clut_header);
      if (!m_tim_clut_header.check() || fail) {
        m_tim_clut_header = {};
        m_tim_header = {};
        return;
      }
      set_data(m_tim_clut_data, m_tim_clut_header.data_size());
      if (fail) {
        return;
      }
    }
    get_value(m_tim_image_header);
    if (fail) {
      return;
    }
    set_data(m_tim_image_data, m_tim_image_header.data_size());
  }
  [[nodiscard]] bool check() const
  {
    return m_tim_header.check() && (!m_tim_header.bpp().check() || m_tim_clut_header.check());
  }
  [[nodiscard]] std::size_t width() const
  {
    static constexpr auto bpp4_step{ 4 };
    static constexpr auto bpp8_step{ 2 };
    static constexpr auto bpp24_step{ 1.5 };
    if (m_tim_header.bpp().bpp4()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width() * bpp4_step);// 4pp
    }
    if (m_tim_header.bpp().bpp8()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width() * bpp8_step);// 8pp
    }
    if (m_tim_header.bpp().bpp16()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width());// 16bpp
    }
    if (m_tim_header.bpp().bpp24()) {
      return static_cast<std::size_t>(m_tim_image_header.rectangle().width() / bpp24_step);// 24 bpp
    }
    return {};// invalid value
  }
  [[nodiscard]] auto height() const { return m_tim_image_header.rectangle().height(); }
  [[nodiscard]] auto area() const
  {
    return width() * static_cast<std::size_t>(m_tim_image_header.rectangle().height());
  }
  [[maybe_unused]] [[nodiscard]] auto x() const { return m_tim_image_header.rectangle().x(); }
  [[maybe_unused]] [[nodiscard]] auto y() const { return m_tim_image_header.rectangle().y(); }

  [[maybe_unused]] [[nodiscard]] auto clut_x() const { return m_tim_clut_header.rectangle().x(); }
  [[maybe_unused]] [[nodiscard]] auto clut_y() const { return m_tim_clut_header.rectangle().y(); }
  [[nodiscard]] auto size() const
  {
    return sizeof(m_tim_header) + m_tim_clut_header.size() + m_tim_image_header.size();
  }
  [[nodiscard]] auto clut_rows() const { return m_tim_clut_header.rectangle().height(); }
  [[maybe_unused]] [[nodiscard]] auto clut_colors() const { return m_tim_clut_header.rectangle().width(); }

  friend std::ostream &operator<<(std::ostream &os, const Tim &input)
  {
    return os << '{' << input.m_tim_header << ", " << input.m_tim_clut_header << ", " << input.m_tim_image_header
              << ", Corrected Width: " << input.width() << '}';
  }
  template<Color dstT = Tim> [[nodiscard]] std::vector<dstT> get_colors([[maybe_unused]] std::uint16_t row = 0U) const
  {
    if (width() == 0 || height() == 0) {
      return {};
    }
    static constexpr auto bpp4 = 4U;
    static constexpr auto bpp8 = 8U;
    static constexpr auto bpp16 = 16U;
    static constexpr auto bpp24 = 24U;
    std::vector<dstT> output{};
    const auto out_size = area();
    output.reserve(out_size);
    switch (static_cast<int>(m_tim_header.bpp())) {

    case bpp4: {
      const auto s = std::span(
        reinterpret_cast<const Bit4Values *>(std::ranges::data(m_tim_image_data)), std::ranges::size(m_tim_image_data));
      // break;
      for (size_t i{}; i < out_size / 2; i++) {
        // std::bitset<8> bs{ static_cast<std::uint8_t>(timImageData_.at(i / 2U)) };
        // static constexpr std::bitset<8> oct1{ 0xF };
        // static constexpr std::bitset<8> oct2{ 0xF0 };
        //        if (i % 2 == 0) {
        //          output.emplace_back(get_color(row, static_cast<std::uint8_t>((bs & oct1).to_ulong())));
        //        } else {
        //          output.emplace_back(get_color(row, static_cast<std::uint8_t>(((bs & oct2) >> 4U).to_ulong())));
        //        }
        const auto values = s[i];
        output.emplace_back(get_color(row, values.first));
        output.emplace_back(get_color(row, values.second));
      }
      break;
    }
    case bpp8: {
      // break;
      for (size_t i{}; i < out_size; i++) {
        output.emplace_back(get_color(row, static_cast<std::uint8_t>(m_tim_image_data[i])));
      }
      break;
    }
    case bpp16: {
      const auto sz = std::ranges::size(m_tim_image_data) / sizeof(Color16);
      const auto s = std::span(reinterpret_cast<const Color16 *>(std::ranges::data(m_tim_image_data)), sz);

      for (const Color auto i : s) { output.emplace_back(i); }
      //      for (size_t i{}; i < std::min(outSize, timImageData_.size() / sizeof(color16)); i++) {
      //        output.emplace_back(get_colorT<color16>(i));
      //      }
      break;
    }
    case bpp24: {
      for (size_t i{}; i < out_size; i++) { output.emplace_back(get_color_t<Color24<2, 1, 0>>(i)); }
      break;
    }
    }
    return output;
  }

  [[maybe_unused]] void save(std::string_view filename) const
  {
    if (clut_rows() == 0) {
      Ppm::save(get_colors<Color16>(), width(), height(), filename);
    } else {
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i = 0; i < clut_rows(); i++) {
        auto ss = std::stringstream{};
        ss << (path.parent_path() / path.stem()).string() << '_' << i << path.extension().string();
        Ppm::save(get_colors<Color16>(i), width(), height(), ss.str());
      }
    }
  }
};

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIM_H