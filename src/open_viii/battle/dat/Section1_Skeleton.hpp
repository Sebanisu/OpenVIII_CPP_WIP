//
// Created by pcvii on 4/4/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION1_SKELETON_HPP
#define OPENVIII_CPP_WIP_SECTION1_SKELETON_HPP
#include <span>
namespace open_viii::battle::dat {
struct Section1_Skeleton
{

  Section1_Skeleton() = default;
  Section1_Skeleton(
    [[maybe_unused]] const char *const     section_start,
    [[maybe_unused]] std::span<const char> span)
  {}
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_SECTION1_SKELETON_HPP
