//
// Created by pcvii on 8/4/2020.
//

#ifndef VIIIARCHIVE_LANGT_H
#define VIIIARCHIVE_LANGT_H
#include <cstdint>
#include <string_view>
#include "OpenVIII/Tools/Tools.h"
namespace open_viii {
enum class LangT : std::uint8_t {
  en,
  fr,
  de,
  it,
  jp,
};
struct LangCommon
{
public:
  static constexpr std::string_view EN{ "en" };
  static constexpr std::string_view FR{ "fr" };
  static constexpr std::string_view DE{ "de" };
  static constexpr std::string_view IT{ "it" };
  static constexpr std::string_view JP{ "jp" };
  static constexpr std::string_view EMPTY{ "" };
  template<LangT langVal> [[maybe_unused]] [[nodiscard]] static constexpr std::string_view to_string() noexcept
  {
    if constexpr (langVal == LangT::en) {
      return EN;
    } else if constexpr (langVal == LangT::fr) {
      return FR;
    } else if constexpr (langVal == LangT::de) {
      return DE;
    } else if constexpr (langVal == LangT::it) {
      return IT;
    } else if constexpr (langVal == LangT::jp) {
      return JP;
    } else {
      return EMPTY;
    }
  }
  [[maybe_unused]] [[nodiscard]] static constexpr LangT from_string(std::string_view strVal) noexcept
  {
    if (strVal.size() == 2) {
      if (open_viii::Tools::i_equals(strVal, EN)) {
        return LangT::en;
      }
      if (open_viii::Tools::i_equals(strVal, DE)) {
        return LangT::de;
      }
      if (open_viii::Tools::i_equals(strVal, FR)) {
        return LangT::fr;
      }
      if (open_viii::Tools::i_equals(strVal, IT)) {
        return LangT::it;
      }
      if (open_viii::Tools::i_equals(strVal, JP)) {
        return LangT::jp;
      }
    }
    exit(1);
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_LANGT_H
