//
// Created by pcvii on 1/25/2021.
//
#ifndef VIIIARCHIVE_SLOT_HPP
#define VIIIARCHIVE_SLOT_HPP
#include "CommonKernel.hpp"
#include <compare>
#include <cstdint>
namespace open_viii::kernel {
/**
 * 8 total - magic id and count
 * @see https://github.com/DarkShinryu/doomtrain/wiki/Selphie-limit-break-sets
 */
struct Slot_impl
{
protected:
  std::uint8_t m_magic_id                    = {};
  std::uint8_t m_count                       = {};
  constexpr Slot_impl()                      = default;
  static constexpr std::size_t EXPECTED_SIZE = 2U;

public:
  constexpr auto
    operator<=>(const Slot_impl &right) const noexcept = default;
};
using Slot = CommonKernel<Slot_impl>;
static_assert(Slot::EXPECTED_SIZE == sizeof(Slot));
static_assert(has_magic_id<Slot>);
static_assert(has_count<Slot>);
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_SLOT_HPP
