//
// Created by pcvii on 9/2/2020.
//

#ifndef VIIIARCHIVE_LAYERID_H
#define VIIIARCHIVE_LAYERID_H
#include <cstdint>
namespace open_viii::graphics::background {
struct LayerID
{
private:
  static constexpr std::uint8_t ID_MASK{ 0b1111'1110 };
  static constexpr std::uint8_t ID_SHIFT{ 1U };
  std::uint8_t m_data{};

public:
  LayerID() = default;
  friend auto operator==(const LayerID &left, const LayerID &right) { return left.m_data == right.m_data; }
  [[nodiscard]] std::uint8_t id() const noexcept
  {
    return static_cast<std::uint8_t>(static_cast<std::uint8_t>(m_data & ID_MASK) >> ID_SHIFT);
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_LAYERID_H
