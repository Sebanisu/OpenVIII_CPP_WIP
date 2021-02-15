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
#ifndef VIIIARCHIVE_RINOALIMITBREAKPART1_HPP
#define VIIIARCHIVE_RINOALIMITBREAKPART1_HPP
#include "TargetT.hpp"
#include "open_viii/strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
struct RinoaLimitBreakPart1
{
  /**
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	Unknown Flags
   * 0x0005	1 byte	Target
   * 0x0006	1 byte	Ability data ID
   * 0x0007	1 byte	Unknown / Unused
   * @see
   * https://github.com/DarkShinryu/doomtrain/wiki/Rinoa-limit-breaks-(part-1)
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t        m_unknown_flags0{};
  TargetT             m_target{};
  std::uint8_t        m_ability_data_id{};
  std::uint8_t        m_unknown0{};

public:
  constexpr auto
    operator<=>(const RinoaLimitBreakPart1 &right) const noexcept = default;
  [[nodiscard]] constexpr auto name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] constexpr auto description_offset() const noexcept
  {
    return m_description_offset;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto unknown_flags0() const noexcept
  {
    return m_unknown_flags0;
  }
  [[nodiscard]] constexpr auto target() const noexcept
  {
    return m_target;
  }
  [[maybe_unused]] [[nodiscard]] constexpr auto ability_data_id() const noexcept
  {
    return m_ability_data_id;
  }
  [[nodiscard]] constexpr auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  std::ostream &out(std::ostream &                                os,
                    [[maybe_unused]] const std::span<const char> &buffer) const
  {
    return os << ", " << static_cast<std::uint32_t>(m_unknown_flags0) << ", "
              << static_cast<std::uint32_t>(m_target) << ", "
              << static_cast<std::uint32_t>(m_ability_data_id) << ", "
              << static_cast<std::uint32_t>(m_unknown0);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_RINOALIMITBREAKPART1_HPP
