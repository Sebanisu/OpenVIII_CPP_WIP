//
// Created by pcvii on 12/22/2020.
//
#ifndef VIIIARCHIVE_FOVTYPET_HPP
#define VIIIARCHIVE_FOVTYPET_HPP
#include <cstdint>
namespace open_viii::battle::stage {
enum struct FovTypeT : std::uint8_t {
  default_fov   = 1U,
  same_fov      = 2U,
  different_fov = 3U,
};
}
#endif// VIIIARCHIVE_FOVTYPET_HPP
