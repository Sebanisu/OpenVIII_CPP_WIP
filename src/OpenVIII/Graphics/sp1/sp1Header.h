//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1HEADER_H
#define VIIIARCHIVE_SP1HEADER_H
#include <cstdint>
#include <cstring>
#include <vector>
#include <span>
#include "sp1Offset.h"
namespace OpenVIII::Graphics {
struct sp1Header
{
private:
  std::uint32_t pointerCount_{};
  std::vector<sp1Offset> offsets_{};

public:
  sp1Header() = default;
  explicit sp1Header(std::span<const char> buffer)
  {
    std::memcpy(pointerCount_ &, std::ranges::data(buffer), sizeof(pointerCount_));
    buffer = buffer.subspan(sizeof(pointerCount_));

    offsets_.resize(pointerCount_);
    std::memcpy(std::ranges::data(offsets_), std::ranges::data(buffer), std::ranges::size(buffer) * sizeof(sp1Offset));
  }
  [[nodiscard]] auto size() const noexcept { return pointerCount_; }
  [[nodiscard]] const auto &Offsets() const noexcept { return offsets_; }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1HEADER_H
