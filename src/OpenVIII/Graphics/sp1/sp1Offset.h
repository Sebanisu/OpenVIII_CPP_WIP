//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1OFFSET_H
#define VIIIARCHIVE_SP1OFFSET_H
#include <cstdint>
namespace OpenVIII::Graphics {
struct sp1Offset
{
private:
  std::uint16_t offset_{};
  std::uint16_t count_{};

public:
  [[nodiscard]] auto Offset() const noexcept { return offset_; }
  [[nodiscard]] auto Count() const noexcept { return count_; }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1OFFSET_H
