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
#ifndef VIIIARCHIVE_ENCODEDSTRINGOFFSET_HPP
#define VIIIARCHIVE_ENCODEDSTRINGOFFSET_HPP
#include "FF8String.hpp"
namespace open_viii {
struct EncodedStringOffset
{
private:
  std::uint16_t m_offset{};
  [[nodiscard]] static intmax_t
    find_string_size(
      const std::span<const char> &buffer,
      const intmax_t               offset,
      bool                         skip_first_null)
  {
    if (
      offset < 0
      || static_cast<std::size_t>(offset) > std::ranges::size(buffer)) {
      return -1;
    }
    intmax_t i{};
    for (const char &c : buffer.subspan(static_cast<std::size_t>(offset))) {
      if (skip_first_null && i == 0) {
        continue;
      }
      if (c == '\0') {
        return i;
      }
      i++;
    }
    //    auto first = buffer.begin() + offset;// clang tidy says to add * but
    //    don't do that. msvc doesn't return a pointer. auto last =
    //    buffer.end(); for (intmax_t i = 0; first != last; ++first, i++) {
    //      if (skip_first_null && i == 0) {
    //        continue;
    //      }
    //      if (*first == '\0') {
    //        return i;
    //      }
    //    }
    return static_cast<signed>(std::ranges::size(buffer)) - offset;
  }
  [[nodiscard]] static auto
    get_string_at_offset(
      const std::span<const char> &buffer,
      intmax_t                     offset,
      bool                         skip_first_null)
  {
    using namespace std::literals::string_view_literals;
    if (
      offset >= 0 && !std::empty(buffer)
      && std::ranges::size(buffer) > static_cast<size_t>(offset)) {
      auto size = find_string_size(buffer, offset, skip_first_null);
      if (
        size > 0
        && std::ranges::size(buffer)
             > (static_cast<size_t>(offset) + static_cast<size_t>(size))) {
        // return std::span<const char>(buffer.data() +
        // static_cast<size_t>(offset), static_cast<size_t>(size));
        return buffer.subspan(
          static_cast<size_t>(offset),
          static_cast<size_t>(size));
      }
    }
    return std::span<const char>{};
  }

public:
  constexpr auto
    operator<=>(const EncodedStringOffset &right) const noexcept = default;
  [[nodiscard]] auto
    raw_bytes(
      const std::span<const char> &buffer,
      const intmax_t               offset          = 0,
      bool                         skip_first_null = false) const
  {
    if (m_offset == INT16_MAX) {
      return std::span<const char>{};
    }
    return get_string_at_offset(
      buffer,
      static_cast<intmax_t>(m_offset) + offset,
      skip_first_null);
  }
  template<LangT langVal>
  [[nodiscard]] auto
    decoded_string(
      const std::span<const char> &buffer,
      const intmax_t               offset          = 0,
      bool                         skip_first_null = false) const
  {
    return FF8String<langVal>::decode(
      raw_bytes(buffer, offset, skip_first_null));
  }
  [[nodiscard]] constexpr auto
    offset() const noexcept
  {
    return m_offset;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_ENCODEDSTRINGOFFSET_HPP
