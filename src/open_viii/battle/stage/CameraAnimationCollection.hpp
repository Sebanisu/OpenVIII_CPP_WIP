//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#define VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#include "open_viii/tools/Tools.hpp"
#include <cstdint>
#include <ostream>
#include <span>
#include <vector>
namespace open_viii::battle::stage {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Animations_Collection
 */
struct CameraAnimationCollection
{
private:
  std::uint16_t m_number_of_sets{};
  std::vector<std::uint16_t> m_offsets_to_sets{};
  std::uint16_t m_camera_end{};

public:
  constexpr CameraAnimationCollection() = default;
  explicit CameraAnimationCollection(std::span<const char> span)
  {
    // std::ranges::data
    Tools::read_val(span, m_number_of_sets);
    span = span.subspan(sizeof(std::uint16_t));
    m_offsets_to_sets.resize(m_number_of_sets);
    Tools::read_val(span, m_offsets_to_sets);
    span = span.subspan(sizeof(std::uint16_t) * m_number_of_sets);
    Tools::read_val(span, m_camera_end);
  }
  friend std::ostream &operator<<(std::ostream &os, const CameraAnimationCollection &in)
  {
    os << '{' << in.m_number_of_sets << ',' << std::hex << std::uppercase << '{';
    bool first = true;
    std::ranges::for_each(in.m_offsets_to_sets, [&os, &first](const std::uint16_t &c) {
      if (!first) {
        os << ',';
      }

      first = false;
      os << static_cast<std::uint16_t>(c);
    });
    return os << std::dec << std::nouppercase << '}' << ',' << in.m_camera_end << "}\n";
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
