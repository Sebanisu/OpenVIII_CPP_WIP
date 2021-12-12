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
#ifndef VIIIARCHIVE_ATTACKFLAGST_HPP
#define VIIIARCHIVE_ATTACKFLAGST_HPP
namespace open_viii::kernel {
enum class AttackFlagsT : std::uint8_t
{
  none                                = 0x0,
  shelled [[maybe_unused]]            = 0x1,
  unk_x2 [[maybe_unused]]             = 0x2,
  unk_x4 [[maybe_unused]]             = 0x4,
  break_damage_limit [[maybe_unused]] = 0x8,
  reflected [[maybe_unused]]          = 0x10,
  unk_x20 [[maybe_unused]]            = 0x20,
  unk_x40 [[maybe_unused]]            = 0x40,
  revive                              = 0x80
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_ATTACKFLAGST_HPP
