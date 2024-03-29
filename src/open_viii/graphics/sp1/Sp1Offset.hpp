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
#ifndef VIIIARCHIVE_SP1OFFSET_HPP
#define VIIIARCHIVE_SP1OFFSET_HPP
namespace open_viii::graphics {
struct Sp1Offset
{
private:
  std::uint16_t m_offset{};
  std::uint16_t m_count{};

public:
  Sp1Offset() = default;
  [[nodiscard]] auto
    offset() const noexcept
  {
    return m_offset;
  }
  [[nodiscard]] auto
    count() const noexcept
  {
    return m_count;
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP1OFFSET_HPP
