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

#ifndef VIIIARCHIVE_CRISISLEVELT_H
#define VIIIARCHIVE_CRISISLEVELT_H
#include <iostream>
#include <array>
namespace open_viii::kernel {
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
    bool start{ true };
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
}// namespace open_viii::kernel
#endif// VIIIARCHIVE_CRISISLEVELT_H
