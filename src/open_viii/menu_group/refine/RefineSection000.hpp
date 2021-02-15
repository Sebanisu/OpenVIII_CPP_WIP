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
#ifndef VIIIARCHIVE_REFINESECTION000_HPP
#define VIIIARCHIVE_REFINESECTION000_HPP
#include "RefineEntry.hpp"
#include "RefineSpellID.hpp"
#include "open_viii/ItemIdT.hpp"
#include <iostream>
namespace open_viii::menu_group {
// enum struct Refine000T
//{
//  tMagRF,
//  iMagRF,
//  fMagRF,
//  lMagRF,
//  timeMagRF,
//  stMagRF,
//  suptRF,
//  forbidMagRF,
//  count,
//};
struct [[maybe_unused]] RefineSection000// refine Item to Magic
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data
   * Ability	    # of Entries  mngrp.bin Location	    Offset
   * Description T Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F000)
   * 0x0	    Item to Thunder/Wind Magic I Mag-RF	    7 entries
   * (mngrp.bin loc: 0x21F038)	0x38	Item to Ice/Water Magic F Mag-RF
   * 10 entries	  (mngrp.bin loc: 0x21F070)	0x70	Item to Fire/Flare Magic
   * L Mag-RF	    21 entries	  (mngrp.bin loc: 0x21F0C0)	0xC0	Item to
   * Life Magic Time Mag-RF    14 entries	  (mngrp.bin loc: 0x21F168)
   * 0x168	Item to Time Magic ST Mag-RF	    17 entries	  (mngrp.bin
   * loc: 0x21F1D8)	0x1D8	Item to Status Magic Supt Mag-RF    20 entries
   * (mngrp.bin loc: 0x21F260)	0x260	Items to Support Magic Forbid Mag-RF  6
   * entries	  (mngrp.bin loc: 0x21F300)	0x300	Items to Forbidden Magic
   */
private:
  static constexpr auto T_MAG_RF_COUNT      = 7U;
  static constexpr auto I_MAG_RF_COUNT      = 7U;
  static constexpr auto F_MAG_RF_COUNT      = 10U;
  static constexpr auto L_MAG_RF_COUNT      = 21U;
  static constexpr auto TIME_MAG_RF_COUNT   = 14U;
  static constexpr auto ST_MAG_RF_COUNT     = 17U;
  static constexpr auto SUPT_RF_COUNT       = 20U;
  static constexpr auto FORBID_MAG_RF_COUNT = 6U;
  std::array<RefineEntry<ItemIdT, RefineSpellID>, T_MAG_RF_COUNT> m_t_mag_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, I_MAG_RF_COUNT> m_i_mag_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, F_MAG_RF_COUNT> m_f_mag_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, L_MAG_RF_COUNT> m_l_mag_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, TIME_MAG_RF_COUNT>
    m_time_mag_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, ST_MAG_RF_COUNT>
                                                                 m_st_mag_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, SUPT_RF_COUNT> m_supt_rf{};
  std::array<RefineEntry<ItemIdT, RefineSpellID>, FORBID_MAG_RF_COUNT>
    m_forbid_mag_rf{};
public:
  [[maybe_unused]] [[nodiscard]] const auto &t_mag_rf() const noexcept
  {
    return m_t_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &i_mag_rf() const noexcept
  {
    return m_i_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &f_mag_rf() const noexcept
  {
    return m_f_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &l_mag_rf() const noexcept
  {
    return m_l_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &time_mag_rf() const noexcept
  {
    return m_time_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &st_mag_rf() const noexcept
  {
    return m_st_mag_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &supt_rf() const noexcept
  {
    return m_supt_rf;
  }
  [[maybe_unused]] [[nodiscard]] const auto &forbid_mag_rf() const noexcept
  {
    return m_forbid_mag_rf;
  }
  [[nodiscard]] static constexpr auto size()
  {
    return T_MAG_RF_COUNT + I_MAG_RF_COUNT + F_MAG_RF_COUNT + L_MAG_RF_COUNT
           + TIME_MAG_RF_COUNT + ST_MAG_RF_COUNT + SUPT_RF_COUNT
           + FORBID_MAG_RF_COUNT;
  }
  template<LangT langVal, typename T>
  std::ostream &out_array(const T &                    arr,
                          std::ostream &               os,
                          const std::span<const char> &buffer = ""sv,
                          const intmax_t               offset = 0,
                          bool skip_first_null                = false) const
  {
    for (const auto item : arr) {
      item.template out<langVal>(os, buffer, offset, skip_first_null);
      os << '\n';
    }
    return os;
  }
  template<LangT langVal>
  std::ostream &out(std::ostream &               os,
                    const std::span<const char> &buffer          = ""sv,
                    const intmax_t               offset          = 0,
                    bool                         skip_first_null = false) const
  {
    os << "tMagRF:\n";
    out_array<langVal>(m_t_mag_rf, os, buffer, offset, skip_first_null);
    os << "iMagRF:\n";
    out_array<langVal>(m_i_mag_rf, os, buffer, offset, skip_first_null);
    os << "fMagRF:\n";
    out_array<langVal>(m_f_mag_rf, os, buffer, offset, skip_first_null);
    os << "lMagRF:\n";
    out_array<langVal>(m_l_mag_rf, os, buffer, offset, skip_first_null);
    os << "timeMagRF:\n";
    out_array<langVal>(m_time_mag_rf, os, buffer, offset, skip_first_null);
    os << "stMagRF:\n";
    out_array<langVal>(m_st_mag_rf, os, buffer, offset, skip_first_null);
    os << "suptMagRF:\n";
    out_array<langVal>(m_supt_rf, os, buffer, offset, skip_first_null);
    os << "forbidMagRF:\n";
    out_array<langVal>(m_forbid_mag_rf, os, buffer, offset, skip_first_null);
    return os;
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_REFINESECTION000_HPP
