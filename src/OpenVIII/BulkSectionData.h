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
#include <iterator>
#include "OpenVIII/ItemIdT.h"
#include "OpenVIII/Kernel/BattleItems.h"
#include "OpenVIII/Kernel/NonBattleItems.h"

namespace open_viii {

template<typename spanT, size_t max = 0U> requires(sizeof(spanT) > 0U) struct BulkSectionData
{
private:
  // data
  std::span<const char> m_span{};
  // strings
  std::span<const char> m_text_span{};

public:
  [[maybe_unused]] explicit BulkSectionData(const std::span<const char> &span, const std::span<const char> &text_span = {})
    : m_span{ span }, m_text_span{ text_span }
  {}
  [[nodiscard]] size_t size() const
  {
    const auto calcSize = [this]() { return std::ranges::size(m_span) / sizeof(spanT); };
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
    if constexpr (std::is_same_v<T, ItemIdT>) {
      if constexpr (std::is_same_v<spanT, kernel::BattleItems>) {
      } else if constexpr (std::is_same_v<spanT, kernel::NonBattleItems>) {
        static constexpr auto battle_items_count = 33U;
        id -= battle_items_count;
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
    memcpy(&r, m_span.data() + (id * sizeof(spanT)), sizeof(spanT));
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
  [[maybe_unused]] auto &span() const noexcept { return m_span; }
  [[maybe_unused]] auto &text_span() const noexcept { return m_text_span; }
};
template<typename spanT, size_t max = 0U> requires(sizeof(spanT) > 0U) struct BulkSectionDataIterator : public std::iterator<std::input_iterator_tag,BulkSectionData<spanT,max>>
{

};
}// namespace open_viii
#endif// VIIIARCHIVE_BULKSECTIONDATA_H