//
// Created by pcvii on 7/13/2020.
//

#include "OpenVIII/Strings/EncodedStringOffset.h"
#ifndef VIIIARCHIVE_MENTRY_H
#define VIIIARCHIVE_MENTRY_H
namespace OpenVIII::MenuGroup {
template<typename inputT, typename outputT> struct mEntry
{
  /*
   * Type	Size	Value	Description
   * UInt16	2	Offset	Text Offset (little-endian)
   * Byte	1	Received	Amount received
   * UInt16	2	UNK	{0x0001}
   * Byte	1	Input_Item_ID	Input: Item id value 0x00-0xC6
   * Byte	1	Required	Amount needed
   * Byte	1	Output_Item_ID	Output: Item id value 0x00-0xC6
   */
private:
  EncodedStringOffset offset_{};
  std::uint8_t amountReceived_{};
  std::uint8_t unknown0_{};
  std::uint8_t unknown1_{};
  inputT input_{};
  std::uint8_t amountRequired_{};
  outputT output_{};

public:
  [[nodiscard]] auto Offset() const noexcept { return offset_; }
  [[nodiscard]] auto AmountReceived() const noexcept { return amountReceived_; }
  [[nodiscard]] auto unknown0() const noexcept { return unknown0_; }
  [[nodiscard]] auto unknown1() const noexcept { return unknown1_; }
  [[nodiscard]] auto Input() const noexcept { return input_; }
  [[nodiscard]] auto AmountRequired() const noexcept { return amountRequired_; }
  [[nodiscard]] auto Output() const noexcept { return output_; }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_MENTRY_H
