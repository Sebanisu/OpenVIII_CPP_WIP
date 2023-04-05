//
// Created by pcvii on 4/4/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION3_MODEL_ANIMATION_HPP
#define OPENVIII_CPP_WIP_SECTION3_MODEL_ANIMATION_HPP
#include <span>
namespace open_viii::battle {
struct Section3_Model_Animation
{

  Section3_Model_Animation() = default;
  Section3_Model_Animation(
    [[maybe_unused]] const char *const     section_start,
    [[maybe_unused]] std::span<const char> span)
  {}
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_SECTION3_MODEL_ANIMATION_HPP
