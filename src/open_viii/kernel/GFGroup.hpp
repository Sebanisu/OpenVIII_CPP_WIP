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
#include <compare>
#include <iostream>
namespace open_viii::kernel {
template<typename T>
requires(
  std::unsigned_integral<T> || std::ranges::contiguous_range<T>) struct GFGroup
{
public:
  static constexpr auto MAX_G_FS = 16U;
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
  constexpr auto operator<=>(const GFGroup<T> &right) const noexcept = default;
  [[nodiscard]] constexpr T
    quetzalcoatl() const noexcept
  {
    return m_gfs.at(QUETZALCOATL);
  }
  [[nodiscard]] constexpr T
    shiva() const noexcept
  {
    return m_gfs.at(SHIVA);
  }
  [[nodiscard]] constexpr T
    ifrit() const noexcept
  {
    return m_gfs.at(IFRIT);
  }
  [[nodiscard]] constexpr T
    siren() const noexcept
  {
    return m_gfs.at(SIREN);
  }
  [[nodiscard]] constexpr T
    brothers() const noexcept
  {
    return m_gfs.at(BROTHERS);
  }
  [[nodiscard]] constexpr T
    diablos() const noexcept
  {
    return m_gfs.at(DIABLOS);
  }
  [[nodiscard]] constexpr T
    carbuncle() const noexcept
  {
    return m_gfs.at(CARBUNCLE);
  }
  [[nodiscard]] constexpr T
    leviathan() const noexcept
  {
    return m_gfs.at(LEVIATHAN);
  }
  [[nodiscard]] constexpr T
    pandemona() const noexcept
  {
    return m_gfs.at(PANDEMONA);
  }
  [[nodiscard]] constexpr T
    cerberus() const noexcept
  {
    return m_gfs.at(CERBERUS);
  }
  [[nodiscard]] constexpr T
    alexander() const noexcept
  {
    return m_gfs.at(ALEXANDER);
  }
  [[nodiscard]] constexpr T
    doomtrain() const noexcept
  {
    return m_gfs.at(DOOMTRAIN);
  }
  [[nodiscard]] constexpr T
    bahamut() const noexcept
  {
    return m_gfs.at(BAHAMUT);
  }
  [[nodiscard]] constexpr T
    cactuar() const noexcept
  {
    return m_gfs.at(CACTUAR);
  }
  [[nodiscard]] constexpr T
    tonberry() const noexcept
  {
    return m_gfs.at(TONBERRY);
  }
  [[nodiscard]] constexpr T
    eden() const noexcept
  {
    return m_gfs.at(EDEN);
  }
  friend auto &
    operator<<(std::ostream &os, const GFGroup<T> &in)
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

private:
  std::array<T, MAX_G_FS> m_gfs{};
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFGROUP_HPP
