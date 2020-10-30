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

#ifndef VIIIARCHIVE_SECTIONDATA_HPP
#define VIIIARCHIVE_SECTIONDATA_HPP
#include <string_view>

namespace open_viii {
template<typename spanT> requires(sizeof(spanT) > 0U) struct SectionData
{
private:
  // data
  spanT m_span{};
  // strings
  std::string_view m_text_span{};

public:
  [[maybe_unused]] explicit SectionData(const spanT &span, const std::string_view &text_span = {})
    : m_span{ span }, m_text_span{ text_span }
  {}
  [[nodiscard]] auto begin() const
  {
    return m_span.begin();
  }
  [[nodiscard]] auto end() const
  {
    return m_span.end();
  }
  [[nodiscard]] size_t size() const
  {
    return std::ranges::size(m_span) / sizeof(spanT);
  }
  const auto *operator->() const
  {
    return &m_span;
  }
  [[maybe_unused]] auto &span() const noexcept
  {
    return m_span;
  }
  auto &text_span() const noexcept
  {
    return m_text_span;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_SECTIONDATA_HPP
