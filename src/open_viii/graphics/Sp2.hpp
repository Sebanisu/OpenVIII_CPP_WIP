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

#ifndef VIIIARCHIVE_SP2_HPP
#define VIIIARCHIVE_SP2_HPP
#include "sp2/Sp2Entry.hpp"
#include "sp2/Sp2Header.hpp"
#include <cstring>
namespace open_viii::graphics {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/Menu_sp2#Layout_of_face.sp2_and_cardanm.sp2:
 */
struct Sp2
{
private:
  std::vector<Sp2Entry> m_entries{};

public:
  Sp2() = default;
  explicit Sp2(std::span<const char> buffer)
  {
    const auto header = Sp2Header{ buffer };
    m_entries.resize(header.size());
    std::span<Sp2Entry> s{ m_entries };
    for (const std::uint32_t offset : header.offsets()) {
      std::memcpy(std::ranges::data(s), std::ranges::data(buffer.subspan(offset)), sizeof(Sp2Entry));
      s = s.subspan(1);
    }
  }
  [[nodiscard]] const auto &at(const size_t i) const
  {
    return m_entries.at(i);
  }
  [[nodiscard]] auto size() const
  {
    return std::ranges::size(m_entries);
  }
  friend std::ostream &operator<<(std::ostream &os, const Sp2 &s)
  {
    os << "{ Entry Count: " << s.size() << " {";
    for (const auto &e : s.m_entries) {
      os << e;
    }
    return os << "}\n";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2_HPP
