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
#ifndef VIIIARCHIVE_REFINESECTION002_HPP
#define VIIIARCHIVE_REFINESECTION002_HPP
#include "RefineEntry.hpp"
#include "RefineSpellID.hpp"
#include <iostream>
namespace open_viii::menu_group {
struct [[maybe_unused]] RefineSection002// refine Magic to Magic
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_3
   * Ability	        # of Entries	mngrp.bin Location	        Offset
   * Description Mid-Mag-RF	        4 entries	(mngrp.bin loc:
   * 0x2200000)	0x0	Upgrade Magic from low level to mid level High-Mag-RF
   * 6 entries	(mngrp.bin loc: 0x2200020)	0x20	Upgrade Magic from mid
   * level to high level
   */
private:
  static constexpr auto MID_MAG_RF_COUNT  = 4U;
  static constexpr auto HIGH_MAG_RF_COUNT = 6U;
  std::array<RefineEntry<RefineSpellID, RefineSpellID>, MID_MAG_RF_COUNT>
    m_mid_mag_rf{};
  std::array<RefineEntry<RefineSpellID, RefineSpellID>, HIGH_MAG_RF_COUNT>
    m_high_mag_rf{};

public:
  [[maybe_unused]] [[nodiscard]] const auto &
    mid_mag_rf() const noexcept
  {
    return m_mid_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &
    high_mag_rf() const noexcept
  {
    return m_high_mag_rf;
  }
  [[nodiscard]] static constexpr auto
    size()
  {
    return MID_MAG_RF_COUNT + HIGH_MAG_RF_COUNT;
  }
  template<LangT langVal, typename T>
  std::ostream &
    out_array(const T &                    arr,
              std::ostream &               os,
              const std::span<const char> &buffer          = ""sv,
              const intmax_t               offset          = 0,
              bool                         skip_first_null = false) const
  {
    for (const auto item : arr) {
      (item.template out<langVal>(os, buffer, offset, skip_first_null)) << '\n';
    }
    return os;
  }
  template<LangT langVal>
  std::ostream &
    out(std::ostream &               os,
        const std::span<const char> &buffer          = ""sv,
        const intmax_t               offset          = 0,
        bool                         skip_first_null = false) const
  {
    os << "midMagRF_:\n";
    out_array<langVal>(m_mid_mag_rf, os, buffer, offset, skip_first_null);
    os << "highMagRF_:\n";
    out_array<langVal>(m_high_mag_rf, os, buffer, offset, skip_first_null);
    return os;
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_REFINESECTION002_HPP
