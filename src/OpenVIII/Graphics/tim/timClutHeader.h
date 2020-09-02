//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_TIMCLUTHEADER_H
#define VIIIARCHIVE_TIMCLUTHEADER_H
#include "timImageHeader.h"
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::timClutHeader
 * @ingroup Graphics
 * @brief Color Lookup Table Header
 */
struct TimClutHeader
{
private:
  /**
   * X value must be divisible by 4, the spec says 16 this but theres some values that aren't standard.
   * @brief X value must be divisible by 4;
   */
  static constexpr auto XDIVISABLE_BY{ 4U };
  /**
   * @brief Y max value is 511
   */
  static constexpr auto MAX_Y{ 511U };

  /**
   * 4 bit can only read up to 16 values and 8 bit can only read up to 256 values. But There are larger other sizes. The game uses
   * @brief Valid width values; some non-standard width values exist so this isn't used.
   */
   [[maybe_unused]]static constexpr std::array VALID_WIDTH = { 16U, 256U };

  TimImageHeader m_image_header{};

public:
  /**
   * Typically the width = number of colors, and height = number of color lookup tables. Sometimes if there is only 16
   * colors (4bpp) there is multiple groups of 16 in the table.
   * @brief Dimensions of the color lookup table.
   */
  [[nodiscard]] auto rectangle() const { return m_image_header.rectangle(); }
  /**
   * Total size of Color Lookup Table including header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto size() const { return m_image_header.size(); };

  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto data_size() const { return m_image_header.data_size(); }
  /**
   * Test X and Y of rectangle to see if they are valid values.
   * Width is usually number of colors should be 16 or 256.
   * @return returns true if valid
   */
  [[nodiscard]] bool check() const
  {
    return m_image_header.rectangle().x() % XDIVISABLE_BY == 0 && m_image_header.rectangle().y() <= MAX_Y;
           //&& Tools::any_of(imageHeader_.rectangle().width(), ValidWidth_); // some non standard sizes.
  }

  friend std::ostream &operator<<(std::ostream &os, const TimClutHeader &input)
  {
    return os << input.m_image_header << " {Colors:" << input.m_image_header.rectangle().width()
              << ", Tables: " << input.m_image_header.rectangle().height() << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMCLUTHEADER_H
