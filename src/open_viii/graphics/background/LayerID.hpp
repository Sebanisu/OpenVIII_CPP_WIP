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
namespace open_viii::graphics::background {
struct LayerID
{
private:
  enum : std::uint8_t
  {
    ID_MASK         = 0b1111'1110,
    ID_INVERSE_MASK = 0b0000'0001,
    ID_SHIFT        = 1U,
  };
  std::uint8_t m_data = {};

public:
  static constexpr auto EXPECTED_SIZE = 1U;
  constexpr LayerID()                 = default;
  constexpr auto
    operator<=>(const LayerID &) const = default;
  [[nodiscard]] constexpr auto
    id() const noexcept
  {
    return static_cast<std::uint8_t>(
      static_cast<std::uint8_t>(m_data & ID_MASK) >> ID_SHIFT);
  }
  [[nodiscard]] constexpr auto
    with_id(std::uint8_t in_id) const noexcept
  {
    LayerID out{};
    out.m_data = static_cast<std::uint8_t>(
      (m_data & ID_INVERSE_MASK) | ((in_id << ID_SHIFT) & ID_MASK));
    return out;
  }
};
static_assert(sizeof(LayerID) == LayerID::EXPECTED_SIZE);
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_LAYERID_HPP
