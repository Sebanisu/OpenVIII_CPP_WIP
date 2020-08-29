//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP2HEADER_H
#define VIIIARCHIVE_SP2HEADER_H
#include <cstdint>
#include <vector>
#include <span>
#include <ranges>
namespace OpenVIII::Graphics {
struct sp2Header
{
private:
  std::uint32_t count_{};
  std::vector<std::uint32_t> offsets_{};

public:
  sp2Header() = default;
  explicit sp2Header(std::span<const char> buffer)
  {
    std::memcpy(&count_, std::ranges::data(buffer), sizeof(count_));
    buffer = buffer.subspan(sizeof(count_));
    offsets_.resize(count_);
    std::memcpy(std::ranges::data(offsets_), std::ranges::data(buffer), sizeof(std::uint32_t) * count_);
  }
  [[nodiscard]] const auto &Offsets() const noexcept { return offsets_; }
  [[nodiscard]] auto &size() const noexcept { return count_; }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP2HEADER_H
