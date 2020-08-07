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

#ifndef VIIIARCHIVE_SECTIONDATA_H
#define VIIIARCHIVE_SECTIONDATA_H
#include <string_view>

namespace OpenVIII {
template<typename spanT> struct SectionData
{
private:
  // data
  std::string_view span_{};
  // strings
  std::string_view textSpan_{};
  // extra data
  std::string_view paramSpan_{};

public:
  [[maybe_unused]] explicit SectionData(const std::string_view &span,
    const std::string_view &textSpan = {},
    const std::string_view &paramSpan = {})
    : span_{ span }, textSpan_{ textSpan }, paramSpan_{ paramSpan }
  {}
  [[nodiscard]] size_t size() const
  {
    if constexpr (sizeof(spanT) == 0) {
      return 0;
    } else {
      return std::size(span_) / sizeof(spanT);
    }
  }
  auto at(size_t id) const
  {
    if (id < size()) {
      auto r = spanT{};
      memcpy(&r, span_.data() + (id * sizeof(spanT)), sizeof(spanT));
      return r;
    }
    return spanT{};
  }
  auto operator[](size_t id) const
  {
    return at(id);
  }
//  auto &begin() const
//  {
//    return at(0);
//  }
//  auto &end() const
//  {
//    return at(size()-1);
//  }
  auto &Span() const noexcept { return span_; }
  auto &TextSpan() const noexcept { return textSpan_; }
  auto &ParamSpan() const noexcept { return paramSpan_; }
};
}// namespace OpenVIII
#endif// VIIIARCHIVE_SECTIONDATA_H
