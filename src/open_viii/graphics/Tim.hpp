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
#include "Png.hpp"
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
  TimHeader                                m_tim_header{};
  TimClutHeader                            m_tim_clut_header{};
  std::vector<Color16<ColorLayoutT::ABGR>> m_tim_clut_data{};
  TimImageHeader                           m_tim_image_header{};
  std::variant<std::vector<Bit4Values>,
               std::vector<std::uint8_t>,
               std::vector<Color16<ColorLayoutT::ABGR>>,
               std::vector<Color24<ColorLayoutT::BGR>>>
    m_tim_image_data{};
  [[nodiscard]] std::size_t
    size_of_image_data() const noexcept
  {
    return std::visit(
      [](const auto &variant) {
        return std::ranges::size(variant);
      },
      m_tim_image_data);
  }
  [[nodiscard]] Color16<ColorLayoutT::ABGR>
    get_color([[maybe_unused]] const std::uint16_t row,
              [[maybe_unused]] const std::uint8_t  color_key,
              const decltype(m_tim_clut_header.rectangle()
                               .width_height())    clut_dims) const
  {
    if (clut_dims.x() == 0 || clut_dims.y() == 0
        || std::empty(m_tim_clut_data)) {
      return {};
    }
    const std::size_t offset = row * clut_dims.x();
    const auto        i      = offset + color_key;
    if (m_tim_clut_data.size() > i) {
      return m_tim_clut_data[i];
    }
    return {};
  }
  template<typename dstT>
  auto
    get_4bpp_colors(const uint16_t                    row,
                    const decltype(m_tim_clut_header.rectangle()
                                     .width_height()) clut_dims) const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data() * 2U);
    std::ranges::for_each(
      std::get<0>(m_tim_image_data),
      [this, &output, &row, &clut_dims](const Bit4Values &color_indexes) {
        const auto &[color1, color2] = color_indexes;
        output.emplace_back(get_color(row, color1, clut_dims));
        output.emplace_back(get_color(row, color2, clut_dims));
      });
    return output;
  }
  template<typename dstT>
  auto
    get_8bbp_colors(const uint16_t                    row,
                    const decltype(m_tim_clut_header.rectangle()
                                     .width_height()) clut_dims) const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data());
    std::ranges::transform(std::get<1>(m_tim_image_data),
                           std::back_inserter(output),
                           [this, &row, &clut_dims](const uint8_t color_index) {
                             return static_cast<dstT>(
                               get_color(row, color_index, clut_dims));
                           });
    return output;
  }
  template<typename dstT>
  auto
    get_16bpp_colors() const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data());
    std::ranges::transform(std::get<2>(m_tim_image_data),
                           std::back_inserter(output),
                           [](const Color16<ColorLayoutT::ABGR> &color) {
                             return static_cast<dstT>(color);
                           });
    return output;
  }
  template<typename dstT>
  auto
    get_24bpp_colors() const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data());
    std::ranges::transform(std::get<3>(m_tim_image_data),
                           std::back_inserter(output),
                           [](const Color24<ColorLayoutT::BGR> &color) {
                             return static_cast<dstT>(color);
                           });
    return output;
  }

public:
  Tim() = default;
  explicit Tim(std::span<const char> buffer)
    : m_tim_header(get_tim_header(buffer)),
      m_tim_clut_header(get_tim_clut_header(buffer)),
      m_tim_clut_data(get_tim_clut_data(buffer)),
      m_tim_image_header(get_tim_image_header(buffer)),
      m_tim_image_data(get_tim_image_data(buffer))
  {}
  explicit Tim(
    TimHeader                                             in_tim_header,
    TimClutHeader                                         in_tim_clut_header,
    std::vector<Color16<ColorLayoutT::ABGR>>              in_tim_clut_data,
    TimImageHeader                                        in_tim_image_header,
    std::variant<std::vector<Bit4Values>,
                 std::vector<std::uint8_t>,
                 std::vector<Color16<ColorLayoutT::ABGR>>,
                 std::vector<Color24<ColorLayoutT::BGR>>> in_tim_image_data)
    : m_tim_header(in_tim_header), m_tim_clut_header(in_tim_clut_header),
      m_tim_clut_data(std::move(in_tim_clut_data)),
      m_tim_image_header(in_tim_image_header),
      m_tim_image_data(std::move(in_tim_image_data))
  {}

  [[nodiscard]] static TimHeader
    get_tim_header(std::span<const char> &buffer)
  {
    return tools::read_val_safe_mutate<TimHeader>(buffer);
  }
  [[nodiscard]] TimClutHeader
    get_tim_clut_header(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check()
        || !m_tim_header.bpp().color_lookup_table_present()) {
      return {};
    }
    return tools::read_val_safe_mutate<TimClutHeader>(buffer);
  }
  [[nodiscard]] std::vector<Color16<ColorLayoutT::ABGR>>
    get_tim_clut_data(std::span<const char> &buffer) const
  {
    if (!m_tim_clut_header.check() || m_tim_clut_header.data_size() == 0U) {
      return {};
    }
    return tools::read_val_safe_mutate<
      std::vector<Color16<ColorLayoutT::ABGR>>>(buffer,
                                                m_tim_clut_header.data_size());
  }
  [[nodiscard]] TimImageHeader
    get_tim_image_header(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check()) {
      return {};
    }
    return tools::read_val_safe_mutate<TimImageHeader>(buffer);
  }
  [[nodiscard]] decltype(m_tim_image_data)
    get_tim_image_data(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check() || m_tim_image_header.data_size() == 0) {
      return {};
    }
    switch (static_cast<int>(m_tim_header.bpp())) {
    case 4: {
      return tools::read_val_safe_mutate<std::vector<Bit4Values>>(
        buffer,
        m_tim_image_header.data_size());
    }
    case 8: {
      return tools::read_val_safe_mutate<std::vector<std::uint8_t>>(
        buffer,
        m_tim_image_header.data_size());
    }
    case 16: {
      return tools::read_val_safe_mutate<
        std::vector<Color16<ColorLayoutT::ABGR>>>(
        buffer,
        m_tim_image_header.data_size());
    }
    case 24: {
      return tools::read_val_safe_mutate<
        std::vector<Color24<ColorLayoutT::BGR>>>(
        buffer,
        m_tim_image_header.data_size());
    }
    }
    assert(false);
    return {};
  }
  [[nodiscard]] bool
    check() const
  {
    const size_t sizeOfImageData = size_of_image_data();
    const size_t a               = area();
    return m_tim_header && (!m_tim_header.bpp() || m_tim_clut_header)
        && (a == sizeOfImageData || a / 2 == sizeOfImageData
            || a * 2 == sizeOfImageData);
  }
  [[nodiscard]] std::uint16_t
    width() const
  {
    static constexpr auto bpp4_step{ 4 };
    static constexpr auto bpp8_step{ 2 };
    static constexpr auto bpp24_step{ 1.5 };
    if (m_tim_header.bpp().bpp4()) {
      return m_tim_image_header.rectangle().width() * bpp4_step;// 4pp
    }
    if (m_tim_header.bpp().bpp8()) {
      return m_tim_image_header.rectangle().width() * bpp8_step;// 8pp
    }
    if (m_tim_header.bpp().bpp16()) {
      return m_tim_image_header.rectangle().width();// 16bpp
    }
    if (m_tim_header.bpp().bpp24()) {
      return static_cast<uint16_t>(m_tim_image_header.rectangle().width()
                                   / bpp24_step);// 24 bpp
    }
    return {};// invalid value
  }
  [[nodiscard]] auto
    height() const
  {
    return m_tim_image_header.rectangle().height();
  }
  [[nodiscard]] std::size_t
    area() const
  {
    return width()
         * static_cast<std::size_t>(m_tim_image_header.rectangle().height());
  }
  [[maybe_unused]] [[nodiscard]] auto
    x() const
  {
    return m_tim_image_header.rectangle().x();
  }
  [[maybe_unused]] [[nodiscard]] auto
    y() const
  {
    return m_tim_image_header.rectangle().y();
  }
  [[maybe_unused]] [[nodiscard]] auto
    clut_x() const
  {
    return m_tim_clut_header.rectangle().x();
  }
  [[maybe_unused]] [[nodiscard]] auto
    clut_y() const
  {
    return m_tim_clut_header.rectangle().y();
  }
  [[nodiscard]] auto
    size() const
  {
    return sizeof(m_tim_header) + m_tim_clut_header.size()
         + m_tim_image_header.size();
  }
  [[nodiscard]] auto
    clut_rows() const
  {
    return m_tim_clut_header.rectangle().height();
  }
  [[maybe_unused]] [[nodiscard]] auto
    clut_colors() const
  {
    return m_tim_clut_header.rectangle().width();
  }
  [[nodiscard]] explicit operator bool() const
  {
    return check();
  }
  friend std::ostream &
    operator<<(std::ostream &os, const Tim &input)
  {
    return os << '{' << input.m_tim_header << ", " << input.m_tim_clut_header
              << ", " << input.m_tim_image_header
              << ", Corrected Width: " << input.width() << '}';
  }
  template<Color dstT = Tim>
  [[nodiscard]] std::vector<dstT>
    get_colors([[maybe_unused]] const std::uint16_t                   row = 0U,
               decltype(m_tim_clut_header.rectangle().width_height()) clut_dims
               = {}) const
  {
    if (clut_dims == decltype(clut_dims){}) {
      clut_dims = m_tim_clut_header.rectangle().width_height();
    }
    enum
    {
      bpp4,
      bpp8,
      bpp16,
      bpp24,
    };
    [[maybe_unused]] const size_t tim_image_data_size = size_of_image_data();
    assert(tim_image_data_size > 0U && area() != 0);
    switch (m_tim_image_data.index()) {
    case bpp4: {
      assert(area() / 2U == tim_image_data_size);
      return get_4bpp_colors<dstT>(row, clut_dims);
    }
    case bpp8: {
      assert(area() == tim_image_data_size);
      return get_8bbp_colors<dstT>(row, clut_dims);
    }
    case bpp16: {
      assert(area() * 2U == tim_image_data_size);
      return get_16bpp_colors<dstT>();
    }
    case bpp24: {
      assert(area() == tim_image_data_size);
      return get_24bpp_colors<dstT>();
    }
    default: {
      assert(false);
      return {};
    }
    }
  }
  /**
   * Update the dims of the clut.
   * @param width_height usually width is number of colors per palette, height
   * is number of palettes.
   * @return A new Tim with the new clut dims.
   */
  Tim
    force_tim_clut_dims(Point<std::uint16_t> width_height) const
  {
    if (m_tim_clut_header.rectangle().area() < width_height.area()) {
      // return empty tim if area is too large.
      return {};
    }
    return Tim(m_tim_header,
               TimClutHeader(m_tim_clut_header.size(),
                             Rectangle<std::uint16_t>(
                               m_tim_clut_header.rectangle().top_left(),
                               width_height)),
               m_tim_clut_data,
               m_tim_image_header,
               m_tim_image_data);
  }
  /**
   * Update the dims of the clut.
   * @param width usually is number of colors per palette
   * @param height usually is number of palettes.
   * @return A new Tim with the new clut dims.
   */
  [[maybe_unused]] Tim
    force_tim_clut_dims(std::uint16_t width, std::uint16_t height) const
  {
    return force_tim_clut_dims(Point<std::uint16_t>(width, height));
  }
  /**
   *
   * @param filename
   */
  [[maybe_unused]] void
    save(std::string_view                                       filename,
         decltype(m_tim_clut_header.rectangle().width_height()) clut_dims = {},
         const int                                              clut = -1) const
  {
    if (clut_rows() == 0) {
      const auto &data = get_colors<Color16<ColorLayoutT::ABGR>>();
      Ppm::save(data, width(), height(), filename);
      Png::save(data,
                width(),
                height(),
                filename,
                std::filesystem::path(filename).string());
    }
    else {
      if (clut_dims == decltype(clut_dims){})
        clut_dims = m_tim_clut_header.rectangle().width_height();
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i{}; i != clut_dims.y(); ++i) {
        std::string prefix = "";
        if (clut >= 0) {
          i      = static_cast<uint16_t>(clut);
          prefix = "F";
        }
        const auto out_path = (path.parent_path() / path.stem()).string() + '_'
                            + prefix + std::to_string(i)
                            + path.extension().string();
        const auto &data
          = get_colors<Color16<ColorLayoutT::ABGR>>(i, clut_dims);
        Ppm::save(data, width(), height(), out_path);
        Png::save(data, width(), height(), out_path, out_path);
        if (clut >= 0)
          return;
      }
    }
    if (clut_rows() != 0) {
      auto       path     = std::filesystem::path(filename);
      const auto out_path = (path.parent_path() / path.stem()).string()
                          + "_clut" + path.extension().string();
      Ppm::save(m_tim_clut_data,
                m_tim_clut_header.rectangle().width(),
                m_tim_clut_header.rectangle().height(),
                out_path);
      Png::save(m_tim_clut_data,
                m_tim_clut_header.rectangle().width(),
                m_tim_clut_header.rectangle().height(),
                out_path,
                std::filesystem::path(filename).string());
    }
  }
};
template<>
auto
  Tim::get_16bpp_colors<Color16<ColorLayoutT::ABGR>>() const
{
  return std::get<2>(m_tim_image_data);
}
template<>
auto
  Tim::get_24bpp_colors<Color24<ColorLayoutT::BGR>>() const
{
  return std::get<3>(m_tim_image_data);
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIM_HPP