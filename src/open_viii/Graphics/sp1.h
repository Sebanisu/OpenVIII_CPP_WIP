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

#ifndef VIIIARCHIVE_SP1_H
#define VIIIARCHIVE_SP1_H
#include "OpenVIII/Graphics/sp1/sp1Header.h"
#include "OpenVIII/Graphics/sp1/sp1Entry.h"
namespace open_viii::graphics {
/**
 * SP1 file parsing. This file contains the coords for for the icons. These might need scaled up because the values are
 * made for the original size image. 256x256
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/Menu_sp2
 */
struct Sp1
{
private:
  std::vector<std::vector<Sp1Entry>> m_entries{};

public:
  Sp1() = default;
  explicit Sp1(std::span<const char> buffer)
  {
    const auto header = Sp1Header{ buffer };
    m_entries.resize(header.size());
    std::span<std::vector<Sp1Entry>> s{ m_entries };
    for (const auto &offset : header.offsets()) {
      auto &current = s.first(1)[0];
      current.resize(offset.count());
      size_t sz = sizeof(Sp1Entry) * offset.count();
      if (std::ranges::size(buffer) < offset.offset() + sz) {
        m_entries = {};
        return;
      }
      std::memcpy(std::ranges::data(current), std::ranges::data(buffer.subspan(offset.offset())), sz);
      s = s.subspan(1);
    }
  }
  [[nodiscard]] const auto &at(const size_t i) const { return m_entries.at(i); }
  [[nodiscard]] auto size() const { return std::ranges::size(m_entries); }
  friend std::ostream &operator<<(std::ostream &os, const Sp1 &s)
  {
    os << "{ Entry Groups Count: " << s.size() << " {";
    for (const auto &eg : s.m_entries) {
      if (std::ranges::size(eg) > 1) {
        os << " Entry Count: " << std::ranges::size(eg) << ", ";
      }
      for (const auto &e : eg) { os << e; }
    }
    return os << "}\n";
  }
};

}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP1_H
