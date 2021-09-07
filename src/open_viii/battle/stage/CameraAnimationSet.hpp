//
// Created by pcvii on 11/18/2020.
//
#ifndef VIIIARCHIVE_CAMERAANIMATIONSET_HPP
#define VIIIARCHIVE_CAMERAANIMATIONSET_HPP
#include "CameraAnimation.hpp"
#include <array>
#include <cstdint>
namespace open_viii::battle::stage {
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#CameraAnimationSet
 */
struct CameraAnimationSet
{
private:
  static constexpr auto            COUNT = 8U;
  std::array<std::uint16_t, COUNT> m_animation_offsets{};

public:
  //  std::uint16_t operator[](const size_t &i)
  //  {
  //    return m_animation_offsets[i];
  //  }
  [[nodiscard]] std::uint16_t
    at(const size_t &i) const
  {
    return m_animation_offsets.at(i);
  }
  [[nodiscard]] static std::size_t
    size() noexcept
  {
    return COUNT;
  }
  [[nodiscard]] auto
    operator*() noexcept
  {
    return &m_animation_offsets;
  }

  [[nodiscard]] const std::array<std::uint16_t, COUNT> &
    animation_offsets() const noexcept
  {
    return m_animation_offsets;
  }
};

inline std::ostream &
  operator<<(std::ostream &os, const CameraAnimationSet &in)
{
  os << '[';
  {
    bool first = true;
    std::ranges::for_each(in.animation_offsets(),
                          [&os, &first](const std::uint16_t &c) {
                            if (!first) {
                              os << ',';
                            }
                            first = false;
                            os << "0x" << std::hex << std::uppercase
                               << static_cast<std::uint16_t>(c)
                               << std::nouppercase << std::dec;
                          });
  }
  return os << "]";
}
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAANIMATIONSET_HPP
