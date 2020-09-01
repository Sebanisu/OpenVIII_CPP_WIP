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
  static constexpr auto widthsOffsetValue = 8U;
  std::uint32_t widthsOffset_{};
  std::uint32_t timOffset_{};
  std::vector<_4bitValues> widths{};
  tim tim_{};
  auto widthsSize() const noexcept { return timOffset_ - widthsOffset_; }

public:
  tdw() = default;
  explicit tdw([[maybe_unused]] std::span<const char> buffer)
  {
    const auto bufferBak = buffer;
    size_t sz32 = sizeof(std::uint32_t);
    if (std::ranges::size(buffer) < sz32) {
      return;
    }
    std::memcpy(&widthsOffset_, std::ranges::data(buffer), sz32);
    buffer = buffer.subspan(sz32);
    if (widthsOffsetValue != widthsOffset_) {
      widthsOffset_ = 0U;
      return;
    }
    if (std::ranges::size(buffer) < sz32) {
      return;
    }
    std::memcpy(&timOffset_, std::ranges::data(buffer), sz32);
    buffer = buffer.subspan(sz32);

    if (std::ranges::size(buffer) < widthsSize()) {
      return;
    }
    widths.resize(widthsSize());
    std::memcpy(std::ranges::data(widths), std::ranges::data(bufferBak.subspan(widthsOffset_)), widthsSize());
    tim_ = tim{ bufferBak.subspan(timOffset_) };
  }
  [[nodiscard]] auto operator[](size_t i) const
  {
    bool first = (i % 2U == 0U);
    i /= 2U;
    if (first) {
      return static_cast<std::uint8_t>(widths.at(i).first);
    }
    return static_cast<std::uint8_t>(widths.at(i).second);
  }
  [[nodiscard]] auto size() const { return std::ranges::size(widths) * 2U; }
  [[nodiscard]] const auto &TIM() const { return tim_; }
  friend std::ostream &operator<<(std::ostream &os, const tdw &t)
  {
    //    if(t.size()>0) {
    os << t.size() << " char widths: ";
    for (const _4bitValues &w : t.widths) {
      os << static_cast<std::uint32_t>(w.first) << ", " << static_cast<std::uint32_t>(w.second) << ", ";
    }
    return os << '\n' << t.tim_;
    //    }
    //    return os;
  }
  void Save(const std::string_view &path) { tim_.Save(path); }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TDW_H
