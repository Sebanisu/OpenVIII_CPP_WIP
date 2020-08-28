//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TDW_H
#define VIIIARCHIVE_TDW_H
#include "tim.h"
#include "_4bitValues.h"
#include <cstdint>
#include <cstring>
namespace OpenVIII::Graphics {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_TDW
 */
struct tdw
{
private:
  std::uint32_t widthsOffset{};
  std::uint32_t timOffset{};
  std::vector<_4bitValues> widths{};
  tim tim_{};
  auto widthsSize() const noexcept { return timOffset - widthsOffset; }

public:
  tdw() = default;
  explicit tdw([[maybe_unused]] std::span<const char> buffer)
  {
    const auto bufferBak = buffer;
    std::memcpy(&widthsOffset, std::ranges::data(buffer), sizeof(std::uint32_t));
    buffer = buffer.subspan(sizeof(std::uint32_t));
    std::memcpy(&timOffset, std::ranges::data(buffer), sizeof(std::uint32_t));
    buffer = buffer.subspan(sizeof(std::uint32_t));
    widths.resize(widthsSize());
    std::memcpy(std::ranges::data(widths), std::ranges::data(bufferBak.subspan(widthsOffset)), widthsSize());
    tim_ = tim{ bufferBak.subspan(timOffset) };
  }
  [[nodiscard]] auto operator[](size_t i) const {
    bool first = (i%2U == 0U);
    i/=2U;
    if(first) {
      return static_cast<std::uint8_t>( widths.at(i).first);
    }
    return static_cast<std::uint8_t>( widths.at(i).second);
  }
  [[nodiscard]] auto size() const
  {
    return std::ranges::size(widths)*2U;
  }
  [[nodiscard]] const auto &TIM()const
  {
    return tim_;
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TDW_H
