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

#ifndef VIIIARCHIVE_MENUMESSAGES_H
#define VIIIARCHIVE_MENUMESSAGES_H
#include "MenuMessagesSection.h"
#include <cstdint>
#include <cstring>
#include <array>
#include <vector>
namespace open_viii::menu_group {
struct MenuMessages
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_tkmnmes
private:
  static constexpr std::uint16_t defaultSize_{ 16U };
  std::uint16_t size_{ defaultSize_ };
  std::array<std::uint16_t, defaultSize_> sections_{};
  std::array<MenuMessagesSection, defaultSize_> subSections_{};

public:
  [[nodiscard]] auto size() const noexcept { return size_; }
  [[nodiscard]] auto Sections() const noexcept { return sections_; }
  [[nodiscard]] auto SubSections() const noexcept { return subSections_; }
  [[nodiscard]] auto begin() const noexcept { return subSections_.begin(); }
  [[nodiscard]] auto end() const noexcept { return subSections_.end(); }
  template<typename T = std::vector<char>> explicit MenuMessages(const T &buffer)
  {
    auto *ptr = buffer.data();
    std::memcpy(&size_, ptr, sizeof(size_));
    if (size_ != defaultSize_) {
      exit(1);
    }
    ptr += sizeof(size_);
    std::memcpy(sections_.data(), ptr, std::size(sections_) * sizeof(std::uint16_t));
    ptr += sizeof(sections_);
    for (size_t i = 0; i < std::size(sections_); i++) {
      //[Count of Subsections] = [Start of file] + [Section value]
      if (sections_.at(i) == 0U) {
        continue;
      }
      ptr = buffer.data() + sections_.at(i);
      std::uint16_t subSectionCount{};
      std::memcpy(&subSectionCount, ptr, sizeof(subSectionCount));
      std::string_view b{ ptr + sizeof(std::uint16_t), subSectionCount * sizeof(std::uint16_t) };
      subSections_.at(i).SetData(b, subSectionCount);
      //      ptr += sizeof(subSectionCount);
      //      subSections_.at(i).resize(subSectionCount);
      //      std::memcpy(subSections_.at(i).data(), ptr, subSections_.at(i).size() * sizeof(EncodedStringOffset));
      //[Start of string location] = [Start of file] + [Section value] + [Subsection value]
    }
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_MENUMESSAGES_H
