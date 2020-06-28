//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_STATGROUP_H
#define VIIIARCHIVE_STATGROUP_H
#include <array>
#include <iostream>
namespace OpenVIII::Kernel {
template<typename T> struct StatGroup
{
private:
  static constexpr auto statCount = 9U;
  std::array<T, statCount> stats_{};
  static constexpr auto HP_{ 0U };
  static constexpr auto STR_{ 1U };
  static constexpr auto VIT_{ 2U };
  static constexpr auto MAG_{ 3U };
  static constexpr auto SPR_{ 4U };
  static constexpr auto SPD_{ 5U };
  static constexpr auto EVA_{ 6U };
  static constexpr auto HIT_{ 7U };
  static constexpr auto LUCK_{ 8U };
  template<typename maybeNumberT> static bool isIntegral([[maybe_unused]] const maybeNumberT &value)
  {
    return std::is_integral_v<maybeNumberT>;
  }
  template<typename maybeNumberT> static bool isIntegral() { return std::is_integral_v<maybeNumberT>; }

public:
  [[nodiscard]] T &HP() const noexcept { return stats_.at(HP_); }
  [[nodiscard]] T &STR() const noexcept { return stats_.at(STR_); }
  [[nodiscard]] T &VIT() const noexcept { return stats_.at(VIT_); }
  [[nodiscard]] T &MAG() const noexcept { return stats_.at(MAG_); }
  [[nodiscard]] T &SPR() const noexcept { return stats_.at(SPR_); }
  [[nodiscard]] T &SPD() const noexcept { return stats_.at(SPD_); }
  [[nodiscard]] T &EVA() const noexcept { return stats_.at(EVA_); }
  [[nodiscard]] T &HIT() const noexcept { return stats_.at(HIT_); }
  [[nodiscard]] T &LUCK() const noexcept { return stats_.at(LUCK_); }
  friend std::ostream &operator<<(std::ostream &os, const StatGroup<T> &input)
  {
    bool first{ true };
    for (const auto &item : input.stats_) {
      if (!first) {
        os << ", ";
      } else {
        first = false;
      }
      if constexpr (std::is_integral_v<T>) {
        os << static_cast<size_t>(item);
      } else {
        os << '{';
        bool subFirst{ true };
        for (const auto &subItem : item) {
          if (!subFirst) {
            os << ", ";
          } else {
            subFirst = false;
          }
          if (isIntegral(subItem)) {
            os << static_cast<size_t>(subItem);
          } else {
            os << subItem;
          }
        }
        os << '}';
      }
    }
    return os;
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_STATGROUP_H
