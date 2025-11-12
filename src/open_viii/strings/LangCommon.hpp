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
#ifndef VIIIARCHIVE_LANGCOMMON_HPP
#define VIIIARCHIVE_LANGCOMMON_HPP
#include "LangT.hpp"
#include <array>
#include <ranges>
#include <utility>
namespace open_viii {
/**
 * Lang Common for conversion from enum to 2 letter language code
 */
namespace LangCommon {
  constexpr void
    for_each_langT(auto &&lambda)
  {
    using u = std::underlying_type_t<LangT>;
    for (auto i = static_cast<u>(LangT::begin); i != static_cast<u>(LangT::end);
         ++i) {
      lambda(static_cast<LangT>(i));
    }
  }
  [[nodiscard]] consteval auto
    to_array()
  {
    using u = std::underlying_type_t<LangT>;
    std::array<LangT, static_cast<u>(LangT::end)> arr = {};
    for_each_langT([i = arr.begin()](auto coo) mutable {
      *i = coo;
      ++i;
    });
    return arr;
  }
  static constexpr std::string_view EN{ "en" };
  static constexpr std::string_view FR{ "fr" };
  static constexpr std::string_view DE{ "de" };
  static constexpr std::string_view IT{ "it" };
  static constexpr std::string_view JP{ "jp" };
  static constexpr std::string_view ES{ "es" };
  static constexpr std::string_view ENFULL{ "English" };
  static constexpr std::string_view FRFULL{ "French" };
  static constexpr std::string_view DEFULL{ "German" };
  static constexpr std::string_view ITFULL{ "Italian" };
  static constexpr std::string_view JPFULL{ "Japanese" };
  static constexpr std::string_view ESFULL{ "Spanish" };
  static constexpr std::string_view ENG{ "eng" };
  static constexpr std::string_view FRE{ "fre" };
  static constexpr std::string_view GER{ "ger" };
  static constexpr std::string_view ITA{ "ita" };
  static constexpr std::string_view SPA{ "spa" };
  static constexpr std::string_view MISC{ "x" };
  static constexpr std::string_view MISCFULL{ "Generic" };
  static constexpr std::string_view EMPTY{ "" };
  /**
   * from langT to string
   * @tparam langVal
   * @return
   */
  [[nodiscard]] constexpr const std::string_view
    to_string(LangT langVal)
  {
    if (langVal == LangT::en) {
      return EN;
    }
    if (langVal == LangT::fr) {
      return FR;
    }
    if (langVal == LangT::de) {
      return DE;
    }
    if (langVal == LangT::it) {
      return IT;
    }
    if (langVal == LangT::es) {
      return ES;
    }
    if (langVal == LangT::jp) {
      return JP;
    }
    return EMPTY;
  }

  /**
   * from langT to string
   * @tparam langVal
   * @return
   */
  [[nodiscard]] constexpr const std::string_view
    to_string_full(LangT langVal)
  {
    if (langVal == LangT::en) {
      return ENFULL;
    }
    if (langVal == LangT::fr) {
      return FRFULL;
    }
    if (langVal == LangT::de) {
      return DEFULL;
    }
    if (langVal == LangT::it) {
      return ITFULL;
    }
    if (langVal == LangT::es) {
      return ESFULL;
    }
    if (langVal == LangT::jp) {
      return JPFULL;
    }
    return EMPTY;
  }
  /**
   * from langT to string
   * @tparam langVal
   * @return
   */
  [[nodiscard]] constexpr const std::string_view
    to_string_3_char(LangT langVal)
  {
    if (langVal == LangT::en) {
      return ENG;
    }
    if (langVal == LangT::fr) {
      return FRE;
    }
    if (langVal == LangT::de) {
      return GER;
    }
    if (langVal == LangT::it) {
      return ITA;
    }
    if (langVal == LangT::es) {
      return SPA;
    }
    if (langVal == LangT::jp) {
      return JP;
    }
    return EMPTY;
  }
  /**
   * Get an array of string views
   */
  [[nodiscard]] consteval auto
    to_string_array()
  {
    auto                                          coos = to_array();
    std::array<std::string_view, std::size(coos)> coos_c_str{};
    std::ranges::transform(coos, coos_c_str.begin(), [](const auto &coo) {
      return to_string(coo);// I hope these are null terminated.
    });
    coos_c_str.back() = MISC;
    return coos_c_str;
  }

  /**
   * Get an array of string views
   */
  [[nodiscard]] consteval auto
    to_string_array_3_char()
  {
    constexpr auto                                    coos = to_array();
    std::array<std::string_view, std::size(coos) + 1> coos_c_str{};
    std::ranges::transform(coos, coos_c_str.begin(), [](const auto &coo) {
      return to_string_3_char(coo);// I hope these are null terminated.
    });
    coos_c_str.back() = MISC;
    return coos_c_str;
  }

  /**
   * Get an array of string views
   */
  [[nodiscard]] consteval auto
    to_string_array_full()
  {
    constexpr auto                                    coos = to_array();
    std::array<std::string_view, std::size(coos) + 1> coos_c_str{};
    std::ranges::transform(coos, coos_c_str.begin(), [](const auto &coo) {
      return to_string_full(coo);// I hope these are null terminated.
    });
    coos_c_str.back() = MISCFULL;
    return coos_c_str;
  }

  [[nodiscard]] constexpr LangT
    from_string_3_char(std::string_view input)
  {
    constexpr auto coos        = to_array();
    constexpr auto coos_3_char = to_string_array_3_char();
    if (const auto it = std::ranges::find_if(
          coos_3_char,
          [&](const auto &coo) -> bool {
            return open_viii::tools::i_equals(coo, input);
          });
        it != coos_3_char.end()) {
      const auto dist = std::ranges::distance(coos_3_char.begin(), it);
      if (std::cmp_less(dist, std::ranges::size(coos))) {
        auto out = coos.begin();
        std::ranges::advance(out, dist);
        return *out;
      }
    }
    return LangT::generic;
  }

  /**
   * Get an array of const char *
   */
  [[nodiscard]] consteval auto
    to_c_str_array()
  {
    auto                                      coos = to_array();
    std::array<const char *, std::size(coos)> coos_c_str{};
    std::ranges::transform(coos, coos_c_str.begin(), [](const auto &coo) {
      return to_string(coo).data();// I hope these are null terminated.
    });
    return coos_c_str;
  }
  /**
   * from langT to string
   * @tparam langVal
   * @return
   */
  template<LangT langVal>
  [[nodiscard]] static consteval std::string_view
    to_string() noexcept
  {
    if constexpr (langVal == LangT::en) {
      return EN;
    }
    else if constexpr (langVal == LangT::fr) {
      return FR;
    }
    else if constexpr (langVal == LangT::de) {
      return DE;
    }
    else if constexpr (langVal == LangT::it) {
      return IT;
    }
    else if constexpr (langVal == LangT::es) {
      return ES;
    }
    else if constexpr (langVal == LangT::jp) {
      return JP;
    }
    else {
      return EMPTY;
    }
  }
  /**
   * from langT to string
   * @tparam langVal
   * @return
   */
  template<LangT langVal>
  [[nodiscard]] static consteval std::string_view
    to_string_full() noexcept
  {
    if constexpr (langVal == LangT::en) {
      return ENFULL;
    }
    else if constexpr (langVal == LangT::fr) {
      return FRFULL;
    }
    else if constexpr (langVal == LangT::de) {
      return DEFULL;
    }
    else if constexpr (langVal == LangT::it) {
      return ITFULL;
    }
    else if constexpr (langVal == LangT::es) {
      return ESFULL;
    }
    else if constexpr (langVal == LangT::jp) {
      return JPFULL;
    }
    else {
      return EMPTY;
    }
  }
  /**
   * from string to langT
   * @param str_val
   * @return
   */
  [[nodiscard]] constexpr LangT
    from_string(std::string_view str_val) noexcept
  {
    if (str_val.size() == 2) {
      if (open_viii::tools::i_equals(str_val, EN)) {
        return LangT::en;
      }
      if (open_viii::tools::i_equals(str_val, DE)) {
        return LangT::de;
      }
      if (open_viii::tools::i_equals(str_val, FR)) {
        return LangT::fr;
      }
      if (open_viii::tools::i_equals(str_val, IT)) {
        return LangT::it;
      }
      if (open_viii::tools::i_equals(str_val, ES)) {
        return LangT::es;
      }
      if (open_viii::tools::i_equals(str_val, JP)) {
        return LangT::jp;
      }
    }
    return LangT::end;
  }
}// namespace LangCommon
static_assert(!LangCommon::to_array().empty());
}// namespace open_viii
#endif// VIIIARCHIVE_LANGCOMMON_HPP
