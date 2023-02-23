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
#ifndef VIIIARCHIVE_BPPT_HPP
#define VIIIARCHIVE_BPPT_HPP
#include <compare>
#include <iostream>
#include <string_view>
namespace open_viii::graphics {
/**
 * @enum open_viii::graphics::BPPT
 * \ingroup graphics
 * @brief Contains 4 to 24 bits per pixel flags. Also a CLP flag for color
 * lookup table.
 * @todo make immutable. And since there are only 4 valid states. Have for
 * static constexpr initializers.
 */
struct BPPT
{
private:
  bool                  m_bpp8                       : 1 { false };
  bool                  m_bpp16                      : 1 { false };
  /**
   * might be used sometimes. for some files. I think it could be for multi
   * format
   */
  bool                  m_unused1                    : 1 { false };
  bool                  m_color_lookup_table_present : 1 { false };
  bool                  m_unused2                    : 1 { false };
  bool                  m_unused3                    : 1 { false };
  bool                  m_unused4                    : 1 { false };
  bool                  m_unused5                    : 1 { false };
  constexpr static auto RAW8_VALUE  = 0b0001U;
  constexpr static auto RAW16_VALUE = 0b0010U;
  constexpr static auto CLP_VALUE   = 0b1000U;

public:
  constexpr static auto BPP4  = 4U;
  constexpr static auto BPP8  = 8U;
  constexpr static auto BPP16 = 16U;
  constexpr static auto BPP24 = 24U;
  auto
    operator<=>(const BPPT &) const = default;
  [[nodiscard]] constexpr bool
    unused() const noexcept
  {
    return m_unused1 && m_unused2 && m_unused3 && m_unused4 && m_unused5;
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp and 16bpp are
   * not set;
   * @return true if 4bpp
   */
  [[nodiscard]] constexpr bool
    bpp4() const noexcept
  {
    return !unused() && !m_bpp8 && !m_bpp16 && m_color_lookup_table_present;
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp is set and
   * 16bpp is not set;
   * @return true if 8bpp
   */
  [[nodiscard]] constexpr bool
    bpp8() const noexcept
  {
    return !unused() && m_bpp8 && !m_bpp16 && m_color_lookup_table_present;
  }
  /**
   * Test bits to check if color lookup table is not present and 8bpp is not set
   * and 16bpp is set;
   * @return true if 16bpp
   */
  [[nodiscard]] constexpr bool
    bpp16() const noexcept
  {
    return !unused() && !m_bpp8 && m_bpp16 && !m_color_lookup_table_present;
  }
  /**
   * Test bits to check if color lookup table is not present and 8bpp is set and
   * 16bpp is set;
   * @return true if 24bpp
   */
  [[nodiscard]] constexpr bool
    bpp24() const noexcept
  {
    return !unused() && m_bpp8 && m_bpp16 && !m_color_lookup_table_present;
  }
  /**
   * Test bits to check if color lookup table is present and 8bpp is ignored and
   * 16bpp is not set;
   * @return true if 16bpp
   */
  [[nodiscard]] constexpr bool
    color_lookup_table_present() const
  {
    return !unused() && !m_bpp16 && m_color_lookup_table_present;
  }
  /**
   * Test that one of the valid states is true.
   * @return true if is a valid state.
   */
  [[nodiscard]] constexpr bool
    check() const noexcept
  {
    return bpp4() || bpp8() || bpp16() || bpp24();
  }
  [[nodiscard]] constexpr explicit operator bool() const noexcept
  {
    return check();
  }
  [[nodiscard]] constexpr explicit operator int() const
  {
    //[[maybe_unused]] static constexpr auto size_ = sizeof(BPPT);
    if (bpp4()) {
      return BPP4;
    }
    if (bpp8()) {
      return BPP8;
    }
    if (bpp16()) {
      return BPP16;
    }
    if (bpp24()) {
      return BPP24;
    }
    return 0;
  }
  [[nodiscard]] constexpr explicit operator unsigned int() const
  {
    //[[maybe_unused]] static constexpr auto size_ = sizeof(BPPT);
    if (bpp4()) {
      return BPP4;
    }
    if (bpp8()) {
      return BPP8;
    }
    if (bpp16()) {
      return BPP16;
    }
    if (bpp24()) {
      return BPP24;
    }
    return 0;
  }
  //  _4bpp = 0b0U, /**< is coded as std::uint8_t of value 0. 4 bits per per
  //  pixel */ _8bpp = 0b1U, /**< is coded as std::uint8_t of value 1. 8 bits
  //  per per pixel */ _16bpp = 0b10U, /**< is coded as std::uint8_t of value 2.
  //  16 bits per per pixel */ _24bpp = _8bpp | _16bpp, /**< is coded as
  //  std::uint8_t of value 3. 24 bits per per pixel */ CLP = 0b1000, /**< is
  //  coded as std::uint8_t of value 8. CLP flag for color lookup table present.
  //  This is required for
  //                   4bpp and 8bpp. */
  constexpr std::uint8_t
    raw() const noexcept
  {
    return static_cast<std::uint8_t>(
      (m_bpp8 ? RAW8_VALUE : 0U) + (m_bpp16 ? RAW16_VALUE : 0U)
      + (m_color_lookup_table_present ? CLP_VALUE : 0U));
  }
  constexpr BPPT() = default;
  constexpr BPPT(
    bool in_bpp8,
    bool in_bpp16,
    bool in_color_lookup_table_present)
    : m_bpp8(in_bpp8), m_bpp16(in_bpp16),
      m_color_lookup_table_present(in_color_lookup_table_present)
  {}
  static consteval BPPT
    BPP4_CONST() noexcept
  {
    return { false, false, true };
  }

  static consteval BPPT
    BPP8_CONST() noexcept
  {
    return { true, false, true };
  }

  static consteval BPPT
    BPP16_CONST() noexcept
  {
    return { false, true, false };
  }

  static consteval BPPT
    BPP24_CONST() noexcept
  {
    return { true, true, false };
  };
};

inline std::ostream &
  operator<<(std::ostream &os, [[maybe_unused]] const BPPT &input)
{
  return os << "{BPP: " << static_cast<int>(input)
            << ", CLP: " << input.color_lookup_table_present() << '}';
}
namespace literals {
  consteval BPPT operator""_bpp(const char *const value)
  {
    const auto sv = std::string_view(value);

    if ((sv.size() == 1U)) {
      if (sv[0] == '4') {
        return BPPT::BPP4_CONST();
      }
      if (sv[0] == '8') {
        return BPPT::BPP8_CONST();
      }
    }
    else if ((sv.size() == 2U)) {
      using namespace std::string_view_literals;
      if (sv == "16"sv) {
        return BPPT::BPP16_CONST();
      }
      if (sv == "24"sv) {
        return BPPT::BPP24_CONST();
      }
    }
    throw std::invalid_argument("value must be 4, 8, 16, or 24");
  }
  static_assert(4_bpp != 8_bpp);
  static_assert(16_bpp != 24_bpp);
}// namespace literals
static_assert(sizeof(BPPT) == 1U);
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_BPPT_HPP