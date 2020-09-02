// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_STATGROUPNOEVANOHIT_H
#define VIIIARCHIVE_STATGROUPNOEVANOHIT_H
#include <array>
#include <iostream>
namespace open_viii::kernel {
template<typename T> struct StatGroupNoEVANoHIT
{
private:
  static constexpr auto maxCount_{ 7U };

  static constexpr auto hp_{ 0U };
  static constexpr auto str_{ 1U };
  static constexpr auto vit_{ 2U };
  static constexpr auto mag_{ 3U };
  static constexpr auto spr_{ 4U };
  static constexpr auto spd_{ 5U };
  static constexpr auto luck_{ 6U };
  std::array<T, maxCount_> stats_{};
  template<typename maybeNumberT> static bool isIntegral([[maybe_unused]] const maybeNumberT &value)
  {
    return std::is_integral_v<maybeNumberT>;
  }
  template<typename maybeNumberT> static bool isIntegral() { return std::is_integral_v<maybeNumberT>; }

public:
  [[nodiscard]] T &HP() const noexcept { return stats_.at(hp_); }
  [[nodiscard]] T &STR() const noexcept { return stats_.at(str_); }
  [[nodiscard]] T &VIT() const noexcept { return stats_.at(vit_); }
  [[nodiscard]] T &MAG() const noexcept { return stats_.at(mag_); }
  [[nodiscard]] T &SPR() const noexcept { return stats_.at(spr_); }
  [[nodiscard]] T &SPD() const noexcept { return stats_.at(spd_); }
  [[nodiscard]] T &LUCK() const noexcept { return stats_.at(luck_); }
  friend std::ostream &operator<<(std::ostream &os, const StatGroupNoEVANoHIT<T> &input)
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
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_STATGROUPNOEVANOHIT_H
