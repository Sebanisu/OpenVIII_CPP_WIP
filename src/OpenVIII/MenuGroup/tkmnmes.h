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

#ifndef VIIIARCHIVE_TKMNMES_H
#define VIIIARCHIVE_TKMNMES_H
#include <cstdint>
#include <cstring>
#include <array>
#include <vector>
#include "OpenVIII/Strings/EncodedStringOffset.h"
namespace OpenVIII::MenuGroup {
struct tkmnmes_subSection
{
private:
  std::vector<EncodedStringOffset> data_{};

public:
  tkmnmes_subSection() = default;
  //  explicit tkmnmes_subSection(const size_t & count)
  //  {
  //    data_.resize(count);
  //  }
  void resize(const size_t count) { data_.resize(count); }
  [[nodiscard]] auto begin() const { return data_.begin(); }
  [[nodiscard]] auto end() const { return data_.end(); }
  [[nodiscard]] auto *data() { return data_.data(); }
  [[nodiscard]] auto size() const { return data_.size(); }
  template<typename T = std::vector<char>> void SetData(const T &buffer, const size_t &count)
  {
    //[Count of Subsections] = [Start of file] + [Section value]
    auto totalBytes = count * sizeof(std::uint16_t);
    if (totalBytes > buffer.size()) {
      totalBytes = buffer.size();
    }
    resize(count);
    std::memcpy(data(), buffer.data(), totalBytes);
    //[Start of string location] = [Start of file] + [Section value] + [Subsection value]
  }
  template<typename T = std::vector<char>> explicit tkmnmes_subSection(const T &buffer)
  {

    //[Count of Subsections] = [Start of file] + [Section value]
    std::uint16_t subSectionCount{};
    auto *ptr = buffer.data();
    std::memcpy(&subSectionCount, ptr, sizeof(std::uint16_t));
    ptr += sizeof(std::uint16_t);
    auto totalBytes = subSectionCount * sizeof(std::uint16_t);
    if (totalBytes > buffer.size() - sizeof(std::uint16_t)) {
      totalBytes = buffer.size() - sizeof(std::uint16_t);
      exit(1);
    }
    resize(totalBytes);
    std::memcpy(data(), ptr, totalBytes);
    //[Start of string location] = [Start of file] + [Section value] + [Subsection value]
  }
};
struct tkmnmes
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_tkmnmes
private:
  static constexpr std::uint16_t defaultSectionCount_{ 16U };
  std::uint16_t sectionCount_{ defaultSectionCount_ };
  std::array<std::uint16_t, defaultSectionCount_> sections_{};
  std::array<tkmnmes_subSection, defaultSectionCount_> subSections_{};

public:
  [[nodiscard]] auto SectionCount() const noexcept { return sectionCount_; }
  [[nodiscard]] auto Sections() const noexcept { return sections_; }
  [[nodiscard]] auto SubSections() const noexcept { return subSections_; }
  template<typename T = std::vector<char>> explicit tkmnmes(const T &buffer)
  {
    auto *ptr = buffer.data();
    std::memcpy(&sectionCount_, ptr, sizeof(sectionCount_));
    if (sectionCount_ != defaultSectionCount_) {
      exit(1);
    }
    ptr += sizeof(sectionCount_);
    std::memcpy(sections_.data(), ptr, std::size(sections_) * sizeof(std::uint16_t));
    ptr += sizeof(sections_);
    for (size_t i = 0; i < std::size(sections_); i++) {
      //[Count of Subsections] = [Start of file] + [Section value]
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
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_TKMNMES_H
