//
// Created by pcvii on 4/4/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION3_MODEL_ANIMATION_HPP
#define OPENVIII_CPP_WIP_SECTION3_MODEL_ANIMATION_HPP
#include <span>
namespace open_viii::battle::dat {
/**
 * @brief Animation
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Animation
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/Animation.cs
 */
struct Animation
{
  std::uint8_t frames_count{};
  /**
   * @brief Animation Frame buffer wip need understand the structure of the data.
   * @see https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/AnimationFrame.cs
   */
  std::vector<std::uint8_t> buffer{};
  Animation() = default;
  Animation(std::span<const char> span)
  : frames_count(tools::read_val<std::uint8_t>(span))
  {
    std::ranges::transform(span,std::back_inserter(buffer),[](const char val){return static_cast<std::uint8_t>(val);});
  }
};
/**
 * @brief Model Animation struct containing header and object data.
 * @see
 * https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Section_3:_Model_animation
 */
struct Section3_Model_Animation
{
  /**
   * @brief Header
   * @see
   * https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Header_.28data_sub_table.29_2
   */
  DatHeader              m_section_header{};
  std::vector<Animation> m_animations{};
  std::vector<std::uint32_t>
    offsets_to_distances(const std::vector<std::uint32_t> &offsets)
  {
    std::vector<std::uint32_t> distances{};
    if (offsets.empty()) {
      return distances;
    }
    distances.reserve(offsets.size() - 1);
    std::transform(
      offsets.begin() + 1,
      offsets.end(),
      offsets.begin(),
      std::back_inserter(distances),
      [](std::uint32_t a, std::uint32_t b) {
        return a - b;
      });
    return distances;
  }
  Section3_Model_Animation() = default;
  Section3_Model_Animation(
    [[maybe_unused]] const char *const           section_start,
    [[maybe_unused]] const std::span<const char> span)
    : m_section_header(span)
  {
    auto distances = offsets_to_distances(m_section_header.m_offsets);
    m_animations.reserve(m_section_header.m_count);
    distances.push_back(static_cast<std::uint32_t>(
      span.size() - m_section_header.m_offsets.back()));
    std::ranges::transform(
      m_section_header.m_offsets,
      distances,
      std::back_inserter(m_animations),
      [&](std::uint32_t offset, std::uint32_t size) -> Animation {
        return { span.subspan(offset, size) };
      });
  }
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_SECTION3_MODEL_ANIMATION_HPP
