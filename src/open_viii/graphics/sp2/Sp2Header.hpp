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

#ifndef VIIIARCHIVE_SP2HEADER_HPP
#define VIIIARCHIVE_SP2HEADER_HPP
#include <cstdint>
#include <ranges>
#include <span>
#include <vector>
namespace open_viii::graphics {
struct Sp2Header
{
private:
  std::uint32_t m_count{};
  std::vector<std::uint32_t> m_offsets{};

public:
  Sp2Header() = default;
  explicit Sp2Header(std::span<const char> buffer)
  {
    std::memcpy(&m_count, std::ranges::data(buffer), sizeof(m_count));
    buffer = buffer.subspan(sizeof(m_count));
    m_offsets.resize(m_count);
    std::memcpy(std::ranges::data(m_offsets),
      std::ranges::data(buffer),
      sizeof(std::uint32_t) * m_count);
  }
  [[nodiscard]] const auto &offsets() const noexcept
  {
    return m_offsets;
  }
  [[nodiscard]] auto &size() const noexcept
  {
    return m_count;
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2HEADER_HPP
