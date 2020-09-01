//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_TIMCLUTHEADER_H
#define VIIIARCHIVE_TIMCLUTHEADER_H
#include "timImageHeader.h"
namespace OpenVIII::Graphics {
/**
 * @struct OpenVIII::Graphics::timClutHeader
 * @ingroup Graphics
 * @brief Color Lookup Table Header
 */
struct timClutHeader
{
private:
  /**
   * @brief X value must be divisible by 16
   */
  static constexpr auto XdivisableBy_{ 16U };
  /**
   * @brief Y max value is 511
   */
  static constexpr auto MaxY_{ 511U };

//  /**
//   * @brief Valid width values
//   */
//  static constexpr std::array ValidWidth_ = { 16U, 256U };

  timImageHeader imageHeader_{};

public:
  /**
   * Typically the width = number of colors, and height = number of color lookup tables. Sometimes if there is only 16
   * colors (4bpp) there is multiple groups of 16 in the table.
   * @brief Dimensions of the color lookup table.
   */
  [[nodiscard]] auto Rectangle() const { return imageHeader_.Rectangle(); }
  /**
   * Total size of Color Lookup Table including header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto size() const { return imageHeader_.size(); };

  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto dataSize() const { return imageHeader_.dataSize(); }
  /**
   * Test X and Y of rectangle to see if they are valid values.
   * Width is usually number of colors should be 16 or 256.
   * @return returns true if valid
   */
  [[nodiscard]] bool Check() const
  {
    return imageHeader_.Rectangle().X() % XdivisableBy_ == 0 && imageHeader_.Rectangle().Y() <= MaxY_;
           //&& Tools::any_of(imageHeader_.Rectangle().Width(), ValidWidth_);
  }

  friend std::ostream &operator<<(std::ostream &os, const timClutHeader &input)
  {
    return os << input.imageHeader_ << " {Colors:" << input.imageHeader_.Rectangle().Width()
              << ", Tables: " << input.imageHeader_.Rectangle().Height() << '}';
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TIMCLUTHEADER_H
