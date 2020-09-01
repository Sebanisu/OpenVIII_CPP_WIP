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

#ifndef VIIIARCHIVE_ENCODEDSTRINGOFFSET_H
#define VIIIARCHIVE_ENCODEDSTRINGOFFSET_H
#include <cstdint>
#include <string_view>
#include "FF8String.h"

namespace open_viii {
struct EncodedStringOffset
{
private:
  std::uint16_t offset_{};

  [[nodiscard]] static intmax_t
    FindStringSize(const std::string_view &buffer, const intmax_t offset, bool skipFirstNull)
  {
    if (static_cast<unsigned>(offset) > std::size(buffer)) {
      return -1;
    }
    auto first = buffer.begin() + offset;// clang tidy says to add * but don't do that. msvc doesn't return a pointer.
    auto last = buffer.end();
    for (intmax_t i = 0; first != last; ++first, i++) {
      if (skipFirstNull && i == 0) {
        continue;
      }
      if (*first == '\0') {
        return i;
      }
    }
    return static_cast<signed>(std::size(buffer)) - offset;
  }
  [[nodiscard]] static auto GetStringAtOffset(const std::string_view &buffer, intmax_t offset, bool skipFirstNull)
  {
    using namespace std::literals::string_view_literals;

    if (offset >= 0 && !std::empty(buffer) && std::size(buffer) > static_cast<size_t>(offset)) {
      auto size = FindStringSize(buffer, offset, skipFirstNull);
      if (size > 0 && std::size(buffer) > (static_cast<size_t>(offset) + static_cast<size_t>(size))) {
        return std::string_view(buffer.data() + static_cast<size_t>(offset), static_cast<size_t>(size));
      }
    }
    return ""sv;
  }

public:
  [[nodiscard]] auto
    RawBytes(const std::string_view &buffer, const intmax_t offset = 0, bool skipFirstNull = false) const
  {
    if (offset_ == INT16_MAX) {
      return ""sv;
    }
    return GetStringAtOffset(buffer, static_cast<intmax_t>(offset_) + offset, skipFirstNull);
  }
  template<LangT langVal>
  [[nodiscard]] auto
    DecodedString(const std::string_view &buffer, const intmax_t offset = 0, bool skipFirstNull = false) const
  {
    return FF8String<langVal>::Decode(RawBytes(buffer, offset, skipFirstNull));
  }

  [[nodiscard]] auto Offset() const noexcept { return offset_; }
};
}// namespace open_viii
#endif// VIIIARCHIVE_ENCODEDSTRINGOFFSET_H
