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
    std::span<std::vector<sp1Entry>> s{entries_};
    for (const auto & offset : header.Offsets()) {
      auto & current = s.first(1)[0];
      current.resize(offset.Count());
      size_t sz = sizeof(sp1Entry) * offset.Count();
      if(std::ranges::size(buffer)<offset.Offset()+sz)
      {
        entries_={};
        return;
      }
      std::memcpy(std::ranges::data(current), std::ranges::data(buffer.subspan(offset.Offset())),sz);
      s = s.subspan(1);
    }
  }
  [[nodiscard]] const auto &at(const size_t i) const { return entries_.at(i); }
  [[nodiscard]] auto size() const { return std::ranges::size(entries_); }
  friend std::ostream & operator << (std::ostream & os, sp1 s)
  {
    os<< "{ Entry Groups Count: " << s.size() << " {";
    for(const auto & eg : s.entries_)
    {
      if(std::ranges::size(eg)>1) {
        os << " Entry Count: " << std::ranges::size(eg) << ", ";
      }
      for(const auto & e : eg)
      {
        os << e;
      }
    }
    return os <<"}\n";
  }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_SP1_H
