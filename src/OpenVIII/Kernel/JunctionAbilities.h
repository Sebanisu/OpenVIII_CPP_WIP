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

#ifndef VIIIARCHIVE_JUNCTIONABILITIES_H
#define VIIIARCHIVE_JUNCTIONABILITIES_H

#include "OpenVIII/Strings/EncodedStringOffset.h"
#include <cstring>
#include "JunctionFlagsT.h"
namespace open_viii::kernel {
template<LangT langVal> struct JunctionAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Junction-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	J-Flag
   */
private:
  EncodedStringOffset m_name_offset{};
  EncodedStringOffset m_description_offset{};
  std::uint8_t m_ap_required{};
  std::array<std::uint8_t, 3> m_junction_flags{};

public:
  [[nodiscard]] auto &name_offset() const noexcept { return m_name_offset; }
  [[nodiscard]] auto &description_offset() const noexcept { return m_description_offset; }
  [[nodiscard]] auto ap_required() const noexcept { return m_ap_required; }
  [[nodiscard]] auto junction_flags() const
  {
    JunctionFlagsT out{};
    std::memcpy(&out, m_junction_flags.data(), m_junction_flags.size());
    return out;
  }
  std::ostream &out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = m_name_offset.decoded_string<langVal>(buffer);
    auto description = m_description_offset.decoded_string<langVal>(buffer);
    if (!std::empty(name)) {
      os << Tools::u8_to_sv(name);
    }
    if (!std::empty(description)) {
      os << ", " << Tools::u8_to_sv(description);
    }
    return os << ", " << static_cast<uint32_t>(m_ap_required) << ", " << static_cast<uint32_t>(junction_flags());
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_JUNCTIONABILITIES_H
