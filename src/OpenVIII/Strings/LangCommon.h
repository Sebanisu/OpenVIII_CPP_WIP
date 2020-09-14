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

#ifndef VIIIARCHIVE_LANGCOMMON_H
#define VIIIARCHIVE_LANGCOMMON_H
#include "LangT.h"
namespace open_viii {
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
  [[maybe_unused]] [[nodiscard]] static constexpr LangT from_string(std::string_view str_val) noexcept
  {
    if (str_val.size() == 2) {
      if (open_viii::Tools::i_equals(str_val, EN)) {
        return LangT::en;
      }
      if (open_viii::Tools::i_equals(str_val, DE)) {
        return LangT::de;
      }
      if (open_viii::Tools::i_equals(str_val, FR)) {
        return LangT::fr;
      }
      if (open_viii::Tools::i_equals(str_val, IT)) {
        return LangT::it;
      }
      if (open_viii::Tools::i_equals(str_val, JP)) {
        return LangT::jp;
      }
    }
    exit(1);
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_LANGCOMMON_H
