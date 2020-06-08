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
#include <vector>
#include <string_view>
#include <cstring>
#ifndef VIIIARCHIVE_KERNEL_H
#define VIIIARCHIVE_KERNEL_H
// will be moving kernel into it's own project once it's more fully baked.
struct Kernel
{
  //https://github.com/alexfilth/doomtrain/wiki/Header
private:
  std::vector<char> buffer_{};
  std::vector<std::uint32_t> sectionOffsets_{};

public:
  template<typename mainT> explicit Kernel(const mainT &main)
  {
    buffer_ = main.GetEntryData(std::string_view("Kernel.bin"));
    if (std::size(buffer_) < sizeof(std::uint32_t)) {
      return;
    }
    std::uint32_t sectionCount{};
    std::memcpy(&sectionCount, buffer_.data(), sizeof(sectionCount));
    if (std::size(buffer_) < sizeof(std::uint32_t) * (sectionCount + 1)) {
      return;
    }
    sectionOffsets_.reserve(sectionCount);
    size_t i{1};
    while (sectionCount-- > 0) { std::memcpy(&sectionOffsets_.emplace_back(), buffer_.data() + (sizeof(sectionCount)*(i++)), sizeof(sectionCount)); }
  }
  [[nodiscard]] const auto &Buffer() const noexcept { return buffer_; }
  [[nodiscard]] auto SectionCount() const noexcept { return sectionOffsets_.size(); }
  [[nodiscard]] const auto &SectionOffsets() const noexcept { return sectionOffsets_; }
};
#endif// VIIIARCHIVE_KERNEL_H
