//
// Created by pcvii on 11/18/2020.
//
#ifndef VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#define VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
#include "CameraAnimation.hpp"
#include "CameraAnimationSet.hpp"
#include "open_viii/tools/Tools.hpp"
namespace open_viii::battle::stage {
/**
 * @brief Main struct representing a collection of camera animations for a
 * battle stage.
 * @details Each BattleCameraSet holds 8 animations.
 * @see
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_Animations_Collection
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Camera/BattleCameraCollection.cs
 */
struct CameraAnimationCollection
{
private:
  std::span<const char>                     backup_span{};
  /**
   * @brief The number of camera animation sets.
   */
  std::uint16_t                             m_set_count{};

  /**
   * @brief A vector containing the offsets for each camera animation set.
   */
  std::vector<std::uint16_t>                m_set_offsets{};

  /**
   * @brief The end position of the camera data.
   */
  std::uint16_t                             m_camera_end{};

  /**
   * @brief A vector containing the camera animation sets.
   */
  std::vector<CameraAnimationSet>           m_camera_animation_set{};

  /**
   * @brief A vector containing vectors of camera animations for each set.
   */
  std::vector<std::vector<CameraAnimation>> m_camera_animation{};

public:
  /**
   * @brief Default constructor for CameraAnimationCollection.
   */
  constexpr CameraAnimationCollection() = default;

  /**
   * @brief Constructs a CameraAnimationCollection from a span of bytes.
   * @param span The span of bytes to parse the CameraAnimationCollection from.
   */
  explicit CameraAnimationCollection(std::span<const char> span)
    : backup_span(span),
      m_set_count(tools::read_val<decltype(m_set_count)>(span)),
      m_set_offsets(
        tools::read_vals<std::ranges::range_value_t<decltype(m_set_offsets)>>(
          span,
          m_set_count)),
      m_camera_end(tools::read_val<decltype(m_camera_end)>(span))
  {
    // TODO sets aren't grouped together will need to use offsets
    m_camera_animation_set.reserve(m_set_count);
    std::ranges::transform(
      m_set_offsets,
      std::back_inserter(m_camera_animation_set),
      [this](const std::uint16_t &offset) {
        return tools::read_val<CameraAnimationSet>(backup_span.subspan(
          offset + 2U));// skipping 2U makes it match the values in open_viii
      });
    m_camera_animation.resize(m_set_count);
    //    for (std::size_t j{};
    //         j < m_camera_animation.size() && j <
    //         m_camera_animation_set.size(); j++) {
    //      const auto &set = m_camera_animation_set.at(j);
    //      const auto &offset = m_set_offsets.at(j);
    //      m_camera_animation.at(j).reserve(CameraAnimationSet::size());
    //      for (std::size_t i{}; i < CameraAnimationSet::size(); i++) {
    //        const auto start_of_animation = offset + 2U + set.at(i);
    //        //        const auto full_pos =
    //        //          sizeof(CameraHeader) + sizeof(CameraSettings) +
    //        //          start_of_animation;
    //        m_camera_animation.at(j).emplace_back(tools::read_val<CameraAnimation>(
    //          backup_span.subspan(start_of_animation)));
    //      }
    // TODO rework this. It does not work
    //    }
  }

  /**
   * @brief Get the number of camera animation sets.
   * @return The number of camera animation sets.
   */
  [[nodiscard]] std::uint16_t
    set_count() const noexcept
  {
    return m_set_count;
  }

  /**
   * @brief Get the vector of set offsets.
   * @return A reference to the vector of set offsets.
   */
  [[nodiscard]] const std::vector<std::uint16_t> &
    set_offsets() const noexcept
  {
    return m_set_offsets;
  }

  /**
   * @brief Get the camera end position.
   * @return The camera end position.
   */
  [[nodiscard]] std::uint16_t
    camera_end() const noexcept
  {
    return m_camera_end;
  }

  /**
   * @brief Get the vector of camera animation sets.
   * @return A reference to the vector of camera animation sets.
   */
  [[nodiscard]] const std::vector<CameraAnimationSet> &
    camera_animation_set() const noexcept
  {
    return m_camera_animation_set;
  }

  /**
   * @brief Get the vector of vectors of camera animations.
   * @return A reference to the vector of vectors of camera animations.
   */
  [[nodiscard]] const std::vector<std::vector<CameraAnimation>> &
    camera_animation() const noexcept
  {
    return m_camera_animation;
  }
};

/**
 * @brief Output stream operator for CameraAnimationCollection.
 * @param os The output stream to write the CameraAnimationCollection to.
 * @param in The CameraAnimationCollection to write to the output stream.
 * @return A reference to the output stream.
 */
inline std::ostream &
  operator<<(std::ostream &os, const CameraAnimationCollection &in)
{
  os << "{\n\t\t\t SET COUNT: " << in.set_count()
     << "\n\t\t\t SET OFFSETS: " << '[';
  {
    bool first = true;
    std::ranges::for_each(
      in.set_offsets(),
      [&os, &first](const std::uint16_t &c) {
        if (!first) {
          os << ',';
        }
        first = false;
        os << "0x" << std::hex << std::uppercase
           << static_cast<std::uint16_t>(c) << std::dec << std::nouppercase;
      });
  }
  os << ']' << "\n\t\t\t END: " << in.camera_end() << "\n\t\t\t SETS: " << '[';
  {
    bool first = true;
    std::ranges::for_each(
      in.camera_animation_set(),
      [&os, &first](const CameraAnimationSet &c) {
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
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAANIMATIONCOLLECTION_HPP
