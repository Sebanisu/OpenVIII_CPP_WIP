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

#ifndef VIIIARCHIVE_GFGROUP_HPP
#define VIIIARCHIVE_GFGROUP_HPP
#include <array>
#include <iostream>

namespace open_viii::kernel {
template<typename T> requires(std::unsigned_integral<T> || std::ranges::contiguous_range<T>) struct GFGroup
{
private:
  static constexpr auto MAX_G_FS = 16U;
  std::array<T, MAX_G_FS> m_gfs{};
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
  static constexpr auto QUETZALCOATL{ 0U };
  static constexpr auto SHIVA{ 1U };
  static constexpr auto IFRIT{ 2U };
  static constexpr auto SIREN{ 3U };
  static constexpr auto BROTHERS{ 4U };
  static constexpr auto DIABLOS{ 5U };
  static constexpr auto CARBUNCLE{ 6U };
  static constexpr auto LEVIATHAN{ 7U };
  static constexpr auto PANDEMONA{ 8U };
  static constexpr auto CERBERUS{ 9U };
  static constexpr auto ALEXANDER{ 10U };
  static constexpr auto DOOMTRAIN{ 11U };
  static constexpr auto BAHAMUT{ 12U };
  static constexpr auto CACTUAR{ 13U };
  static constexpr auto TONBERRY{ 14U };
  static constexpr auto EDEN{ 15U };
  //  template<typename maybeNumberT> static bool is_integral([[maybe_unused]] const maybeNumberT &value)
  //  {
  //    return std::is_integral_v<maybeNumberT>;
  //  }
  //  template<typename maybeNumberT> static bool is_integral() { return std::is_integral_v<maybeNumberT>; }

public:
  [[nodiscard]] T &quetzalcoatl() const noexcept { return m_gfs.at(QUETZALCOATL); }
  [[nodiscard]] T &shiva() const noexcept { return m_gfs.at(SHIVA); }
  [[nodiscard]] T &ifrit() const noexcept { return m_gfs.at(IFRIT); }
  [[nodiscard]] T &siren() const noexcept { return m_gfs.at(SIREN); }
  [[nodiscard]] T &brothers() const noexcept { return m_gfs.at(BROTHERS); }
  [[nodiscard]] T &diablos() const noexcept { return m_gfs.at(DIABLOS); }
  [[nodiscard]] T &carbuncle() const noexcept { return m_gfs.at(CARBUNCLE); }
  [[nodiscard]] T &leviathan() const noexcept { return m_gfs.at(LEVIATHAN); }
  [[nodiscard]] T &pandemona() const noexcept { return m_gfs.at(PANDEMONA); }
  [[nodiscard]] T &cerberus() const noexcept { return m_gfs.at(CERBERUS); }
  [[nodiscard]] T &alexander() const noexcept { return m_gfs.at(ALEXANDER); }
  [[nodiscard]] T &doomtrain() const noexcept { return m_gfs.at(DOOMTRAIN); }
  [[nodiscard]] T &bahamut() const noexcept { return m_gfs.at(BAHAMUT); }
  [[nodiscard]] T &cactuar() const noexcept { return m_gfs.at(CACTUAR); }
  [[nodiscard]] T &tonberry() const noexcept { return m_gfs.at(TONBERRY); }
  [[nodiscard]] T &eden() const noexcept { return m_gfs.at(EDEN); }
  friend auto &operator<<(std::ostream &os, const GFGroup<T> &in)
  {
    os << '{';
    bool first = true;
    for (const auto &item : in.m_gfs) {
      if (!first) {
        os << ", ";
      } else {
        first = false;
      }
      if (std::is_integral_v<decltype(item)>) {
        os << static_cast<std::size_t>(item);
      } else {
        os << item;
      }
    }
    return os << '}';
  }
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFGROUP_HPP
