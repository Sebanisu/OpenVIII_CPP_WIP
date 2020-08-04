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

#include "OpenVIII/Strings/EncodedStringOffset.h"
#ifndef VIIIARCHIVE_RefineEntry_H
#define VIIIARCHIVE_RefineEntry_H
namespace OpenVIII::MenuGroup {
template<typename inputT, typename outputT> struct RefineEntry
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

  template<LangT langVal>
  std::ostream &out(std::ostream &os, const std::string_view &buffer = ""sv,
                    const intmax_t offset = 0,
                    bool skipFirstNull = false) const
  {
    return os << '"' << offset_.DecodedString<langVal>(buffer,offset,skipFirstNull) << "\", " << static_cast<std::uint16_t>(AmountReceived()) << ", "
              << static_cast<std::uint16_t>(unknown0()) << ", " << static_cast<std::uint16_t>(unknown1()) << ", "
              << static_cast<std::uint16_t>(Input()) << ", " << static_cast<std::uint16_t>(AmountRequired()) << ", "
              << static_cast<std::uint16_t>(Output());
  }
//  friend std::ostream &operator<<(std::ostream &os, const RefineEntry<inputT, outputT> &input)
//  {
//    return input.out(os);
//  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_RefineEntry_H
