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
#include "Bit4Values.hpp"
#include "BPPT.hpp"
#include "open_viii/graphics/Color.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include "open_viii/graphics/tim/TimClutHeader.hpp"
#include "open_viii/graphics/tim/TimHeader.hpp"
#include "Png.hpp"
namespace open_viii::graphics {
/**
 * @brief TIM, or PSX TIM, is an uncompressed raster image file format
 * associated with the Sony PlayStation family of video game consoles. It
 * supports 4- and 8-bit paletted images, and 15- and 24-bit full color images.
 *
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
 * @see http://www.psxdev.net/forum/viewtopic.php?t=780
 * @see https://github.com/ColdSauce/psxsdk/blob/master/tools/bmp2tim.c
 */
struct Tim
{
private:
  /**
   * @brief The header of the TIM file.
   */
  TimHeader                                m_tim_header{};

  /**
   * @brief The header of the Color Lookup Table (CLUT) in the TIM file.
   */
  TimClutHeader                            m_tim_clut_header{};

  /**
   * @brief The data of the Color Lookup Table (CLUT) in the TIM file.
   */
  std::vector<Color16<ColorLayoutT::ABGR>> m_tim_clut_data{};

  /**
   * @brief The header of the image data in the TIM file.
   */
  TimImageHeader                           m_tim_image_header{};

  /**
   * @brief The variant holding the actual image data of the TIM file.
   * Can hold 4-bit paletted, 8-bit paletted, 15-bit full color, or 24-bit full
   * color data.
   */
  std::variant<
    std::vector<Bit4Values>,
    std::vector<std::uint8_t>,
    std::vector<Color16<ColorLayoutT::ABGR>>,
    std::vector<Color24<ColorLayoutT::BGR>>>
    m_tim_image_data{};

  /**
   * @brief Get the size of the image data.
   * @return std::size_t Size of the image data.
   */
  [[nodiscard]] std::size_t
    size_of_image_data() const noexcept
  {
    return std::visit(
      [](const auto &variant) {
        return std::ranges::size(variant);
      },
      m_tim_image_data);
  }

  /**
   * @brief Get the color from the CLUT using the provided row and color_key.
   * @param row Row in the CLUT.
   * @param color_key Color key (index) in the row.
   * @param clut_dims Dimensions of the CLUT (width and height).
   * @return Color16<ColorLayoutT::ABGR> Color value from the CLUT.
   */
  [[nodiscard]] Color16<ColorLayoutT::ABGR>
    get_color(
      [[maybe_unused]] const std::uint16_t                         row,
      [[maybe_unused]] const std::uint8_t                          color_key,
      const decltype(m_tim_clut_header.rectangle().width_height()) clut_dims)
      const
  {
    if (
      clut_dims.x() == 0 || clut_dims.y() == 0 || std::empty(m_tim_clut_data)) {
      return {};
    }
    const std::size_t offset = row * clut_dims.x();
    const auto        i      = offset + color_key;
    if (m_tim_clut_data.size() > i) {
      return m_tim_clut_data[i];
    }
    return {};
  }

  /**
   * @brief Get 4-bit paletted colors from the image data.
   * @tparam dstT Destination color type.
   * @param row The row of the Color Lookup Table (CLUT) to use.
   * @param clut_dims The dimensions of the CLUT.
   * @return A vector of colors of type dstT.
   */
  template<typename dstT>
  auto
    get_4bpp_colors(
      const uint16_t                                               row,
      const decltype(m_tim_clut_header.rectangle().width_height()) clut_dims)
      const
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

  /**
   * @brief Get 8-bit paletted colors from the image data.
   * @tparam dstT Destination color type.
   * @param row The row of the Color Lookup Table (CLUT) to use.
   * @param clut_dims The dimensions of the CLUT.
   * @return A vector of colors of type dstT.
   */
  template<typename dstT>
  auto
    get_8bbp_colors(
      const uint16_t                                               row,
      const decltype(m_tim_clut_header.rectangle().width_height()) clut_dims)
      const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data());
    std::ranges::transform(
      std::get<1>(m_tim_image_data),
      std::back_inserter(output),
      [this, &row, &clut_dims](const uint8_t color_index) {
        return static_cast<dstT>(get_color(row, color_index, clut_dims));
      });
    return output;
  }

  /**
   * @brief Get 15-bit (16-bit) full color colors from the image data.
   * @tparam dstT Destination color type.
   * @return A vector of colors of type dstT.
   */
  template<typename dstT>
  auto
    get_16bpp_colors() const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data());
    std::ranges::transform(
      std::get<2>(m_tim_image_data),
      std::back_inserter(output),
      [](const Color16<ColorLayoutT::ABGR> &color) {
        return static_cast<dstT>(color);
      });
    return output;
  }

  /**
   * @brief Get 24-bit full color colors from the image data.
   * @tparam dstT Destination color type.
   * @return A vector of colors of type dstT.
   */
  template<typename dstT>
  auto
    get_24bpp_colors() const
  {
    std::vector<dstT> output{};
    output.reserve(size_of_image_data());
    std::ranges::transform(
      std::get<3>(m_tim_image_data),
      std::back_inserter(output),
      [](const Color24<ColorLayoutT::BGR> &color) {
        return static_cast<dstT>(color);
      });
    return output;
  }

public:
  /**
   * @brief Default constructor.
   */
  Tim() = default;

  /**
   * @brief Construct a Tim object from a buffer.
   * @param buffer A span containing TIM file data.
   */
  explicit Tim(std::span<const char> buffer)
    : m_tim_header(get_tim_header(buffer)),
      m_tim_clut_header(get_tim_clut_header(buffer)),
      m_tim_clut_data(get_tim_clut_data(buffer)),
      m_tim_image_header(get_tim_image_header(buffer)),
      m_tim_image_data(get_tim_image_data(buffer))
  {}

  /**
   * @brief Construct a Tim object with all its components.
   * @param in_tim_header TIM header.
   * @param in_tim_clut_header TIM CLUT header.
   * @param in_tim_clut_data TIM CLUT data.
   * @param in_tim_image_header TIM image header.
   * @param in_tim_image_data TIM image data.
   */
  explicit Tim(
    TimHeader                                in_tim_header,
    TimClutHeader                            in_tim_clut_header,
    std::vector<Color16<ColorLayoutT::ABGR>> in_tim_clut_data,
    TimImageHeader                           in_tim_image_header,
    std::variant<
      std::vector<Bit4Values>,
      std::vector<std::uint8_t>,
      std::vector<Color16<ColorLayoutT::ABGR>>,
      std::vector<Color24<ColorLayoutT::BGR>>> in_tim_image_data)
    : m_tim_header(in_tim_header), m_tim_clut_header(in_tim_clut_header),
      m_tim_clut_data(std::move(in_tim_clut_data)),
      m_tim_image_header(in_tim_image_header),
      m_tim_image_data(std::move(in_tim_image_data))
  {}

  /**
   * @brief Get the TIM header from the buffer.
   * @param buffer A span containing TIM file data.
   * @return The TIM header object.
   */
  [[nodiscard]] static TimHeader
    get_tim_header(std::span<const char> &buffer)
  {
    return tools::read_val<TimHeader>(buffer);
  }

  /**
   * @brief Get the TIM CLUT header from the buffer.
   * @param buffer A span containing TIM file data.
   * @return The TIM CLUT header object.
   */
  [[nodiscard]] TimClutHeader
    get_tim_clut_header(std::span<const char> &buffer) const
  {
    if (
      !m_tim_header.check()
      || !m_tim_header.bpp().color_lookup_table_present()) {
      return {};
    }
    return tools::read_val<TimClutHeader>(buffer);
  }

  /**
   * @brief Get the TIM CLUT data from the buffer.
   * @param buffer A span containing TIM file data.
   * @return A vector containing the TIM CLUT data.
   */
  [[nodiscard]] std::vector<Color16<ColorLayoutT::ABGR>>
    get_tim_clut_data(std::span<const char> &buffer) const
  {
    if (!m_tim_clut_header.check() || m_tim_clut_header.data_size() == 0U) {
      return {};
    }
    return tools::read_vals<Color16ABGR>(
      buffer,
      m_tim_clut_header.data_size()/2U);
  }

  /**
   * @brief Get the TIM image header from the buffer.
   * @param buffer A span containing TIM file data.
   * @return The TIM image header object.
   */
  [[nodiscard]] TimImageHeader
    get_tim_image_header(std::span<const char> &buffer) const
  {
    if (!m_tim_header.check()) {
      return {};
    }
    return tools::read_val<TimImageHeader>(buffer);
  }
  /**
   * @brief Get the TIM image data from the buffer.
   * @param buffer A span containing TIM file data.
   * @return A variant containing the TIM image data.
   */
  [[nodiscard]] decltype(m_tim_image_data)
    get_tim_image_data(std::span<const char> &buffer) const

  /**
   * @brief Check the validity of the TIM object.
   * @return A boolean indicating whether the TIM object is valid or not.
   */
  {
    if (!m_tim_header.check() || m_tim_image_header.data_size() == 0) {
      return {};
    }
    switch (static_cast<int>(m_tim_header.bpp())) {
    case 4: {
      return tools::read_vals<Bit4Values>(
        buffer,
        m_tim_image_header.data_size());
    }
    case 8: {
      return tools::read_vals<std::uint8_t>(
        buffer,
        m_tim_image_header.data_size());
    }
    case 16: {
      return tools::read_vals<Color16ABGR>(
        buffer,
        m_tim_image_header.data_size()/2);
    }
    case 24: {
      return tools::read_vals<Color24BGR>(
        buffer,
        m_tim_image_header.data_size()/3);
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

  /**
   * @brief Get the width of the TIM image.
   * @return The width of the TIM image.
   */
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
      return static_cast<uint16_t>(
        m_tim_image_header.rectangle().width() / bpp24_step);// 24 bpp
    }
    return {};// invalid value
  }

  /**
   * @brief Get the height of the TIM image.
   * @return The height of the TIM image.
   */
  [[nodiscard]] auto
    height() const
  {
    return m_tim_image_header.rectangle().height();
  }

  /**
   * @brief Get the area of the TIM image.
   * @return The area of the TIM image.
   */
  [[nodiscard]] std::size_t
    area() const
  {
    return width()
         * static_cast<std::size_t>(m_tim_image_header.rectangle().height());
  }

  /**
   * @brief Get the X coordinate of the TIM image header.
   * @return The X coordinate of the TIM image header.
   */
  [[maybe_unused]] [[nodiscard]] auto
    x() const
  {
    return m_tim_image_header.rectangle().x();
  }

  /**
   * @brief Get the Y coordinate of the TIM image header.
   * @return The Y coordinate of the TIM image header.
   */
  [[maybe_unused]] [[nodiscard]] auto
    y() const
  {
    return m_tim_image_header.rectangle().y();
  }

  /**
   * @brief Get the X coordinate of the TIM color lookup table (CLUT) header.
   * @return The X coordinate of the TIM CLUT header.
   */
  [[maybe_unused]] [[nodiscard]] auto
    clut_x() const
  {
    return m_tim_clut_header.rectangle().x();
  }

  /**
   * @brief Get the Y coordinate of the TIM color lookup table (CLUT) header.
   * @return The Y coordinate of the TIM CLUT header.
   */
  [[maybe_unused]] [[nodiscard]] auto
    clut_y() const
  {
    return m_tim_clut_header.rectangle().y();
  }

  /**
   * @brief Get the size of the TIM object in bytes.
   * @return The size of the TIM object in bytes.
   */
  [[nodiscard]] auto
    size() const
  {
    return sizeof(m_tim_header) + m_tim_clut_header.size()
         + m_tim_image_header.size();
  }

  /**
   * @brief Get the number of rows in the color lookup table (CLUT).
   * @return The number of rows in the CLUT.
   */
  [[nodiscard]] auto
    clut_rows() const
  {
    return m_tim_clut_header.rectangle().height();
  }

  /**
   * @brief Get the number of colors in the color lookup table (CLUT).
   * @return The number of colors in the CLUT.
   */
  [[maybe_unused]] [[nodiscard]] auto
    clut_colors() const
  {
    return m_tim_clut_header.rectangle().width();
  }

  /**
   * @brief Check if the TIM object is valid.
   * @return True if the TIM object is valid, false otherwise.
   */
  [[nodiscard]] explicit operator bool() const
  {
    return check();
  }

  /**
   * @brief Get the color data of the TIM object.
   * @tparam dstT The color type (default: Tim).
   * @param row The row of the color lookup table (CLUT) to use (default: 0U).
   * @param clut_dims The dimensions of the CLUT (default: {}).
   * @return A vector of color data.
   */
  template<Color dstT = Tim>
  [[nodiscard]] std::vector<dstT>
    get_colors(
      [[maybe_unused]] const std::uint16_t                   row = 0U,
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
   * @brief Update the dimensions of the color lookup table (CLUT).
   * @param width_height The new dimensions (width is the number of colors per
   * palette, height is the number of palettes).
   * @return A new TIM object with the updated CLUT dimensions.
   */
  Tim
    force_tim_clut_dims(Point<std::uint16_t> width_height) const
  {
    if (m_tim_clut_header.rectangle().area() < width_height.area()) {
      // return empty tim if area is too large.
      return {};
    }
    return Tim(
      m_tim_header,
      TimClutHeader(
        m_tim_clut_header.size(),
        Rectangle<std::uint16_t>(
          m_tim_clut_header.rectangle().top_left(),
          width_height)),
      m_tim_clut_data,
      m_tim_image_header,
      m_tim_image_data);
  }

  /**
   * @brief Update the dimensions of the color lookup table (CLUT).
   * @param width The new width (usually the number of colors per palette).
   * @param height The new height (usually the number of palettes).
   * @return A new TIM object with the updated CLUT dimensions.
   */
  [[maybe_unused]] Tim
    force_tim_clut_dims(std::uint16_t width, std::uint16_t height) const
  {
    return force_tim_clut_dims(Point<std::uint16_t>(width, height));
  }

  /**
   * @brief Save the TIM object to a file.
   * @param filename The output file name.
   * @param clut_dims The dimensions of the color lookup table (CLUT) (default:
   * {}).
   * @param clut The index of the CLUT to save (-1 for all, default: -1).
   */
  [[maybe_unused]] void
    save(
      std::string_view                                       filename,
      decltype(m_tim_clut_header.rectangle().width_height()) clut_dims = {},
      const int                                              clut = -1) const
  {
    if (clut_rows() == 0) {
      const auto &data = get_colors<Color16<ColorLayoutT::ABGR>>();
      //Ppm::save(data, width(), height(), filename);
      Png::save(
        data,
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
        //Ppm::save(data, width(), height(), out_path);
        Png::save(data, width(), height(), out_path, out_path);
        if (clut >= 0)
          return;
      }
    }
    if (clut_rows() != 0) {
      auto       path     = std::filesystem::path(filename);
      const auto out_path = (path.parent_path() / path.stem()).string()
                          + "_clut" + path.extension().string();
//      Ppm::save(
//        m_tim_clut_data,
//        m_tim_clut_header.rectangle().width(),
//        m_tim_clut_header.rectangle().height(),
//        out_path);
      Png::save(
        m_tim_clut_data,
        m_tim_clut_header.rectangle().width(),
        m_tim_clut_header.rectangle().height(),
        out_path,
        std::filesystem::path(filename).string());
    }
  }

  /**
   * @brief Get the TIM header.
   * @return The TIM header.
   */
  [[nodiscard]] TimHeader
    tim_header() const noexcept
  {
    return m_tim_header;
  }

  /**
   * @brief Get the TIM CLUT header.
   * @return The TIM CLUT header.
   */
  [[nodiscard]] TimClutHeader
    tim_clut_header() const noexcept
  {
    return m_tim_clut_header;
  }

  /**
   * @brief Get the TIM CLUT data.
   * @return A vector of TIM CLUT data.
   */
  [[nodiscard]] std::vector<Color16<ColorLayoutT::ABGR>>
    tim_clut_data() const noexcept
  {
    return m_tim_clut_data;
  }

  /**
   * @brief Get the TIM image header.
   * @return The TIM image header.
   */
  [[nodiscard]] TimImageHeader
    tim_image_header() const noexcept
  {
    return m_tim_image_header;
  }

  /**
   * @brief Get the TIM image data.
   * @return A reference to the variant holding the TIM image data.
   */
  [[nodiscard]] const std::variant<
    std::vector<Bit4Values>,
    std::vector<std::uint8_t>,
    std::vector<Color16<ColorLayoutT::ABGR>>,
    std::vector<Color24<ColorLayoutT::BGR>>> &
    tim_image_data() const noexcept
  {
    return m_tim_image_data;
  }
};

/**
 * @brief Get the 16bpp colors as a vector of Color16<ColorLayoutT::ABGR>
 * objects.
 * @return A vector of Color16<ColorLayoutT::ABGR> objects representing the
 * 16bpp colors.
 */
template<>
auto
  Tim::get_16bpp_colors<Color16<ColorLayoutT::ABGR>>() const
{
  return std::get<2>(m_tim_image_data);
}

/**
 * @brief Get the 24bpp colors as a vector of Color24<ColorLayoutT::BGR>
 * objects.
 * @return A vector of Color24<ColorLayoutT::BGR> objects representing the 24bpp
 * colors.
 */
template<>
auto
  Tim::get_24bpp_colors<Color24<ColorLayoutT::BGR>>() const
{
  return std::get<3>(m_tim_image_data);
}

/**
 * @brief Stream insertion operator for Tim objects.
 * @param os The output stream to insert the Tim object into.
 * @param input The Tim object to insert into the output stream.
 * @return The output stream with the inserted Tim object.
 */
inline std::ostream &
  operator<<(std::ostream &os, const Tim &input)
{
  return os << '{' << input.tim_header() << ", " << input.tim_clut_header()
            << ", " << input.tim_image_header()
            << ", Corrected Width: " << input.width() << '}';
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIM_HPP