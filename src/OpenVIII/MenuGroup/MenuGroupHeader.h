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

#ifndef VIIIARCHIVE_MENUGROUPHEADER_H
#define VIIIARCHIVE_MENUGROUPHEADER_H
#include "MenuGroupHeaderSection.h"
#include <cstdint>
#include <cstring>
#include <string_view>
#include <array>
namespace OpenVIII::MenuGroup {
struct MenuGroupHeader
{
private:
  static constexpr auto size_{ 2048U };
  static constexpr auto count_{ size_ / sizeof(MenuGroupHeaderSection) };
  std::array<MenuGroupHeaderSection, count_> sections_{};

public:

  constexpr static std::string_view FILENAME = "mngrphd.bin";
  static constexpr auto size() { return count_; }
  [[nodiscard]] const auto &Sections() const { return sections_; }

  MenuGroupHeader() = default;
  template<typename mainT> explicit MenuGroupHeader(const mainT &main)
  {
    auto buffer = main.GetEntryData(FILENAME);
    if (std::size(buffer) < sizeof(std::uint32_t) || size_ != std::size(buffer)) {
      return;
    }
    std::memcpy(sections_.data(), buffer.data(), std::size(buffer));
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_MENUGROUPHEADER_H
