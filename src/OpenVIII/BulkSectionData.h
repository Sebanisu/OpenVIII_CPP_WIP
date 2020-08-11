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

#ifndef VIIIARCHIVE_BULKSECTIONDATA_H
#define VIIIARCHIVE_BULKSECTIONDATA_H
#include <string_view>

namespace OpenVIII {
template<typename spanT, size_t max = 0U> struct BulkSectionData
{
private:
  // data
  std::string_view span_{};
  // strings
  std::string_view textSpan_{};

public:
  [[maybe_unused]] explicit BulkSectionData(const std::string_view &span, const std::string_view &textSpan = {})
    : span_{ span }, textSpan_{ textSpan }
  {}
  [[nodiscard]] size_t size() const
  {
    if constexpr (sizeof(spanT) == 0) {
      return 0;
    } else {
      size_t calcSize = std::size(span_) / sizeof(spanT);
      if (max == 0U || max > calcSize) {
        return calcSize;
      }
      return max;
    }
  }
  auto at(size_t id) const
  {
    if (id < size()) {
      return operator[](id);
    }
    using namespace std::string_literals;
    throw std::out_of_range(
      "BulkSectionData index out of range: "s + std::to_string(id) + "//"s + std::to_string(size()));
    // return spanT{};
  }
  auto operator[](size_t id) const noexcept
  {
    auto r = spanT{};
    memcpy(&r, span_.data() + (id * sizeof(spanT)), sizeof(spanT));
    return r;
  }
  //  auto &begin() const
  //  {
  //    return at(0);
  //  }
  //  auto &end() const
  //  {
  //    return at(size()-1);
  //  }
  [[maybe_unused]] auto &Span() const noexcept { return span_; }
  auto &TextSpan() const noexcept { return textSpan_; }
};
}// namespace OpenVIII
#endif// VIIIARCHIVE_BULKSECTIONDATA_H
