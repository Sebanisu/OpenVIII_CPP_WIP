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
#ifndef VIIIARCHIVE_LAYERID_HPP
#define VIIIARCHIVE_LAYERID_HPP
#include <cstdint>
namespace open_viii::graphics::background {
struct LayerID
{
private:
  static constexpr std::uint8_t ID_MASK{ 0b1111'1110 };
  static constexpr std::uint8_t ID_SHIFT{ 1U };
  std::uint8_t                  m_data{};

public:
  LayerID() = default;
  friend auto operator==(const LayerID &left, const LayerID &right)
  {
    return left.m_data == right.m_data;
  }
  [[nodiscard]] std::uint8_t id() const noexcept
  {
    return static_cast<std::uint8_t>(static_cast<std::uint8_t>(m_data & ID_MASK)
                                     >> ID_SHIFT);
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_LAYERID_HPP
