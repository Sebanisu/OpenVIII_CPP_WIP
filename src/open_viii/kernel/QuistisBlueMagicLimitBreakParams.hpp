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
#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_HPP
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_HPP
#include "CommonKernel.hpp"
#include "QuistisBlueMagicLimitBreakParam.hpp"
namespace open_viii::kernel {
/**
 * Offset	Length	Description
 * 0x0000	4 bytes	Status 1
 * 0x0004	2 bytes	Status 0
 * 0x0006	1 bytes	Attack Power
 * 0x0007	1 byte	Attack Param
 * @see
 * https://github.com/DarkShinryu/doomtrain/wiki/Quistis-limit-break-parameters
 */
struct QuistisBlueMagicLimitBreakParams_impl
{
protected:
  CrisisLevelT<QuistisBlueMagicLimitBreakParam> m_blue_magic_data = {};
  static constexpr std::size_t EXPECTED_SIZE        = sizeof(m_blue_magic_data);
  constexpr QuistisBlueMagicLimitBreakParams_impl() = default;

public:
  constexpr auto
    operator<=>(
      const QuistisBlueMagicLimitBreakParams_impl &right) const noexcept
    = default;
  [[nodiscard]] const auto *
    operator->() const noexcept
  {
    return &m_blue_magic_data;
  }
};
using QuistisBlueMagicLimitBreakParams
  = CommonKernel<QuistisBlueMagicLimitBreakParams_impl>;
// using QuistisBlueMagicLimitBreakParams =
// CrisisLevelT<QuistisBlueMagicLimitBreakParam>;
static_assert(has_blue_magic_data<QuistisBlueMagicLimitBreakParams>);
static_assert(
  sizeof(QuistisBlueMagicLimitBreakParams)
  == QuistisBlueMagicLimitBreakParams::EXPECTED_SIZE);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAMS_HPP
