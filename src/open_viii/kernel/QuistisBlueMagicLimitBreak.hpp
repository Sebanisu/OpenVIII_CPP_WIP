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

#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_HPP
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_HPP
#include "AttackFlagsT.hpp"
#include "AttackTypeT.hpp"
#include "ElementT.hpp"
#include "open_viii/Strings/EncodedStringOffset.hpp"
namespace open_viii::kernel {
template<LangT langVal> struct QuistisBlueMagicLimitBreak
{
  /* https://github.com/DarkShinryu/doomtrain/wiki/Blue-magic-(Quistis-limit-break)
   * 0x0000	2 bytes	Offset to limit name
   * 0x0002	2 bytes	Offset to limit description
   * 0x0004	2 bytes	Magic ID
   * 0x0006	1 byte	Unknown
   * 0x0007	1 byte	Attack Type
   * 0x0008	2 bytes	Unknown
   * 0x000A	1 byte	Attack Flags
   * 0x000B	1 byte	Unknown
   * 0x000C	1 byte	Element
   * 0x000D	1 byte	Status Attack
   * 0x000E	1 byte	Critical Bonus
   * 0x000F	1 byte	Unknown
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};

  std::uint16_t m_magic_id{};
  std::uint8_t m_unknown0{};
  AttackTypeT m_attack_type{};
  std::uint8_t m_unknown1{};
  std::uint8_t m_unknown2{};
  AttackFlagsT m_attack_flags{};
  std::uint8_t m_unknown3{};
  ElementT m_element{};
  std::uint8_t m_status_attack{};
  std::uint8_t m_critical_bonus{};
  std::uint8_t m_unknown4{};

public:
  [[nodiscard]] auto &name_offset() const noexcept
  {
    return m_name_offset;
  }
  [[nodiscard]] auto &description_offset() const noexcept
  {
    return m_description_offset;
  }
  [[nodiscard]] auto magic_id() const noexcept
  {
    return m_magic_id;
  }
  [[nodiscard]] auto unknown0() const noexcept
  {
    return m_unknown0;
  }
  [[nodiscard]] auto attack_type() const noexcept
  {
    return m_attack_type;
  }
  [[nodiscard]] auto unknown1() const noexcept
  {
    return m_unknown1;
  }
  [[nodiscard]] auto unknown2() const noexcept
  {
    return m_unknown2;
  }
  [[nodiscard]] auto attack_flags() const noexcept
  {
    return m_attack_flags;
  }
  [[nodiscard]] auto unknown3() const noexcept
  {
    return m_unknown3;
  }
  [[nodiscard]] auto element() const noexcept
  {
    return m_element;
  }
  [[nodiscard]] auto status_attack() const noexcept
  {
    return m_status_attack;
  }
  [[nodiscard]] auto critical_bonus() const noexcept
  {
    return m_critical_bonus;
  }
  [[nodiscard]] auto unknown4() const noexcept
  {
    return m_unknown4;
  }

  std::ostream &out(std::ostream &os, const std::span<const char> &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << tools::u8_to_sv(description);
    }
    return os << ", " << static_cast<std::uint32_t>(m_magic_id) << ", " << static_cast<std::uint32_t>(m_unknown0)
              << ", " << static_cast<std::uint32_t>(m_attack_type) << ", " << static_cast<std::uint32_t>(m_unknown1)
              << ", " << static_cast<std::uint32_t>(m_unknown2) << ", " << static_cast<std::uint32_t>(m_attack_flags)
              << ", " << static_cast<std::uint32_t>(m_unknown3) << ", " << static_cast<std::uint32_t>(m_element) << ", "
              << static_cast<std::uint32_t>(m_status_attack) << ", " << static_cast<std::uint32_t>(m_critical_bonus)
              << ", " << static_cast<std::uint32_t>(m_unknown4);
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAK_HPP
