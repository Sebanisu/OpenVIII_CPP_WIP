//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_ENCODEDSTRINGOFFSET_H
#define VIIIARCHIVE_ENCODEDSTRINGOFFSET_H
#include <cstdint>
#include <string_view>
#include "FF8String.h"

namespace OpenVIII {
struct EncodedStringOffset
{
private:
  std::uint16_t offset_{};

  [[nodiscard]] static intmax_t FindStringSize(const std::string_view &buffer, const intmax_t offset)
  {
    if (static_cast<unsigned>(offset) > std::size(buffer)) {
      return -1;
    }
    auto first = buffer.begin() + offset;// clang tidy says to add * but don't do that. msvc doesn't return a pointer.
    auto last = buffer.end();
    for (intmax_t i = 0; first != last; ++first, i++) {
      if (*first == '\0') {
        return i;
      }
    }
    return static_cast<signed>(std::size(buffer)) - offset;
  }
  [[nodiscard]] static auto GetStringAtOffset(const std::string_view &buffer, uint16_t offset)
  {
    using namespace std::literals::string_view_literals;
    if (offset != INT16_MAX && !std::empty(buffer) && std::size(buffer) > offset) {
      auto size = FindStringSize(buffer, static_cast<intmax_t>(offset));
      if (size >= 0) {
        return std::string_view(buffer.data() + offset, static_cast<size_t>(size));
      }
    }
    return ""sv;
  }

public:
  [[nodiscard]] auto RawBytes(const std::string_view &buffer) const { return GetStringAtOffset(buffer, offset_); }
  [[nodiscard]] auto DecodedString(const std::string_view &buffer) const { return FF8String::Decode(RawBytes(buffer)); }
};
}// namespace OpenVIII
#endif// VIIIARCHIVE_ENCODEDSTRINGOFFSET_H
