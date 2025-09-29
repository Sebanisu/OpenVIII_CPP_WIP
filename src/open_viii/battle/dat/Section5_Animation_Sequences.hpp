//
// Created by pcvii on 4/4/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION5_ANIMATION_SEQUENCES_HPP
#define OPENVIII_CPP_WIP_SECTION5_ANIMATION_SEQUENCES_HPP
#include <span>
namespace open_viii::battle::dat {
struct Section5_Animation_Sequences
{

  Section5_Animation_Sequences() = default;
  Section5_Animation_Sequences(
    [[maybe_unused]] const char *const     section_start,
    [[maybe_unused]] std::span<const char> span_1,
    [[maybe_unused]] std::span<const char> span_2)
  {}
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_SECTION5_ANIMATION_SEQUENCES_HPP
