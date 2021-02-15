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
#ifndef VIIIARCHIVE_SP1HEADER_HPP
#define VIIIARCHIVE_SP1HEADER_HPP
#include "Sp1Offset.hpp"
#include <cstdint>
#include <cstring>
#include <span>
#include <vector>
namespace open_viii::graphics {
struct Sp1Header
{
private:
  std::uint32_t          m_offset_count{};
  std::vector<Sp1Offset> m_offsets{};
public:
  Sp1Header() = default;
  explicit Sp1Header(std::span<const char> buffer)
  {
    {
      size_t sz = sizeof(std::uint32_t);
      if (sz > std::ranges::size(buffer)) {
        return;
      }
      std::memcpy(&m_offset_count, std::ranges::data(buffer), sz);
      buffer = buffer.subspan(sz);
    }
    {
      m_offsets.resize(m_offset_count);
      size_t sz = static_cast<std::size_t>(m_offset_count) * sizeof(Sp1Offset);
      if (sz > std::ranges::size(buffer)) {
        m_offset_count = {};
        m_offsets      = {};
        return;
      }
      std::memcpy(std::ranges::data(m_offsets), std::ranges::data(buffer), sz);
    }
  }
  [[nodiscard]] auto size() const noexcept
  {
    return m_offset_count;
  }
  [[nodiscard]] const auto &offsets() const noexcept
  {
    return m_offsets;
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP1HEADER_HPP
