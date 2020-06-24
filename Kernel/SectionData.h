//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_SECTIONDATA_H
#define VIIIARCHIVE_SECTIONDATA_H
#include <string_view>
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
  [[nodiscard]] size_t Count() const
  {
    if constexpr (sizeof(spanT) == 0) {
      return 0;
    } else {
      return std::size(span_) / sizeof(spanT);
    }
  }
  auto GetEntry(size_t id) const
  {
    if (id < Count()) {
      auto r = spanT{};
      memcpy(&r, span_.data() + (id * sizeof(spanT)), sizeof(spanT));
      return r;
    }
    return spanT{};
  }
  auto &Span() const noexcept { return span_; }
  auto &TextSpan() const noexcept { return textSpan_; }
  auto &ParamSpan() const noexcept { return paramSpan_; }
};
#endif// VIIIARCHIVE_SECTIONDATA_H
