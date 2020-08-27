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
#include "OpenVIII/ItemID.h"
#include "OpenVIII/Kernel/BattleItems.h"
#include "OpenVIII/Kernel/NonBattleItems.h"

namespace OpenVIII {
template<typename spanT, size_t max = 0U> requires(sizeof(spanT) > 0U) struct BulkSectionData
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
    const auto calcSize = [this]() { return std::size(span_) / sizeof(spanT); };
    if constexpr (max == 0U) {
      return calcSize();
    } else {
      const auto c = calcSize();
      if (max > c) {
        return c;
      }
      return max;
    }
  }

  template<typename T = std::size_t> requires(std::integral<T> && !std::signed_integral<T>) auto at(const T id_v) const
  {
    auto id = static_cast<size_t>(id_v);
    if constexpr (std::is_same_v<T, ItemID>) {
      if constexpr (std::is_same_v<spanT, Kernel::BattleItems>) {
      } else if constexpr (std::is_same_v<spanT, Kernel::NonBattleItems>) {
        static constexpr auto battleItemsCount = 33U;
        id -= battleItemsCount;
      } else {
        throw(std::invalid_argument{ "ItemID used in wrong place!" });
      }
    }

    if (id > size()) {
      using namespace std::string_literals;
      throw std::out_of_range(
        "BulkSectionData index out of range: "s + std::to_string(id) + "//"s + std::to_string(size()));
      // return spanT{};
    }
    return operator[](id);
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
  [[maybe_unused]] auto &TextSpan() const noexcept { return textSpan_; }
};
}// namespace OpenVIII
#endif// VIIIARCHIVE_BULKSECTIONDATA_H