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

#ifndef VIIIARCHIVE_MENUMESSAGESSECTION_HPP
#define VIIIARCHIVE_MENUMESSAGESSECTION_HPP

#include "OpenVIII/Strings/EncodedStringOffset.hpp"
namespace open_viii::menu_group {
struct MenuMessagesSection
{
private:
  std::vector<EncodedStringOffset> m_data{};

  [[nodiscard]] auto *data() { return m_data.data(); }

public:
  MenuMessagesSection() = default;
  //  explicit MenuMessagesSection(const size_t & count)
  //  {
  //    data_.resize(count);
  //  }
  void resize(const size_t count) { m_data.resize(count); }
  [[nodiscard]] auto begin() const { return m_data.begin(); }
  [[nodiscard]] auto end() const { return m_data.end(); }
  [[nodiscard]] auto size() const { return m_data.size(); }
  //  [[nodiscard]] auto & at(const size_t & index)
  //  {
  //    return data_.at(index);
  //  }
  template<typename T = std::vector<char>> void set_data(const T &buffer, const size_t &count)
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
  template<typename T = std::vector<char>> explicit MenuMessagesSection(const T &buffer)
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
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_MENUMESSAGESSECTION_HPP
