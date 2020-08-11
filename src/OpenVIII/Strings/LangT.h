//
// Created by pcvii on 8/4/2020.
//

#ifndef VIIIARCHIVE_LANGT_H
#define VIIIARCHIVE_LANGT_H
#include <cstdint>
#include <string_view>
#include "OpenVIII/Tools/Tools.h"
namespace OpenVIII {
enum class LangT : std::uint8_t {
  EN,
  FR,
  DE,
  IT,
  JP,
};
struct LangCommon
{
  static constexpr std::string_view en_{ "en" };
  static constexpr std::string_view fr_{ "fr" };
  static constexpr std::string_view de_{ "de" };
  static constexpr std::string_view it_{ "it" };
  static constexpr std::string_view jp_{ "jp" };
  static constexpr std::string_view empty_{ "" };
  template<LangT langVal> [[nodiscard]] static constexpr std::string_view ToString() noexcept
  {
    if constexpr (langVal == LangT::EN) {
      return en_;
    } else if constexpr (langVal == LangT::FR) {
      return fr_;
    } else if constexpr (langVal == LangT::DE) {
      return de_;
    } else if constexpr (langVal == LangT::IT) {
      return it_;
    } else if constexpr (langVal == LangT::JP) {
      return jp_;
    } else {
      return empty_;
    }
  }
  [[nodiscard]] static constexpr LangT FromString(std::string_view strVal) noexcept
  {
    if (strVal.size() == 2) {
      if (OpenVIII::Tools::iEquals(strVal, en_)) {
        return LangT::EN;
      }
      if (OpenVIII::Tools::iEquals(strVal, de_)) {
        return LangT::DE;
      }
      if (OpenVIII::Tools::iEquals(strVal, fr_)) {
        return LangT::FR;
      }
      if (OpenVIII::Tools::iEquals(strVal, it_)) {
        return LangT::IT;
      }
      if (OpenVIII::Tools::iEquals(strVal, jp_)) {
        return LangT::JP;
      }
    }
    exit(1);
  }
};
}// namespace OpenVIII
#endif// VIIIARCHIVE_LANGT_H
