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
#include "GFID.hpp"
#include "open_viii/tools/array_wrapper.hpp"
namespace open_viii::kernel {
template<typename T>
  requires(std::unsigned_integral<T> || std::ranges::contiguous_range<T>)
struct GFGroup
{
public:
  constexpr auto
    operator<=>(const GFGroup<T> &right) const noexcept
    = default;
  [[nodiscard]] constexpr T
    quetzalcoatl() const noexcept
  {
    return m_gfs.at(GFID::Quezacotl);
  }
  [[nodiscard]] constexpr T
    shiva() const noexcept
  {
    return m_gfs.at(GFID::Shiva);
  }
  [[nodiscard]] constexpr T
    ifrit() const noexcept
  {
    return m_gfs.at(GFID::Ifrit);
  }
  [[nodiscard]] constexpr T
    siren() const noexcept
  {
    return m_gfs.at(GFID::Siren);
  }
  [[nodiscard]] constexpr T
    brothers() const noexcept
  {
    return m_gfs.at(GFID::Brothers);
  }
  [[nodiscard]] constexpr T
    diablos() const noexcept
  {
    return m_gfs.at(GFID::Diablos);
  }
  [[nodiscard]] constexpr T
    carbuncle() const noexcept
  {
    return m_gfs.at(GFID::Carbuncle);
  }
  [[nodiscard]] constexpr T
    leviathan() const noexcept
  {
    return m_gfs.at(GFID::Leviathan);
  }
  [[nodiscard]] constexpr T
    pandemona() const noexcept
  {
    return m_gfs.at(GFID::Pandemona);
  }
  [[nodiscard]] constexpr T
    cerberus() const noexcept
  {
    return m_gfs.at(GFID::Cerberus);
  }
  [[nodiscard]] constexpr T
    alexander() const noexcept
  {
    return m_gfs.at(GFID::Alexander);
  }
  [[nodiscard]] constexpr T
    doomtrain() const noexcept
  {
    return m_gfs.at(GFID::Doomtrain);
  }
  [[nodiscard]] constexpr T
    bahamut() const noexcept
  {
    return m_gfs.at(GFID::Bahamut);
  }
  [[nodiscard]] constexpr T
    cactuar() const noexcept
  {
    return m_gfs.at(GFID::Cactuar);
  }
  [[nodiscard]] constexpr T
    tonberry() const noexcept
  {
    return m_gfs.at(GFID::Tonberry);
  }
  [[nodiscard]] constexpr T
    eden() const noexcept
  {
    return m_gfs.at(GFID::Eden);
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
  tools::array_wrapper<T, static_cast<std::uint8_t>(GFID::Eden) + 1> m_gfs{};
};
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_GFGROUP_HPP
