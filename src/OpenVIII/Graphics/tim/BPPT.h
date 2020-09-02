//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_BPPT_H
#define VIIIARCHIVE_BPPT_H
#include <iostream>
namespace open_viii::graphics {
/**
 * @enum open_viii::graphics::BPPT
 * \ingroup Graphics
 * @brief Contains 4 to 24 bits per pixel flags. Also a CLP flag for color lookup table.
 * @note change to default initiated cpp20 style.
 */
struct BPPT
{
private:
  bool m_bpp8 : 1 { false };
  bool m_bpp16 : 1 { false };
  bool m_unused1 : 1 { false };
  bool m_color_lookup_table_present : 1 { false };

  bool m_unused2 : 1 { false };
  bool m_unused3 : 1 { false };
  bool m_unused4 : 1 { false };
  bool m_unused5 : 1 { false };

public:
  [[nodiscard]] bool unused() const { return m_unused1 && m_unused2 && m_unused3 && m_unused4 && m_unused5; }
  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are not set;
   * @return true if 4bpp
   */
  [[nodiscard]] bool bpp4() const { return !unused() && !m_bpp8 && !m_bpp16 && m_color_lookup_table_present; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and 16bpp is not set;
   * @return true if 8bpp
   */
  [[nodiscard]] bool bpp8() const { return !unused() && m_bpp8 && !m_bpp16 && m_color_lookup_table_present; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set and 16bpp is set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool bpp16() const { return !unused() && !m_bpp8 && m_bpp16 && !m_color_lookup_table_present; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and 16bpp is set;
   * @return true if 24bpp
   */
  [[nodiscard]] bool bpp24() const { return !unused() && m_bpp8 && m_bpp16 && !m_color_lookup_table_present; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is ignored and 16bpp is not set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool color_lookup_table_present() const { return !unused() && !m_bpp16 && m_color_lookup_table_present; }
  /**
   * Test that one of the valid states is true.
   * @return true if is a valid state.
   */
  [[nodiscard]] bool check() const { return bpp4() || bpp8() || bpp16() || bpp24(); }
  [[nodiscard]] explicit operator bool() const { return check(); }
  [[nodiscard]] explicit operator int() const
  {
    //[[maybe_unused]] static constexpr auto size_ = sizeof(BPPT);
    if (bpp4()) {
      const int ret_val = 4;
      return ret_val;
    }
    if (bpp8()) {
      const int ret_val = 8;
      return ret_val;
    }
    if (bpp16()) {
      const int ret_val = 16;
      return ret_val;
    }
    if (bpp24()) {
      const int ret_val = 24;
      return ret_val;
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
    return os << "{BPP: " << static_cast<int>(input) << ", CLP: " << input.m_color_lookup_table_present << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_BPPT_H
