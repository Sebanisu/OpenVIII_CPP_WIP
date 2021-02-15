//
// Created by pcvii on 12/22/2020.
//
#ifndef VIIIARCHIVE_ROLLTYPET_HPP
#define VIIIARCHIVE_ROLLTYPET_HPP
#include <cstdint>
namespace open_viii::battle::stage {
enum struct RollTypeT : std::uint8_t {
  todo_roll      = 0U,
  default_roll   = 1U,
  same_roll      = 2U,
  different_roll = 3U,
};
}
#endif// VIIIARCHIVE_ROLLTYPET_HPP
