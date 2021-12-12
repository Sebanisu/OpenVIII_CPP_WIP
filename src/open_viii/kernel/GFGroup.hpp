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
namespace open_viii::kernel {
template<typename T>
requires(
  std::unsigned_integral<T> || std::ranges::contiguous_range<T>) struct GFGroup
{
public:
  enum
  {
    QUETZALCOATL,
    SHIVA,
    IFRIT,
    SIREN,
    BROTHERS,
    DIABLOS,
    CARBUNCLE,
    LEVIATHAN,
    PANDEMONA,
    CERBERUS,
    ALEXANDER,
    DOOMTRAIN,
    BAHAMUT,
    CACTUAR,
    TONBERRY,
    EDEN,
    MAX_GFS
  };
  constexpr auto
    operator<=>(const GFGroup<T> &right) const noexcept = default;
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
      }
      else {
        first = false;
      }
      if (std::is_integral_v<std::decay_t<decltype(item)>>) {
        os << +item;
      }
      else {
        os << item;
      }
    }
    return os << '}';
  }

private:
  std::array<T, MAX_GFS> m_gfs{};
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFGROUP_HPP
