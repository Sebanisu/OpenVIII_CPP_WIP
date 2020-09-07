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

  constexpr static auto BPP4{ 4U };
  constexpr static auto BPP8{ 8U };
  constexpr static auto BPP16{ 16U };
  constexpr static auto BPP24{ 24U };

  constexpr static auto RAW8_VALUE = 0b1U;
  constexpr static auto RAW16_VALUE = 0b10U;
  constexpr static auto CLP_VALUE = 0b1000U;

public:
  consteval friend BPPT operator"" _bpp(unsigned long long int value);
  constexpr friend auto operator==(const BPPT &left, const BPPT &right)
  {
    return left.m_bpp8 == right.m_bpp8 && left.m_bpp16 == right.m_bpp16// && left.m_unused1 == right.m_unused1
           && left.m_color_lookup_table_present == right.m_color_lookup_table_present;
    //           && left.m_unused2 == right.m_unused2 && left.m_unused3 == right.m_unused3
    //           && left.m_unused4 == right.m_unused4 && left.m_unused5 == right.m_unused5;
  }
  constexpr friend auto operator<(const BPPT &left, const BPPT &right)
  {
    return left.m_bpp8 < right.m_bpp8 && left.m_bpp16 < right.m_bpp16// && left.m_unused1 < right.m_unused1
           && left.m_color_lookup_table_present < right.m_color_lookup_table_present;
    //           && left.m_unused2 < right.m_unused2
    //           && left.m_unused3 < right.m_unused3 && left.m_unused4 < right.m_unused4 && left.m_unused5 <
    //           right.m_unused5;
  }

  constexpr friend auto operator>(const BPPT &left, const BPPT &right)
  {
    return left.m_bpp8 > right.m_bpp8 && left.m_bpp16 > right.m_bpp16// && left.m_unused1 > right.m_unused1
           && left.m_color_lookup_table_present
                > right.m_color_lookup_table_present;// && left.m_unused2 > right.m_unused2
    //&& left.m_unused3 > right.m_unused3 && left.m_unused4 > right.m_unused4 && left.m_unused5 > right.m_unused5;
  }
  [[nodiscard]] constexpr bool unused() const noexcept
  {
    return m_unused1 && m_unused2 && m_unused3 && m_unused4 && m_unused5;
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are not set;
   * @return true if 4bpp
   */
  constexpr void bpp4(bool in) const noexcept
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
  constexpr void bpp8(bool in) const
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
  constexpr void bpp16(bool in) const
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
  constexpr void bpp24(bool in) const
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
  [[nodiscard]] constexpr bool bpp4() const noexcept
  {
    return !unused() && !m_bpp8 && !m_bpp16 && m_color_lookup_table_present;
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and 16bpp is not set;
   * @return true if 8bpp
   */
  [[nodiscard]] constexpr bool bpp8() const noexcept
  {
    return !unused() && m_bpp8 && !m_bpp16 && m_color_lookup_table_present;
  }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set and 16bpp is set;
   * @return true if 16bpp
   */
  [[nodiscard]] constexpr bool bpp16() const noexcept
  {
    return !unused() && !m_bpp8 && m_bpp16 && !m_color_lookup_table_present;
  }

  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and 16bpp is set;
   * @return true if 24bpp
   */
  [[nodiscard]] constexpr bool bpp24() const noexcept
  {
    return !unused() && m_bpp8 && m_bpp16 && !m_color_lookup_table_present;
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp is ignored and 16bpp is not set;
   * @return true if 16bpp
   */
  [[nodiscard]] constexpr bool color_lookup_table_present() const
  {
    return !unused() && !m_bpp16 && m_color_lookup_table_present;
  }
  /**
   * Test that one of the valid states is true.
   * @return true if is a valid state.
   */
  [[nodiscard]] constexpr bool check() const noexcept { return bpp4() || bpp8() || bpp16() || bpp24(); }
  [[nodiscard]] constexpr explicit operator bool() const noexcept { return check(); }
  [[nodiscard]] constexpr explicit operator int() const
  {
    //[[maybe_unused]] static constexpr auto size_ = sizeof(BPPT);
    if (bpp4()) {
      const int ret_val = BPP4;
      return ret_val;
    }
    if (bpp8()) {
      const int ret_val = BPP8;
      return ret_val;
    }
    if (bpp16()) {
      const int ret_val = BPP16;
      return ret_val;
    }
    if (bpp24()) {
      const int ret_val = BPP24;
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
  constexpr std::uint8_t raw() const noexcept
  {
    return static_cast<std::uint8_t>(
      (m_bpp8 ? RAW8_VALUE : 0U) + (m_bpp16 ? RAW16_VALUE : 0U) + (m_color_lookup_table_present ? CLP_VALUE : 0U));
  }
  friend std::ostream &operator<<(std::ostream &os, [[maybe_unused]] const BPPT &input)
  {
    return os << "{BPP: " << static_cast<int>(input) << ", CLP: " << input.m_color_lookup_table_present << '}';
  }
};
consteval BPPT operator""_bpp(unsigned long long int value)
{
  BPPT r{};
  switch (value) {
  case BPPT::BPP4:
    r.bpp4(true);
    break;
  case BPPT::BPP8:
    r.bpp8(true);
    break;
  case BPPT::BPP16:
    r.bpp16(true);
    break;
  case BPPT::BPP24:
    r.bpp24(true);
    break;
  }
  return r;
}
static_assert(sizeof(BPPT) == 1U);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_BPPT_H