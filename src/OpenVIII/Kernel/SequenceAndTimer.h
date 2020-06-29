//
// Created by pcvii on 6/27/2020.
//

#ifndef VIIIARCHIVE_SEQUENCEANDTIMER_H
#define VIIIARCHIVE_SEQUENCEANDTIMER_H
#include <cstdint>
#include <iostream>
namespace OpenVIII::Kernel {
struct SequenceAndTimer
{
private:
  std::uint8_t Sequence_{};
  std::uint8_t Timer_{};

public:
  [[nodiscard]] auto Sequence() const noexcept { return Sequence_; }
  [[nodiscard]] auto Timer() const noexcept { return Timer_; }
  friend std::ostream &operator<<(std::ostream &os, const SequenceAndTimer &input)
  {
    return os << '{' << static_cast<std::uint32_t>(input.Sequence()) << ", "
              << static_cast<std::uint32_t>(input.Timer()) << '}';
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_SEQUENCEANDTIMER_H
