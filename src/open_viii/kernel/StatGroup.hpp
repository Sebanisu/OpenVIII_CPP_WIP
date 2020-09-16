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

#ifndef VIIIARCHIVE_STATGROUP_HPP
#define VIIIARCHIVE_STATGROUP_HPP
#include <array>
#include <iostream>
#include <concepts>
namespace open_viii::kernel {
template<typename T> requires(std::unsigned_integral<T> || std::ranges::contiguous_range<T>) struct StatGroup
{
private:
  static constexpr auto STAT_COUNT = 9U;
  std::array<T, STAT_COUNT> m_stats{};
  static constexpr auto HP{ 0U };
  static constexpr auto STR{ 1U };
  static constexpr auto VIT{ 2U };
  static constexpr auto MAG{ 3U };
  static constexpr auto SPR{ 4U };
  static constexpr auto SPD{ 5U };
  static constexpr auto EVA{ 6U };
  static constexpr auto HIT{ 7U };
  static constexpr auto LUCK{ 8U };
  //  template<typename maybeNumberT> static bool is_integral([[maybe_unused]] const maybeNumberT &value)
  //  {
  //    return std::is_integral_v<maybeNumberT>;
  //  }
  // template<typename maybeNumberT> static bool is_integral() { return std::is_integral_v<maybeNumberT>; }

public:
  [[maybe_unused]] [[nodiscard]] T &hp() const noexcept { return m_stats.at(HP); }
  [[nodiscard]] T &str() const noexcept { return m_stats.at(STR); }
  [[maybe_unused]] [[nodiscard]] T &vit() const noexcept { return m_stats.at(VIT); }
  [[nodiscard]] T &mag() const noexcept { return m_stats.at(MAG); }
  [[nodiscard]] T &spr() const noexcept { return m_stats.at(SPR); }
  [[nodiscard]] T &spd() const noexcept { return m_stats.at(SPD); }
  [[maybe_unused]] [[nodiscard]] T &eva() const noexcept { return m_stats.at(EVA); }
  [[nodiscard]] T &hit() const noexcept { return m_stats.at(HIT); }
  [[maybe_unused]] [[nodiscard]] T &luck() const noexcept { return m_stats.at(LUCK); }
  friend std::ostream &operator<<(std::ostream &os, const StatGroup<T> &input)
  {
    bool first{ true };
    for (const auto &item : input.m_stats) {
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
          if (std::is_integral_v<decltype(subItem)>) {
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
#endif// VIIIARCHIVE_STATGROUP_HPP
