//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_SP1_H
#define VIIIARCHIVE_SP1_H
#include "OpenVIII/Graphics/sp1/sp1Header.h"
#include "OpenVIII/Graphics/sp1/sp1Entry.h"
namespace OpenVIII::Graphics {
/**
 * SP1 file parsing. This file contains the coords for for the icons. These might need scaled up because the values are
 * made for the original size image. 256x256
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/Menu_sp2
 */
struct sp1
{
private:
  std::vector<std::vector<sp1Entry>> entries_{};

public:
  sp1() = default;
  explicit sp1(std::span<const char> buffer)
  {
    const auto header = sp1Header{ buffer };
    entries_.resize(header.size());
    std::size_t i{};
    for (auto entry : entries_) {
      const auto entryOffset = header.Offsets().at(i++);
      entry.resize(entryOffset.Count());
      size_t entrySize = sizeof(sp1Entry) * entryOffset.Count();
      const auto entryBuffer = buffer.subspan(entryOffset.Offset(), entrySize);
      std::memcpy(std::ranges::data(entry), std::ranges::data(entryBuffer), entrySize);
    }
  }
  [[nodiscard]] const auto &at(const size_t i) const { return entries_.at(i); }
  [[nodiscard]] auto &size() const { return entries_size(); }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1_H
