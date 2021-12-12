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
#ifndef VIIIARCHIVE_CRISISLEVELT_HPP
#define VIIIARCHIVE_CRISISLEVELT_HPP
namespace open_viii::kernel {
// template<typename T>
// struct CrisisLevelT
//{
// private:
//   std::array<T, 4> m_levels{};
//
// public:
//   constexpr auto
//     operator<=>(const CrisisLevelT<T> &right) const noexcept = default;
//   [[nodiscard]] auto &
//     begin()
//   {
//     return m_levels.begin();
//   }
//   [[nodiscard]] auto &
//     end()
//   {
//     return m_levels.end();
//   }
//   [[nodiscard]] constexpr auto
//     level1() const noexcept
//   {
//     return m_levels.at(0);
//   }
//   [[nodiscard]] constexpr auto
//     level2() const noexcept
//   {
//     return m_levels.at(1);
//   }
//   [[nodiscard]] constexpr auto
//     level3() const noexcept
//   {
//     return m_levels.at(2);
//   }
//   [[nodiscard]] constexpr auto
//     level4() const noexcept
//   {
//     return m_levels.at(3);
//   }
//   friend std::ostream &
//     operator<<(std::ostream &os, const CrisisLevelT<T> &input)
//   {
//     os << '{';
//     bool start{ true };
//     for (const auto &level : input.m_levels) {
//       if (!start) {
//         os << ", ";
//       }
//       if constexpr (std::is_integral_v<T>) {
//         os << static_cast<uint32_t>(level);
//       }
//       else {
//         os << level;
//       }
//       start = false;
//     }
//     return os << '}';
//   }
// };

template<typename T>
using CrisisLevelT = std::array<T, 4>;
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CRISISLEVELT_HPP
