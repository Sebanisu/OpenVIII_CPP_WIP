//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAM_HPP
#define VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAM_HPP
#include "BattleOnlyStatusesT.hpp"
#include "CrisisLevelT.hpp"
#include "PersistentStatusesT.hpp"
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
struct QuistisBlueMagicLimitBreakParam_impl
{
protected:
  BattleOnlyStatusesT          m_battle_only_statuses = {};
  PersistentStatusesT          m_persistent_statuses  = {};
  std::uint8_t                 m_attack_power         = {};
  std::uint8_t                 m_attack_param         = {};
  static constexpr std::size_t EXPECTED_SIZE          = 8U;
  constexpr QuistisBlueMagicLimitBreakParam_impl()    = default;

public:
  constexpr auto
    operator<=>(
      const QuistisBlueMagicLimitBreakParam_impl &right) const noexcept
    = default;
};
using QuistisBlueMagicLimitBreakParam
  = CommonKernel<QuistisBlueMagicLimitBreakParam_impl>;
static_assert(
  QuistisBlueMagicLimitBreakParam::EXPECTED_SIZE
  == sizeof(QuistisBlueMagicLimitBreakParam));
static_assert(has_battle_only_statuses<QuistisBlueMagicLimitBreakParam>);
static_assert(has_persistent_statuses<QuistisBlueMagicLimitBreakParam>);
static_assert(has_attack_power<QuistisBlueMagicLimitBreakParam>);
static_assert(has_attack_param<QuistisBlueMagicLimitBreakParam>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_QUISTISBLUEMAGICLIMITBREAKPARAM_HPP
