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

#ifndef VIIIARCHIVE_REFINESECTION001_H
#define VIIIARCHIVE_REFINESECTION001_H
#include "OpenVIII/ItemIdT.h"
#include "RefineEntry.h"
#include <array>
#include <iostream>
namespace open_viii::menu_group {
struct [[maybe_unused]] RefineSection001// Item to Item
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_2
   * Ability	    # of Entries  mngrp.bin Location	        Offset  Description
   * Recov Med-RF   9 Entries	  (mngrp.bin loc: 0x21F800)	0x0	Item to Recovery Items
   * ST Med-RF      12 Entries	  (mngrp.bin loc: 0x21F848)	0x48	Item to Status Removal Items
   * Ammo-RF	    16 Entries	  (mngrp.bin loc: 0x21F8A8)	0xA8	Item to Ammo Item
   * Forbid Med-RF  20 Entries	  (mngrp.bin loc: 0x21F928)	0x128	Item to Forbidden Medicine
   * GFRecov Med-RF 12 Entries	  (mngrp.bin loc: 0x21F9C8)	0x1C8	Item to GF Recovery Items
   * GFAbl Med-RF   42 Entries	  (mngrp.bin loc: 0x21FA28)	0x228	Item to GF Ability Medicine Items
   * Tool-RF	    32 Entries	  (mngrp.bin loc: 0x21FB78)	0x378	Item to Tool Items
   * */
private:
  static constexpr auto RECOV_MED_RF_COUNT = 9U;
  static constexpr auto ST_MED_RF_COUNT = 12U;
  static constexpr auto AMMO_RF_COUNT = 16U;
  static constexpr auto FORBID_MED_RF_COUNT = 20U;
  static constexpr auto GF_RECOV_RF_COUNT = 12U;
  static constexpr auto GF_ABL_MED_RF_COUNT = 42U;
  static constexpr auto TOOL_RF_COUNT = 32U;
  std::array<RefineEntry<ItemIdT, ItemIdT>, RECOV_MED_RF_COUNT> m_recov_med_rf{};
  std::array<RefineEntry<ItemIdT, ItemIdT>, ST_MED_RF_COUNT> m_st_med_rf{};
  std::array<RefineEntry<ItemIdT, ItemIdT>, AMMO_RF_COUNT> m_ammo_rf{};
  std::array<RefineEntry<ItemIdT, ItemIdT>, FORBID_MED_RF_COUNT> m_forbid_med_rf{};
  std::array<RefineEntry<ItemIdT, ItemIdT>, GF_RECOV_RF_COUNT> m_gf_recov_rf{};
  std::array<RefineEntry<ItemIdT, ItemIdT>, GF_ABL_MED_RF_COUNT> m_gf_abl_med_rf{};
  std::array<RefineEntry<ItemIdT, ItemIdT>, TOOL_RF_COUNT> m_tool_rf{};

public:
  [[maybe_unused]] [[nodiscard]] const auto &recov_med_rf() const noexcept { return m_recov_med_rf; }
  [[maybe_unused]] [[nodiscard]] const auto &st_med_rf() const noexcept { return m_st_med_rf; }
  [[maybe_unused]] [[nodiscard]] const auto &ammo_rf() const noexcept { return m_ammo_rf; }
  [[maybe_unused]] [[nodiscard]] const auto &forbid_med_rf() const noexcept { return m_forbid_med_rf; }
  [[maybe_unused]] [[nodiscard]] const auto &gf_recov_rf() const noexcept { return m_gf_recov_rf; }
  [[maybe_unused]] [[nodiscard]] const auto &gf_abl_med_rf() const noexcept { return m_gf_abl_med_rf; }
  [[maybe_unused]] [[nodiscard]] const auto &tool_rf() const noexcept { return m_tool_rf; }
  [[nodiscard]] static constexpr auto size()
  {
    return RECOV_MED_RF_COUNT + ST_MED_RF_COUNT + AMMO_RF_COUNT + FORBID_MED_RF_COUNT + GF_RECOV_RF_COUNT
           + GF_ABL_MED_RF_COUNT + TOOL_RF_COUNT;
  }

  template<LangT langVal, typename T>
  [[maybe_unused]] std::ostream &out_array(const T &arr,
    std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skip_first_null = false) const

  {
    for (const auto item : arr) { (item.template out<langVal>(os, buffer, offset, skip_first_null)) << '\n'; }
    return os;
  }
  template<LangT langVal>
  std::ostream &out(std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skip_first_null = false) const

  {
    os << "recovMedRF:\n";
    out_array<langVal>(m_recov_med_rf, os, buffer, offset, skip_first_null);
    os << "stMedRF:\n";
    out_array<langVal>(m_st_med_rf, os, buffer, offset, skip_first_null);
    os << "ammoRF:\n";
    out_array<langVal>(m_ammo_rf, os, buffer, offset, skip_first_null);
    os << "forbidMedRF:\n";
    out_array<langVal>(m_forbid_med_rf, os, buffer, offset, skip_first_null);
    os << "gfRecovRF:\n";
    out_array<langVal>(m_gf_recov_rf, os, buffer, offset, skip_first_null);
    os << "gfAblMedRF:\n";
    out_array<langVal>(m_gf_abl_med_rf, os, buffer, offset, skip_first_null);
    os << "toolRF:\n";
    out_array<langVal>(m_tool_rf, os, buffer, offset, skip_first_null);
    return os;
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_REFINESECTION001_H
