//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP2_H
#define VIIIARCHIVE_SP2_H
#include "sp2/sp2Entry.h"
#include "sp2/sp2Header.h"
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
  [[nodiscard]] const auto &at(const size_t i) const { return m_entries.at(i); }
  [[nodiscard]] auto size() const { return std::ranges::size(m_entries); }
  friend std::ostream &operator<<(std::ostream &os, Sp2 s)
  {
    os << "{ Entry Count: " << s.size() << " {";
    for (const auto &e : s.m_entries) { os << e; }
    return os << "}\n";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2_H
