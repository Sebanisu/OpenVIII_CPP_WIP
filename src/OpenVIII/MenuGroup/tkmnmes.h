//
// Created by pcvii on 7/1/2020.
//

#ifndef VIIIARCHIVE_TKMNMES_H
#define VIIIARCHIVE_TKMNMES_H
#include <cstdint>
#include <cstring>
#include <array>
#include <vector>
#include "OpenVIII/Strings/EncodedStringOffset.h"
namespace OpenVIII::MenuGroup {
struct tkmnmes
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_tkmnmes
private:
  static constexpr std::uint16_t defaultSectionCount_{ 16U };
  std::uint16_t sectionCount_{ defaultSectionCount_ };
  std::array<std::uint16_t, defaultSectionCount_> sections_{};
  std::array<std::vector<EncodedStringOffset>, defaultSectionCount_> subSections_{};

public:
  [[nodiscard]] auto SectionCount() const noexcept { return sectionCount_; }
  [[nodiscard]] auto Sections() const noexcept { return sections_; }
  [[nodiscard]] auto SubSections() const noexcept { return subSections_; }
  template<typename T = std::vector<char>>
  explicit tkmnmes(const T &buffer)
  {
    auto *ptr = buffer.data();
    std::memcpy(&sectionCount_, ptr, sizeof(sectionCount_));
    if (sectionCount_ != defaultSectionCount_) {
      exit(1);
    }
    ptr += sizeof(sectionCount_);
    std::memcpy(sections_.data(), ptr, sizeof(sections_));
    ptr += sizeof(sections_);
    for (size_t i = 0; i < std::size(sections_); i++) {
      //[Count of Subsections] = [Start of file] + [Section value]
      ptr = buffer.data() + sections_.at(i);
      std::uint16_t subSectionCount{};
      std::memcpy(&subSectionCount, ptr, sizeof(subSectionCount));
      ptr += sizeof(subSectionCount);
      subSections_.at(i).resize(subSectionCount);
      std::memcpy(subSections_.at(i).data(), ptr, subSections_.at(i).size() * sizeof(EncodedStringOffset));
      //[Start of string location] = [Start of file] + [Section value] + [Subsection value]
    }
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_TKMNMES_H
