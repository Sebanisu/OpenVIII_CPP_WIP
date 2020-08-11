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

#ifndef VIIIARCHIVE_REFINESECTION000_H
#define VIIIARCHIVE_REFINESECTION000_H
#include "RefineItemID.h"
#include "RefineSpellID.h"
#include "RefineEntry.h"
#include <iostream>
namespace OpenVIII::MenuGroup {
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

struct [[maybe_unused]] RefineSection000// Refine Item to Magic
{
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data
   * Ability	    # of Entries  mngrp.bin Location	    Offset	Description
   * T Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F000)	0x0	    Item to Thunder/Wind Magic
   * I Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F038)	0x38	Item to Ice/Water Magic
   * F Mag-RF	    10 entries	  (mngrp.bin loc: 0x21F070)	0x70	Item to Fire/Flare Magic
   * L Mag-RF	    21 entries	  (mngrp.bin loc: 0x21F0C0)	0xC0	Item to Life Magic
   * Time Mag-RF    14 entries	  (mngrp.bin loc: 0x21F168)	0x168	Item to Time Magic
   * ST Mag-RF	    17 entries	  (mngrp.bin loc: 0x21F1D8)	0x1D8	Item to Status Magic
   * Supt Mag-RF    20 entries	  (mngrp.bin loc: 0x21F260)	0x260	Items to Support Magic
   * Forbid Mag-RF  6 entries	  (mngrp.bin loc: 0x21F300)	0x300	Items to Forbidden Magic
   */
private:
  static constexpr auto tMagRFCount_ = 7U;
  static constexpr auto iMagRFCount_ = 7U;
  static constexpr auto fMagRFCount_ = 10U;
  static constexpr auto lMagRFCount_ = 21U;
  static constexpr auto timeMagRFCount_ = 14U;
  static constexpr auto stMagRFCount_ = 17U;
  static constexpr auto suptRFCount_ = 20U;
  static constexpr auto forbidMagRFCount_ = 6U;
  std::array<RefineEntry<RefineItemID, RefineSpellID>, tMagRFCount_> tMagRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, iMagRFCount_> iMagRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, fMagRFCount_> fMagRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, lMagRFCount_> lMagRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, timeMagRFCount_> timeMagRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, stMagRFCount_> stMagRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, suptRFCount_> suptRF_{};
  std::array<RefineEntry<RefineItemID, RefineSpellID>, forbidMagRFCount_> forbidMagRF_{};

public:
  // template<const Refine000T i>
  //  [[nodiscard]] const auto & at()
  //  {
  //    if constexpr (i == Refine000T::tMagRF) {
  //      return tMagRF();
  //    }
  //    else if constexpr (i == Refine000T::iMagRF){
  //      return iMagRF();
  //    }
  //    else if constexpr (i == Refine000T::fMagRF){
  //      return fMagRF();
  //    }
  //    else if constexpr (i == Refine000T::lMagRF){
  //      return lMagRF();
  //    }
  //    else if constexpr (i == Refine000T::timeMagRF){
  //      return timeMagRF();
  //    }
  //    else if constexpr (i == Refine000T::stMagRF){
  //      return stMagRF();
  //    }
  //    else if constexpr (i == Refine000T::suptRF){
  //      return suptRF();
  //    }
  //    else if constexpr (i == Refine000T::forbidMagRF){
  //      return forbidMagRF();
  //    }
  //    else
  //    {
  //      return nullptr;
  //    }
  //  }
  //  template<std::size_t i=0, std::size_t count=SIZE_MAX, typename fT> void static_for([[maybe_unused]]const fT & f)
  //  {
  //    if constexpr (i<count && i<static_cast<size_t>(Refine000T::count))
  //    {
  //      f(i,at<static_cast<Refine000T>(i)>());
  //      static_for<i+1U,count>(f);
  //    }
  //  }
  [[maybe_unused]] [[nodiscard]] const auto &tMagRF() const noexcept { return tMagRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &iMagRF() const noexcept { return iMagRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &fMagRF() const noexcept { return fMagRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &lMagRF() const noexcept { return lMagRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &timeMagRF() const noexcept { return timeMagRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &stMagRF() const noexcept { return stMagRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &suptRF() const noexcept { return suptRF_; }
  [[maybe_unused]] [[nodiscard]] const auto &forbidMagRF() const noexcept { return forbidMagRF_; }
  [[nodiscard]] static constexpr auto size()
  {
    return tMagRFCount_ + iMagRFCount_ + fMagRFCount_ + lMagRFCount_ + timeMagRFCount_ + stMagRFCount_ + suptRFCount_
           + forbidMagRFCount_;
  }
  template<LangT langVal, typename T>
  std::ostream &outArray(const T &arr,
    std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skipFirstNull = false) const

  {
    for (const auto item : arr) {
      item.template out<langVal>(os, buffer, offset, skipFirstNull);
      os << '\n';
    }
    return os;
  }
  template<LangT langVal>
  std::ostream &out(std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skipFirstNull = false) const

  {
    os << "tMagRF:\n";
    outArray<langVal>(tMagRF_, os, buffer, offset, skipFirstNull);
    os << "iMagRF:\n";
    outArray<langVal>(iMagRF_, os, buffer, offset, skipFirstNull);
    os << "fMagRF:\n";
    outArray<langVal>(fMagRF_, os, buffer, offset, skipFirstNull);
    os << "lMagRF:\n";
    outArray<langVal>(lMagRF_, os, buffer, offset, skipFirstNull);
    os << "timeMagRF:\n";
    outArray<langVal>(timeMagRF_, os, buffer, offset, skipFirstNull);
    os << "stMagRF:\n";
    outArray<langVal>(stMagRF_, os, buffer, offset, skipFirstNull);
    os << "suptMagRF:\n";
    outArray<langVal>(suptRF_, os, buffer, offset, skipFirstNull);
    os << "forbidMagRF:\n";
    outArray<langVal>(forbidMagRF_, os, buffer, offset, skipFirstNull);
    return os;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_REFINESECTION000_H
