//
// Created by pcvii on 6/22/2020.
//

#ifndef VIIIARCHIVE_CRISISLEVELT_H
#define VIIIARCHIVE_CRISISLEVELT_H
#include <iostream>
#include <array>
namespace OpenVIII::Kernel {
template<typename T> struct CrisisLevelT
{
private:
  std::array<T, 4> levels_{};

public:
  [[nodiscard]] auto &begin() { return levels_.begin(); }
  [[nodiscard]] auto &end() { return levels_.end(); }
  [[nodiscard]] auto Level1() const noexcept { return levels_.at(0); }
  [[nodiscard]] auto Level2() const noexcept { return levels_.at(1); }
  [[nodiscard]] auto Level3() const noexcept { return levels_.at(2); }
  [[nodiscard]] auto Level4() const noexcept { return levels_.at(3); }
  friend std::ostream &operator<<(std::ostream &os, const CrisisLevelT<T> &input)
  {

    os << '{';
    bool start {true};
    for (const auto &level : input.levels_) {
      if (!start) {
        os << ", ";
      }
      if constexpr (std::is_integral_v<T>) {
        os << static_cast<uint32_t>(level);
      } else {
        os << level;
      }
      start = false;
    }
    return os << '}';
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_CRISISLEVELT_H
