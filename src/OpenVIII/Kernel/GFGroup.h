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

#ifndef VIIIARCHIVE_GFGROUP_H
#define VIIIARCHIVE_GFGROUP_H
#include <array>
#include <iostream>

namespace OpenVIII::Kernel {
template<typename T> struct GFGroup
{
private:
  static constexpr auto maxGFs = 16U;
  std::array<T, maxGFs> gfs_{};
  //  static constexpr auto Quetzalcoatl_{};
  //  static constexpr auto Shiva_{};
  //  static constexpr auto Ifrit_{};
  //  static constexpr auto Siren_{};
  //  static constexpr auto Brothers_{};
  //  static constexpr auto Diablos_{};
  //  static constexpr auto Carbuncle_{};
  //  static constexpr auto Leviathan_{};
  //  static constexpr auto Pandemona_{};
  //  static constexpr auto Cerberus_{};
  //  static constexpr auto Alexander_{};
  //  static constexpr auto Doomtrain_{};
  //  static constexpr auto Bahamut_{};
  //  static constexpr auto Cactuar_{};
  //  static constexpr auto Tonberry_{};
  //  static constexpr auto Eden_{};
  static constexpr auto Quetzalcoatl_{ 0U };
  static constexpr auto Shiva_{ 1U };
  static constexpr auto Ifrit_{ 2U };
  static constexpr auto Siren_{ 3U };
  static constexpr auto Brothers_{ 4U };
  static constexpr auto Diablos_{ 5U };
  static constexpr auto Carbuncle_{ 6U };
  static constexpr auto Leviathan_{ 7U };
  static constexpr auto Pandemona_{ 8U };
  static constexpr auto Cerberus_{ 9U };
  static constexpr auto Alexander_{ 10U };
  static constexpr auto Doomtrain_{ 11U };
  static constexpr auto Bahamut_{ 12U };
  static constexpr auto Cactuar_{ 13U };
  static constexpr auto Tonberry_{ 14U };
  static constexpr auto Eden_{ 15U };
  template<typename maybeNumberT> static bool isIntegral([[maybe_unused]] const maybeNumberT &value)
  {
    return std::is_integral_v<maybeNumberT>;
  }
  template<typename maybeNumberT> static bool isIntegral() { return std::is_integral_v<maybeNumberT>; }

public:
  [[nodiscard]] T &Quetzalcoatl() const noexcept { return gfs_.at(Quetzalcoatl_); }
  [[nodiscard]] T &Shiva() const noexcept { return gfs_.at(Shiva_); }
  [[nodiscard]] T &Ifrit() const noexcept { return gfs_.at(Ifrit_); }
  [[nodiscard]] T &Siren() const noexcept { return gfs_.at(Siren_); }
  [[nodiscard]] T &Brothers() const noexcept { return gfs_.at(Brothers_); }
  [[nodiscard]] T &Diablos() const noexcept { return gfs_.at(Diablos_); }
  [[nodiscard]] T &Carbuncle() const noexcept { return gfs_.at(Carbuncle_); }
  [[nodiscard]] T &Leviathan() const noexcept { return gfs_.at(Leviathan_); }
  [[nodiscard]] T &Pandemona() const noexcept { return gfs_.at(Pandemona_); }
  [[nodiscard]] T &Cerberus() const noexcept { return gfs_.at(Cerberus_); }
  [[nodiscard]] T &Alexander() const noexcept { return gfs_.at(Alexander_); }
  [[nodiscard]] T &Doomtrain() const noexcept { return gfs_.at(Doomtrain_); }
  [[nodiscard]] T &Bahamut() const noexcept { return gfs_.at(Bahamut_); }
  [[nodiscard]] T &Cactuar() const noexcept { return gfs_.at(Cactuar_); }
  [[nodiscard]] T &Tonberry() const noexcept { return gfs_.at(Tonberry_); }
  [[nodiscard]] T &Eden() const noexcept { return gfs_.at(Eden_); }
  friend auto &operator<<(std::ostream &os, const GFGroup<T> &in)
  {
    os << '{';
    bool first = true;
    for (const auto &item : in.gfs_) {
      if (!first) {
        os << ", ";
      } else {
        first = false;
      }
      if (isIntegral(item)) {
        os << static_cast<std::size_t>(item);
      } else {
        os << item;
      }
    }
    return os << '}';
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_GFGROUP_H
