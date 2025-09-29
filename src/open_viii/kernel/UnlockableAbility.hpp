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
#ifndef VIIIARCHIVE_UNLOCKABLEABILITY_HPP
#define VIIIARCHIVE_UNLOCKABLEABILITY_HPP
#include "AbilitiesT.hpp"
#include "CommonKernel.hpp"
#include "UnlockerT.hpp"
namespace open_viii::kernel {
/**
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Junctionable-GFs
 */
struct UnlockableAbility_impl
{
protected:
  UnlockerT    m_unlocker                    = {};
  std::uint8_t m_unknown                     = {};
  AbilitiesT   m_ability                     = {};
  constexpr UnlockableAbility_impl()         = default;
  static constexpr std::size_t EXPECTED_SIZE = 4U;

public:
  constexpr auto
    operator<=>(const UnlockableAbility_impl &right) const noexcept
    = default;
};
using UnlockableAbility = CommonKernel<UnlockableAbility_impl>;
static_assert(UnlockableAbility::EXPECTED_SIZE == sizeof(UnlockableAbility));
static_assert(has_unlocker<UnlockableAbility>);
static_assert(has_unknown<UnlockableAbility>);
static_assert(has_ability<UnlockableAbility>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_UNLOCKABLEABILITY_HPP
