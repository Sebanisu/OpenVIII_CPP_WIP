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
namespace open_viii::menu_group {
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
  EncodedStringOffset m_offset{};
  std::uint8_t m_amount_received{};
  std::uint8_t m_unknown0{};
  std::uint8_t m_unknown1{};
  inputT m_input{};
  std::uint8_t m_amount_required{};
  outputT m_output{};

public:
  [[nodiscard]] const auto &offset() const noexcept { return m_offset; }
  [[maybe_unused]] [[nodiscard]] const auto &amount_received() const noexcept { return m_amount_received; }
  [[nodiscard]] const auto &unknown0() const noexcept { return m_unknown0; }
  [[nodiscard]] const auto &unknown1() const noexcept { return m_unknown1; }
  [[nodiscard]] const auto &input() const noexcept { return m_input; }
  [[maybe_unused]] [[nodiscard]] const auto &amount_required() const noexcept { return m_amount_required; }
  [[nodiscard]] const auto &output() const noexcept { return m_output; }

  template<LangT langVal>
  std::ostream &out(std::ostream &os,
    const std::string_view &buffer = ""sv,
    const intmax_t offset = 0,
    bool skip_first_null = false) const
  {
    return os << '"' << Tools::u8_to_sv(m_offset.decoded_string<langVal>(buffer, offset, skip_first_null)) << "\", "
              << static_cast<std::uint16_t>(m_amount_received) << ", " << static_cast<std::uint16_t>(m_unknown0) << ", "
              << static_cast<std::uint16_t>(m_unknown1) << ", " << static_cast<std::uint16_t>(m_input) << ", "
              << static_cast<std::uint16_t>(m_amount_required) << ", " << static_cast<std::uint16_t>(m_output);
  }
  //  friend std::ostream &operator<<(std::ostream &os, const RefineEntry<inputT, outputT> &input)
  //  {
  //    return input.out(os);
  //  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_RefineEntry_H
