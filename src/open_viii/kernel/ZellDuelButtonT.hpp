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
#ifndef VIIIARCHIVE_ZELLDUELBUTTONT_HPP
#define VIIIARCHIVE_ZELLDUELBUTTONT_HPP
namespace open_viii::kernel {
enum class ZellDuelButtonT : std::uint16_t
{
  end      = 0x0001,
  up       = 0x0010,
  right    = 0x0020,
  down     = 0x0040,
  left     = 0x0080,
  l2       = 0x0100,
  r2       = 0x0200,
  l1       = 0x0400,
  r1       = 0x0800,
  triangle = 0x1000,
  circle   = 0x2000,
  cross    = 0x4000,
  square   = 0x8000,
  none     = 0xFFFF,
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ZELLDUELBUTTONT_HPP
