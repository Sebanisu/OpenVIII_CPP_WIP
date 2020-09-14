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

#ifndef VIIIARCHIVE_TIMHEADER_H
#define VIIIARCHIVE_TIMHEADER_H
#include <cstdint>
#include <iostream>
#include "OpenVIII/Graphics/BPPT.h"
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::timHeader
 * @ingroup Graphics
 * @brief Color Lookup Table Header
 */
struct TimHeader
{
private:
  /**
   * @brief expected value of tag_ is 0x10.
   */
  static constexpr auto TAG_VAL = 0x10;
  /**
   * @brief expected value of version_ is 0x0.
   */
  static constexpr auto VERSION_VAL = 0x0;
  /**
   * @brief tag_ is first byte of a TIM file.
   */
  std::uint8_t m_tag{};
  /**
   * @brief version_ is second byte of a TIM file.
   */
  std::uint8_t m_version{};

  /**
   * @brief value should be 0.
   */
  std::uint8_t m_none0{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t m_none1{};

  /**
   * @brief bits per pixel flags and CLP flag
   */
  BPPT m_bpp{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t m_none2{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t m_none3{};
  /**
   * @brief value should be 0 or 22
   */
  std::uint8_t m_none4{};

public:
  /**
   * @return bits per pixel flags and CLP flag
   */
  [[nodiscard]] auto bpp() const { return m_bpp; }
  /**
   * All none values should be 0. version_ should be versionVal_. tag_ should be tagVal_. Only valid values for bpp are
   * 4bpp with CLP, 8bpp with CLP, 16bpp, or 24 bpp.
   * @brief Test member variables for valid values.
   * @return true if Check test passes.
   */
  [[nodiscard]] bool check() const
  {
    return m_none0 == 0 && m_none1 == 0 && m_none2 == 0 && m_none3 == 0
           && /*(none4_ == 0 || none4_ == 22) &&*/ m_tag == TAG_VAL && m_version == VERSION_VAL && m_bpp.check();
  }

  friend std::ostream &operator<<(std::ostream &os, const TimHeader &input)
  {
    //[[maybe_unused]]static constexpr auto size_ = sizeof(input);
    return os << "{ Tag: " << static_cast<uint32_t>(input.m_tag)
              << ", Version: " << static_cast<uint32_t>(input.m_version) << ", " << input.m_bpp << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMHEADER_H
