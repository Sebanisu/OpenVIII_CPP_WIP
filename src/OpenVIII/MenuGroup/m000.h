//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M000_H
#define VIIIARCHIVE_M000_H
#include <cstdint>
#include "OpenVIII/Strings/EncodedStringOffset.h"

namespace OpenVIII::MenuGroup {
struct [[maybe_unused]] m000 //Refine Item to Magic
{

  struct Entry
  {
    /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Entry
     * Type	Size	Value	Description
     * UInt16	2	Offset	Text Offset (little-endian)
     * Byte	1	Received	Amount received
     * UInt16	2	UNK	{0x0001}
     * Byte	1	Input_Item_ID	Input: Item id value (0x00-0xC6)
     * Byte	1	Required	Amount needed
     * Byte	1	Output_Spell_ID	Output: Spell id value (0x01-0x38)
     */
  private:
    EncodedStringOffset offset_{};
    std::uint8_t amountReceived_{};
    std::uint8_t unknown0_{};
    std::uint8_t unknown1_{};
    std::uint8_t itemID_{};
    std::uint8_t amountRequired_{};
    std::uint8_t spellID_{};

  public:
    [[nodiscard]] auto Offset() const noexcept { return offset_; }
    [[nodiscard]] auto AmountReceived() const noexcept { return amountReceived_; }
    [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
    [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
    [[nodiscard]] auto ItemID() const noexcept { return itemID_; }
    [[nodiscard]] auto AmountRequired() const noexcept { return amountRequired_; }
    [[nodiscard]] auto SpellID() const noexcept { return spellID_; }
  };
  /* http://wiki.ffrtt.ru/index.php?title=FF8/Menu_m000_m004#Data
   * Ability	    # of Entries  mngrp.bin Location	        Offset	Description
   * T Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F000)	0x0	Item to Thunder/Wind Magic
   * I Mag-RF	    7 entries	  (mngrp.bin loc: 0x21F038)	0x38	Item to Ice/Water Magic
   * F Mag-RF	    10 entries	  (mngrp.bin loc: 0x21F070)	0x70	Item to Fire/Flare Magic
   * L Mag-RF	    21 entries	  (mngrp.bin loc: 0x21F0C0)	0xC0	Item to Life Magic
   * Time Mag-RF    14 entries	  (mngrp.bin loc: 0x21F168)	0x168	Item to Time Magic
   * ST Mag-RF	    17 entries	  (mngrp.bin loc: 0x21F1D8)	0x1D8	Item to Status Magic
   * Supt Mag-RF    20 entries	  (mngrp.bin loc: 0x21F260)	0x260	Items to Support Magic
   * Forbid Mag-RF  6 entries	  (mngrp.bin loc: 0x21F300)	0x300	Items to Forbidden Magic
   */
private:
  constexpr auto tMagRFCount_ = 7U;
  constexpr auto iMagRFCount_ = 7U;
  constexpr auto fMagRFCount_ = 10U;
  constexpr auto lMagRFCount_ = 21U;
  constexpr auto timeMagRFCount_ = 14U;
  constexpr auto stMagRFCount_ = 17U;
  constexpr auto suptRFCount_ = 20U;
  constexpr auto forbidMagRFCount_ = 6U;
  std::array<Entry, tMagRFCount_> tMagRF_{};
  std::array<Entry, iMagRFCount_> iMagRF_{};
  std::array<Entry, fMagRFCount_> fMagRF_{};
  std::array<Entry, lMagRFCount_> lMagRF_{};
  std::array<Entry, timeMagRFCount_> timeMagRF_{};
  std::array<Entry, stMagRFCount_> stMagRF_{};
  std::array<Entry, suptRFCount_> suptRF_{};
  std::array<Entry, forbidMagRFCount_> forbidMagRF_{};

public:
  [[nodiscard]] auto tMagRF() const noexcept { return tMagRF_; }
  [[nodiscard]] auto iMagRF() const noexcept { return iMagRF_; }
  [[nodiscard]] auto fMagRF() const noexcept { return fMagRF_; }
  [[nodiscard]] auto lMagRF() const noexcept { return lMagRF_; }
  [[nodiscard]] auto timeMagRF() const noexcept { return timeMagRF_; }
  [[nodiscard]] auto stMagRF() const noexcept { return stMagRF_; }
  [[nodiscard]] auto suptRF() const noexcept { return suptRF_; }
  [[nodiscard]] auto forbidMagRF() const noexcept { return forbidMagRF_; }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_M000_H
