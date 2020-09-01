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
struct sp2
{
private:
  std::vector<sp2Entry> entries_{};

public:
  sp2() = default;
  explicit sp2(std::span<const char> buffer)
  {
    const auto header = sp2Header{ buffer };
    entries_.resize(header.size());
    std::span<sp2Entry> s{ entries_ };
    for (const std::uint32_t offset : header.Offsets()) {
      std::memcpy(std::ranges::data(s), std::ranges::data(buffer.subspan(offset)), sizeof(sp2Entry));
      s = s.subspan(1);
    }
  }
  [[nodiscard]] const auto &at(const size_t i) const { return entries_.at(i); }
  [[nodiscard]] auto size() const { return std::ranges::size(entries_); }
  friend std::ostream &operator<<(std::ostream &os, sp2 s)
  {
    os << "{ Entry Count: " << s.size() << " {";
    for (const auto &e : s.entries_) { os << e; }
    return os << "}\n";
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_SP2_H
