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
  static consteval auto
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
  static constexpr std::string_view EMPTY{};
  /**
   * from langT to string
   * @tparam langVal
   * @return
   */
  constexpr std::string_view
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
  template<LangT langVal>
  [[maybe_unused]] [[nodiscard]] static consteval std::string_view
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
   * from string to langT
   * @param str_val
   * @return
   */
  [[maybe_unused]] [[nodiscard]] constexpr LangT
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
