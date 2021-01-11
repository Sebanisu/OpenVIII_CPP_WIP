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

#ifndef VIIIARCHIVE_ZELLDUELLIMITBREAKPARAMS_HPP
#define VIIIARCHIVE_ZELLDUELLIMITBREAKPARAMS_HPP
#include <array>
#include <cstdint>
namespace open_viii::kernel {
struct ZellDuelLimitBreakParams
{
private:
  // https://github.com/DarkShinryu/doomtrain/wiki/Zell-limit-break-parameters
  // Zell Limit Break Parameters Section
  //  Offset	Length	Description
  //  0x0000	1 byte	[[StartMove 0
  //  0x0001	1 byte	Next Sequence 0_1
  //  0x0002	1 byte	Next Sequence 0_2
  //  0x0003	1 byte	Next Sequence 0_3
  //  0x0004	1 byte	[[StartMove 1
  //  0x0005	1 byte	Next Sequence 1_1
  //  0x0006	1 byte	Next Sequence 1_2
  //  0x0007	1 byte	Next Sequence 1_3
  //  0x0008	1 byte	[[StartMove 2
  //  0x0009	1 byte	Next Sequence 2_1
  //  0x000A	1 byte	Next Sequence 2_2
  //  0x000B	1 byte	Next Sequence 2_3
  //  0x000C	1 byte	[[StartMove 3
  //  0x000D	1 byte	Next Sequence 3_1
  //  0x000E	1 byte	Next Sequence 3_2
  //  0x000F	1 byte	Next Sequence 3_3
  //  0x0010	1 byte	[[StartMove 4
  //  0x0011	1 byte	Next Sequence 4_1
  //  0x0012	1 byte	Next Sequence 4_2
  //  0x0013	1 byte	Next Sequence 4_3
  //  0x0014	1 byte	[[StartMove 5
  //  0x0015	1 byte	Next Sequence 5_1
  //  0x0016	1 byte	Next Sequence 5_2
  //  0x0017	1 byte	Next Sequence 5_3
  //  0x0018	1 byte	[[StartMove 6
  //  0x0019	1 byte	Next Sequence 6_1
  //  0x001A	1 byte	Next Sequence 6_2
  //  0x001B	1 byte	Next Sequence 6_3
  //  0x001C	1 byte	[[StartMove 7
  //  0x001D	1 byte	Next Sequence 7_1
  //  0x001E	1 byte	Next Sequence 7_2
  //  0x001F	1 byte	Next Sequence 7_3
  //  0x0020	1 byte	[[StartMove 8
  //  0x0021	1 byte	Next Sequence 8_1
  //  0x0022	1 byte	Next Sequence 8_2
  //  0x0023	1 byte	Next Sequence 8_3
  //  0x0024	1 byte	[[StartMove 9
  //  0x0025	1 byte	Next Sequence 9_1
  //  0x0026	1 byte	Next Sequence 9_2
  //  0x0027	1 byte	Next Sequence 9_3
  //  0x0028	1 byte	[[StartMove 10
  //  0x0029	1 byte	Next Sequence 10_1
  //  0x002A	1 byte	Next Sequence 10_2
  //  0x002B	1 byte	Next Sequence 10_3
  //  0x002C	1 byte	[[StartMove 11
  //  0x002D	1 byte	Next Sequence 11_1
  //  0x002E	1 byte	Next Sequence 11_2
  //  0x002F	1 byte	Next Sequence 11_3
  //  0x0030	1 byte	[[StartMove 12
  //  0x0031	1 byte	Next Sequence 12_1
  //  0x0032	1 byte	Next Sequence 12_2
  //  0x0033	1 byte	Next Sequence 12_3
  //  0x0034	1 byte	[[StartMove 13
  //  0x0035	1 byte	Next Sequence 13_1
  //  0x0036	1 byte	Next Sequence 13_2
  //  0x0037	1 byte	Next Sequence 13_3
  //  0x0038	1 byte	[[StartMove 14
  //  0x0039	1 byte	Next Sequence 14_1
  //  0x003A	1 byte	Next Sequence 14_2
  //  0x003B	1 byte	Next Sequence 14_3
  //  0x003C	1 byte	[[StartMove 15
  //  0x003D	1 byte	Next Sequence 15_1
  //  0x003E	1 byte	Next Sequence 15_2
  //  0x003F	1 byte	Next Sequence 15_3
  //  0x0040	1 byte	[[StartMove 16
  //  0x0041	1 byte	Next Sequence 16_1
  //  0x0042	1 byte	Next Sequence 16_2
  //  0x0043	1 byte	Next Sequence 16_3
  //  0x0044	1 byte	[[StartMove 17
  //  0x0045	1 byte	Next Sequence 17_1
  //  0x0046	1 byte	Next Sequence 17_2
  //  0x0047	1 byte	Next Sequence 17_3
  //  0x0048	1 byte	[[StartMove 18
  //  0x0049	1 byte	Next Sequence 18_1
  //  0x004A	1 byte	Next Sequence 18_2
  //  0x004B	1 byte	Next Sequence 18_3
  //  0x004C	1 byte	[[StartMove 19
  //  0x004D	1 byte	Next Sequence 19_1
  //  0x004E	1 byte	Next Sequence 19_2
  //  0x004F	1 byte	Next Sequence 19_3
  //  0x0050	1 byte	[[StartMove 20
  //  0x0051	1 byte	Next Sequence 20_1
  //  0x0052	1 byte	Next Sequence 20_2
  //  0x0053	1 byte	Next Sequence 20_3
  //  0x0054	1 byte	[[StartMove 21
  //  0x0055	1 byte	Next Sequence 21_1
  //  0x0056	1 byte	Next Sequence 21_2
  //  0x0057	1 byte	Next Sequence 21_3
  //  0x0058	1 byte	[[StartMove 22
  //  0x0059	1 byte	Next Sequence 22_1
  //  0x005A	1 byte	Next Sequence 22_2
  //  0x005B	1 byte	Next Sequence 22_3
  //  0x005C	1 byte	[[StartMove 23
  //  0x005D	1 byte	Next Sequence 23_1
  //  0x005E	1 byte	Next Sequence 23_2
  //  0x005F	1 byte	Next Sequence 23_3
  //  0x0060	1 byte	[[StartMove 24
  //  0x0061	1 byte	Next Sequence 24_1
  //  0x0062	1 byte	Next Sequence 24_2
  //  0x0063	1 byte	Next Sequence 24_3
  std::array<std::uint8_t, 4> m_data{};

public:
  [[nodiscard]] constexpr auto at(const size_t &index) const
  {
    return m_data.at(index);
  }
  [[nodiscard]] constexpr auto size()
  {
    return m_data.size();
  }
  //[[nodiscard]] constexpr auto operator[](const size_t &index) { return
  // data_[index]; }
  [[nodiscard]] constexpr auto begin()
  {
    return m_data.begin();
  }
  [[nodiscard]] constexpr auto end()
  {
    return m_data.end();
  }
  std::ostream &out(
    std::ostream &os, [[maybe_unused]] const std::span<const char> &textSpan)
  {
    os << '{';
    bool first = true;
    for (const auto &item : m_data) {
      if (!first) {
        os << ", ";
      }
      first = false;
      os << static_cast<size_t>(item);
    }
    return os << "};";
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ZELLDUELLIMITBREAKPARAMS_HPP
