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
  std::uint32_t offsetCount_{};
  std::vector<sp1Offset> offsets_{};

public:
  sp1Header() = default;
  explicit sp1Header(std::span<const char> buffer)
  {
    {
      size_t sz = sizeof(std::uint32_t);
      if (sz > std::ranges::size(buffer)) {
        return;
      }
      std::memcpy(&offsetCount_, std::ranges::data(buffer), sz);
      buffer = buffer.subspan(sz);
    }
    {
      offsets_.resize(offsetCount_);
      size_t sz = static_cast<std::size_t>(offsetCount_) * sizeof(sp1Offset);
      if (sz > std::ranges::size(buffer)) {
        offsetCount_ = {};
        offsets_ = {};
        return;
      }
      std::memcpy(std::ranges::data(offsets_), std::ranges::data(buffer), sz);
    }
  }
  [[nodiscard]] auto size() const noexcept { return offsetCount_; }
  [[nodiscard]] const auto &Offsets() const noexcept { return offsets_; }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1HEADER_H
