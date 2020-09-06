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
  mutable bool m_bpp8 : 1 { false };
  mutable bool m_bpp16 : 1 { false };
  mutable bool m_unused1 : 1 { false };
  mutable bool m_color_lookup_table_present : 1 { false };

  bool m_unused2 : 1 { false };
  bool m_unused3 : 1 { false };
  bool m_unused4 : 1 { false };
  bool m_unused5 : 1 { false };

public:
  friend auto operator==(const BPPT &left, const BPPT &right)
  {
    return left.m_bpp8 == right.m_bpp8 && left.m_bpp16 == right.m_bpp16 && left.m_unused1 == right.m_unused1
           && left.m_color_lookup_table_present == right.m_color_lookup_table_present
           && left.m_unused2 == right.m_unused2 && left.m_unused3 == right.m_unused3
           && left.m_unused4 == right.m_unused4 && left.m_unused5 == right.m_unused5;
  }
  friend auto operator<(const BPPT &left, const BPPT &right)
  {
    return left.m_bpp8 < right.m_bpp8 && left.m_bpp16 < right.m_bpp16 && left.m_unused1 < right.m_unused1
           && left.m_color_lookup_table_present < right.m_color_lookup_table_present && left.m_unused2 < right.m_unused2
           && left.m_unused3 < right.m_unused3 && left.m_unused4 < right.m_unused4 && left.m_unused5 < right.m_unused5;
  }

  friend auto operator>(const BPPT &left, const BPPT &right)
  {
    return left.m_bpp8 > right.m_bpp8 && left.m_bpp16 > right.m_bpp16 && left.m_unused1 > right.m_unused1
           && left.m_color_lookup_table_present > right.m_color_lookup_table_present && left.m_unused2 > right.m_unused2
           && left.m_unused3 > right.m_unused3 && left.m_unused4 > right.m_unused4 && left.m_unused5 > right.m_unused5;
  }
  [[nodiscard]] bool unused() const noexcept { return m_unused1 && m_unused2 && m_unused3 && m_unused4 && m_unused5; }
  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are not set;
   * @return true if 4bpp
   */
  void bpp4(bool in) const noexcept
  {
    if (in) {
      m_bpp8 = false;
      m_bpp16 = false;
      m_color_lookup_table_present = true;
    }
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and 16bpp is not set;
   * @return true if 8bpp
   */
  void bpp8(bool in) const
  {
    if (in) {
      m_bpp8 = true;
      m_bpp16 = false;
      m_color_lookup_table_present = true;
    }
  }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set and 16bpp is set;
   * @return true if 16bpp
   */
  void bpp16(bool in) const
  {
    if (in) {
      m_bpp8 = false;
      m_bpp16 = true;
      m_color_lookup_table_present = false;
    }
  }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and 16bpp is set;
   * @return true if 24bpp
   */
  void bpp24(bool in) const
  {
    if (in) {
      m_bpp8 = true;
      m_bpp16 = true;
      m_color_lookup_table_present = false;
    }
  }

  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are not set;
   * @return true if 4bpp
   */
  [[nodiscard]] bool bpp4() const noexcept { return !unused() && !m_bpp8 && !m_bpp16 && m_color_lookup_table_present; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and 16bpp is not set;
   * @return true if 8bpp
   */
  [[nodiscard]] bool bpp8() const noexcept { return !unused() && m_bpp8 && !m_bpp16 && m_color_lookup_table_present; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set and 16bpp is set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool bpp16() const noexcept { return !unused() && !m_bpp8 && m_bpp16 && !m_color_lookup_table_present; }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and 16bpp is set;
   * @return true if 24bpp
   */
  [[nodiscard]] bool bpp24() const noexcept { return !unused() && m_bpp8 && m_bpp16 && !m_color_lookup_table_present; }
  /**
   * Test bits to check if color lookup table is present and 8bpp is ignored and 16bpp is not set;
   * @return true if 16bpp
   */
  [[nodiscard]] bool color_lookup_table_present() const
  {
    return !unused() && !m_bpp16 && m_color_lookup_table_present;
  }
  /**
   * Test that one of the valid states is true.
   * @return true if is a valid state.
   */
  [[nodiscard]] bool check() const noexcept { return bpp4() || bpp8() || bpp16() || bpp24(); }
  [[nodiscard]] explicit operator bool() const noexcept { return check(); }
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
  std::uint8_t raw() const noexcept
  {
    constexpr static auto clp_value = 8U;
    return static_cast<std::uint8_t>(
      (m_bpp8 ? 1U : 0U) + (m_bpp16 ? 2U : 0U) + (m_color_lookup_table_present ? clp_value : 0U));
  }
  friend std::ostream &operator<<(std::ostream &os, const BPPT &input)
  {
    return os << "{BPP: " << static_cast<int>(input) << ", CLP: " << input.m_color_lookup_table_present << '}';
  }
};
static_assert(sizeof(BPPT) == 1U);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_BPPT_H
