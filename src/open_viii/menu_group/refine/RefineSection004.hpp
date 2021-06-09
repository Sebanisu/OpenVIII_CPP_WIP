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
#ifndef VIIIARCHIVE_REFINESECTION004_HPP
#define VIIIARCHIVE_REFINESECTION004_HPP
#include "RefineCardID.hpp"
#include "RefineEntry.hpp"
#include "open_viii/ItemIdT.hpp"
#include <array>
#include <iostream>
namespace open_viii::menu_group {
struct [[maybe_unused]] RefineSection004// refine cards to items
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data_5
   * Ability	        # of Entries	mngrp.bin Location	        Offset
   * Description CardMod	        110 Entries	(mngrp.bin loc:
   * 0x221000)	0x0	Card to Items level
   */
private:
  static constexpr auto CARD_MOD_COUNT = 110U;
  std::array<RefineEntry<RefineCardID, ItemIdT>, CARD_MOD_COUNT> m_card_mod{};

public:
  [[nodiscard]] const auto &
    card_mod() const noexcept
  {
    return m_card_mod;
  }
  [[nodiscard]] const auto *
    operator->() const
  {
    return &m_card_mod;
  }
  [[nodiscard]] static constexpr auto
    size()
  {
    return CARD_MOD_COUNT;
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
    os << "cardMod:\n";
    out_array<langVal>(m_card_mod, os, buffer, offset, skip_first_null);
    return os;
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_REFINESECTION004_HPP
