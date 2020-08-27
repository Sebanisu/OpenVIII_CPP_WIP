//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_BPPT_H
#define VIIIARCHIVE_BPPT_H
#include <iostream>
namespace OpenVIII::Graphics {
/**
 * @enum OpenVIII::Graphics::BPPT
 * \ingroup Graphics
 * @brief Contains 4 to 24 bits per pixel flags. Also a CLP flag for color lookup table.
 * @note change to default initiated cpp20 style.
 */
struct BPPT
{
private:
  bool _8bpp_ : 1 { false };
  bool _16bpp_ : 1 { false };
  bool unused1 : 1 { false };
  bool colorLookupTablePresent_ : 1 { false };

  bool unused2 : 1 { false };
  bool unused3 : 1 { false };
  bool unused4 : 1 { false };
  bool unused5 : 1 { false };

public:
  [[nodiscard]] bool unused() const { return unused1 && unused2 && unused3 && unused4 && unused5; }
  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are not set;
   * @return true if 4bpp
   */
  [[nodiscard]] bool _4bpp() const { return !unused() && !_8bpp_ && !_16bpp_ && colorLookupTablePresent_; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and 16bpp is not set;
   * @return true if 8bpp
   */
  [[nodiscard]] bool _8bpp() const { return !unused() && _8bpp_ && !_16bpp_ && colorLookupTablePresent_; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set and 16bpp is set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool _16bpp() const { return !unused() && !_8bpp_ && _16bpp_ && !colorLookupTablePresent_; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and 16bpp is set;
   * @return true if 24bpp
   */
  [[nodiscard]] bool _24bpp() const { return !unused() && _8bpp_ && _16bpp_ && !colorLookupTablePresent_; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is ignored and 16bpp is not set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool ColorLookupTablePresent() const { return !unused() && !_16bpp_ && colorLookupTablePresent_; }
  /**
   * Test that one of the valid states is true.
   * @return true if is a valid state.
   */
  [[nodiscard]] bool Check() const { return _4bpp() || _8bpp() || _16bpp() || _24bpp(); }
  [[nodiscard]] explicit operator bool() const { return Check(); }
  [[nodiscard]] explicit operator int() const
  {
    //[[maybe_unused]] static constexpr auto size_ = sizeof(BPPT);
    if (_4bpp()) {
      const int retVal = 4;
      return retVal;
    }
    if (_8bpp()) {
      const int retVal = 8;
      return retVal;
    }
    if (_16bpp()) {
      const int retVal = 16;
      return retVal;
    }
    if (_24bpp()) {
      const int retVal = 24;
      return retVal;
    }
    return 0;
  }
  //  _4bpp = 0b0U, /**< is coded as std::uint8_t of value 0. 4 bits per per pixel */
  //  _8bpp = 0b1U, /**< is coded as std::uint8_t of value 1. 8 bits per per pixel */
  //  _16bpp = 0b10U, /**< is coded as std::uint8_t of value 2. 16 bits per per pixel */
  //  _24bpp = _8bpp | _16bpp, /**< is coded as std::uint8_t of value 3. 24 bits per per pixel */
  //  CLP = 0b1000, /**< is coded as std::uint8_t of value 8. CLP flag for color lookup table present. This is required
  //  for
  //                   4bpp and 8bpp. */

  friend std::ostream &operator<<(std::ostream &os, const BPPT &input)
  {
    return os << "{BPP: " << static_cast<int>(input) << ", CLP: " << input.colorLookupTablePresent_ << '}';
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_BPPT_H
