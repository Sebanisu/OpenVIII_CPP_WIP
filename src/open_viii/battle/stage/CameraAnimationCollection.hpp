//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#define VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#include "CameraAnimation.hpp"
#include "CameraAnimationSet.hpp"
#include "open_viii/tools/Tools.hpp"
#include <cstdint>
#include <ostream>
#include <span>
#include <vector>
namespace open_viii::battle::stage {
/**
 * @see
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Animations_Collection
 */
struct CameraAnimationCollection
{
private:
  std::uint16_t m_set_count{};
  std::vector<std::uint16_t> m_set_offsets{};
  std::uint16_t m_camera_end{};
  std::vector<CameraAnimationSet> m_camera_animation_set{};
  std::vector<std::vector<CameraAnimation>> m_camera_animation{};

public:
  constexpr CameraAnimationCollection() = default;
  explicit CameraAnimationCollection(std::span<const char> span)
  {
    // const auto start = span.begin();
    const auto backup_span = span;
    // std::ranges::data
    Tools::read_val(span, m_set_count);
    span = span.subspan(sizeof(std::uint16_t));
    m_set_offsets.resize(m_set_count);
    Tools::read_val(span, m_set_offsets);
    span = span.subspan(sizeof(std::uint16_t) * m_set_count);
    Tools::read_val(span, m_camera_end);
    // const auto size{sizeof(CameraAnimationSet) * m_set_count};
    span = span.subspan(sizeof(std::uint16_t));
    // TODO sets aren't grouped together will need to use offsets
    m_camera_animation_set.reserve(m_set_count);
    std::ranges::transform(m_set_offsets,
      std::back_inserter(m_camera_animation_set),
      [&backup_span](const std::uint16_t &offset) {
        return Tools::read_val<CameraAnimationSet>(backup_span.subspan(
          offset + 2U));// skipping 2U makes it match the values in open_viii
      });
    m_camera_animation.resize(m_set_count);
//    for (std::size_t j{};
//         j < m_camera_animation.size() && j < m_camera_animation_set.size();
//         j++) {
//      const auto &set = m_camera_animation_set.at(j);
//      const auto &offset = m_set_offsets.at(j);
//      m_camera_animation.at(j).reserve(CameraAnimationSet::size());
//      for (std::size_t i{}; i < CameraAnimationSet::size(); i++) {
//        const auto start_of_animation = offset + 2U + set.at(i);
//        //        const auto full_pos =
//        //          sizeof(CameraHeader) + sizeof(CameraSettings) +
//        //          start_of_animation;
//        m_camera_animation.at(j).emplace_back(Tools::read_val<CameraAnimation>(
//          backup_span.subspan(start_of_animation)));
//      }
      //TODO rework this. It does not work
//    }
  }
  friend std::ostream &operator<<(
    std::ostream &os, const CameraAnimationCollection &in)
  {
    os << "{\n\t\t\t SET COUNT: " << in.m_set_count
       << "\n\t\t\t SET OFFSETS: " << '[';
    {
      bool first = true;
      std::ranges::for_each(
        in.m_set_offsets, [&os, &first](const std::uint16_t &c) {
          if (!first) {
            os << ',';
          }

          first = false;
          os << "0x" << std::hex << std::uppercase
             << static_cast<std::uint16_t>(c) << std::dec << std::nouppercase;
        });
    }
    os << ']' << "\n\t\t\t END: " << in.m_camera_end
       << "\n\t\t\t SETS: " << '[';
    {
      bool first = true;
      std::ranges::for_each(
        in.m_camera_animation_set, [&os, &first](const CameraAnimationSet &c) {
          if (!first) {
            os << ",";
          }
          os << "\n\t\t\t\t";
          first = false;
          os << c;
        });
    }
    return os << "]";
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
