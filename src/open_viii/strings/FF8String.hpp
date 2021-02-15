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
#ifndef VIIIARCHIVE_FF8STRING_HPP
#define VIIIARCHIVE_FF8STRING_HPP
#include "LangT.hpp"
#include <array>
#include <sstream>
#include <string_view>
using namespace std::string_view_literals;
namespace open_viii {
template<LangT langVal> struct FF8String
{
private:
  static constexpr std::array<std::u8string_view, 256> EU_CODE_PAGE{
    u8"\x00"sv,   u8"\x01"sv,   u8"\n"sv,     u8"\x03"sv,   u8"\x04"sv,
    u8"\x05"sv,   u8"\x06"sv,   u8"\x07"sv,   u8"\x08"sv,   u8"\x09"sv,
    u8"\x0A"sv,   u8"\x0B"sv,   u8"\x0C"sv,   u8"\x0D"sv,   u8"\x0E"sv,
    u8"\x0F"sv,   u8"\x10"sv,   u8"\x11"sv,   u8"\x12"sv,   u8"\x13"sv,
    u8"\x14"sv,   u8"\x15"sv,   u8"\x16"sv,   u8"\x17"sv,   u8"\x18"sv,
    u8"\x19"sv,   u8"\x1A"sv,   u8"\x1B"sv,   u8"\x1C"sv,   u8"\x1D"sv,
    u8"\x1E"sv,   u8"\x1F"sv,   u8" "sv,      u8"0"sv,      u8"1"sv,
    u8"2"sv,      u8"3"sv,      u8"4"sv,      u8"5"sv,      u8"6"sv,
    u8"7"sv,      u8"8"sv,      u8"9"sv,      u8"%"sv,      u8"/"sv,
    u8":"sv,      u8"!"sv,      u8"?"sv,      u8"…"sv,      u8"+"sv,
    u8"-"sv,      u8"="sv,      u8"*"sv,      u8"&"sv,      u8"\u300C"sv,
    u8"\u300D"sv, u8"("sv,      u8")"sv,      u8"·"sv,      u8"."sv,
    u8"sv,"sv,    u8"~"sv,      u8"“"sv,      u8"”"sv,      u8"‘"sv,
    u8"#"sv,      u8"$"sv,      u8R"(")"sv,   u8"_"sv,      u8"A"sv,
    u8"B"sv,      u8"C"sv,      u8"D"sv,      u8"E"sv,      u8"F"sv,
    u8"G"sv,      u8"H"sv,      u8"I"sv,      u8"J"sv,      u8"K"sv,
    u8"L"sv,      u8"M"sv,      u8"N"sv,      u8"O"sv,      u8"P"sv,
    u8"Q"sv,      u8"R"sv,      u8"S"sv,      u8"T"sv,      u8"U"sv,
    u8"V"sv,      u8"W"sv,      u8"X"sv,      u8"Y"sv,      u8"Z"sv,
    u8"a"sv,      u8"b"sv,      u8"c"sv,      u8"d"sv,      u8"e"sv,
    u8"f"sv,      u8"g"sv,      u8"h"sv,      u8"i"sv,      u8"j"sv,
    u8"k"sv,      u8"l"sv,      u8"m"sv,      u8"n"sv,      u8"o"sv,
    u8"p"sv,      u8"q"sv,      u8"r"sv,      u8"s"sv,      u8"t"sv,
    u8"u"sv,      u8"v"sv,      u8"w"sv,      u8"x"sv,      u8"y"sv,
    u8"z"sv,      u8"À"sv,      u8"Á"sv,      u8"Â"sv,      u8"Ä"sv,
    u8"Ç"sv,      u8"È"sv,      u8"É"sv,      u8"Ê"sv,      u8"Ë"sv,
    u8"Ì"sv,      u8"Í"sv,      u8"Î"sv,      u8"Ï"sv,      u8"Ñ"sv,
    u8"Ò"sv,      u8"Ó"sv,      u8"Ô"sv,      u8"Ö"sv,      u8"Ù"sv,
    u8"Ú"sv,      u8"Û"sv,      u8"Ü"sv,      u8"Œ"sv,      u8"ß"sv,
    u8"à"sv,      u8"á"sv,      u8"â"sv,      u8"ä"sv,      u8"ç"sv,
    u8"è"sv,      u8"é"sv,      u8"ê"sv,      u8"ë"sv,      u8"ì"sv,
    u8"í"sv,      u8"î"sv,      u8"ï"sv,      u8"ñ"sv,      u8"ò"sv,
    u8"ó"sv,      u8"ô"sv,      u8"ö"sv,      u8"ù"sv,      u8"ú"sv,
    u8"û"sv,      u8"ü"sv,      u8"œ"sv,      u8"\u2167"sv, u8"["sv,
    u8"]"sv,      u8"\u25A0"sv, u8"\u25CE"sv, u8"\u2666"sv, u8"\u3016"sv,
    u8"\u3017"sv, u8"\u25A1"sv, u8"\u2605"sv, u8"\u300E"sv, u8"\u300F"sv,
    u8"\u25BD"sv, u8";"sv,      u8"\u25BC"sv, u8"\u203E"sv, u8"\u2A2F"sv,
    u8"\u2606"sv, u8"\u02CA"sv, u8"\u2193"sv, u8"°"sv,      u8"¡"sv,
    u8"¿"sv,      u8"\u2500"sv, u8"«"sv,      u8"»"sv,      u8"±"sv,
    u8"\u266C"sv, u8"\xC4"sv,   u8"\u2191"sv, u8"V"sv,      u8"III"sv,
    u8"¡"sv,      u8"\u2122"sv, u8"<"sv,      u8">"sv,      u8"GA"sv,
    u8"ME"sv,     u8"FOL"sv,    u8"D"sv,      u8"ER"sv,     u8"Sl"sv,
    u8"ot"sv,     u8"lng"sv,    u8"St"sv,     u8"ec"sv,     u8"kp"sv,
    u8"la1"sv,    u8":z"sv,     u8"Fe"sv,     u8"nte"sv,    u8"lng"sv,
    u8"lre"sv,    u8":S:"sv,    u8"S0"sv,     u8"Ra"sv,     u8"nu"sv,
    u8"ra"sv,     u8"\u00AE"sv, u8"\xE3"sv,   u8"\xE4"sv,   u8"\xE5"sv,
    u8"\xE6"sv,   u8"\xE7"sv,   u8"in"sv,     u8"e "sv,     u8"ne"sv,
    u8"to"sv,     u8"re"sv,     u8"HP"sv,     u8"l "sv,     u8"ll"sv,
    u8"GF"sv,     u8"nt"sv,     u8"il"sv,     u8"o "sv,     u8"ef"sv,
    u8"on"sv,     u8" w"sv,     u8" r"sv,     u8"wi"sv,     u8"fi"sv,
    u8"\xFA"sv,   u8"s "sv,     u8"ar"sv,     u8"\u2C77"sv, u8" S"sv,
    u8"ag"sv
  };
  static constexpr std::array<std::u8string_view, 256> JP_CODE_PAGE = {
    u8"\x00"sv, u8"\x01"sv, u8"\n"sv,   u8"\x03"sv, u8"\x04"sv, u8"\x05"sv,
    u8"\x06"sv, u8"\x07"sv, u8"\x08"sv, u8"\x09"sv, u8"\x0A"sv, u8"\x0B"sv,
    u8"\x0C"sv, u8"\x0D"sv, u8"\x0E"sv, u8"\x0F"sv, u8"\x10"sv, u8"\x11"sv,
    u8"\x12"sv, u8"\x13"sv, u8"\x14"sv, u8"\x15"sv, u8"\x16"sv, u8"\x17"sv,
    u8"\x18"sv, u8"\x19"sv, u8"\x1A"sv, u8"\x1B"sv, u8"\x1C"sv, u8"\x1D"sv,
    u8"\x1E"sv, u8"\x1F"sv, u8"バ"sv,   u8"ば"sv,   u8"ビ"sv,   u8"び"sv,
    u8"ブ"sv,   u8"ぶ"sv,   u8"ベ"sv,   u8"べ"sv,   u8"ボ"sv,   u8"ぼ"sv,
    u8"ガ"sv,   u8"が"sv,   u8"ギ"sv,   u8"ぎ"sv,   u8"グ"sv,   u8"ぐ"sv,
    u8"ゲ"sv,   u8"げ"sv,   u8"ゴ"sv,   u8"ご"sv,   u8"ザ"sv,   u8"ざ"sv,
    u8"ジ"sv,   u8"じ"sv,   u8"ズ"sv,   u8"ず"sv,   u8"ゼ"sv,   u8"ぜ"sv,
    u8"ゾ"sv,   u8"ぞ"sv,   u8"ダ"sv,   u8"だ"sv,   u8"ヂ"sv,   u8"ぢ"sv,
    u8"ヅ"sv,   u8"づ"sv,   u8"デ"sv,   u8"で"sv,   u8"ド"sv,   u8"ど"sv,
    u8"ヴ"sv,   u8"パ"sv,   u8"ぱ"sv,   u8"ピ"sv,   u8"ぴ"sv,   u8"プ"sv,
    u8"ぷ"sv,   u8"ペ"sv,   u8"ぺ"sv,   u8"ポ"sv,   u8"ぽ"sv,   u8"０"sv,
    u8"１"sv,   u8"２"sv,   u8"３"sv,   u8"４"sv,   u8"５"sv,   u8"６"sv,
    u8"７"sv,   u8"８"sv,   u8"９"sv,   u8"、"sv,   u8"。"sv,   u8"　"sv,
    u8"ハ"sv,   u8"は"sv,   u8"ヒ"sv,   u8"ひ"sv,   u8"フ"sv,   u8"ふ"sv,
    u8"ヘ"sv,   u8"へ"sv,   u8"ホ"sv,   u8"ほ"sv,   u8"カ"sv,   u8"か"sv,
    u8"キ"sv,   u8"き"sv,   u8"ク"sv,   u8"く"sv,   u8"ケ"sv,   u8"け"sv,
    u8"コ"sv,   u8"こ"sv,   u8"サ"sv,   u8"さ"sv,   u8"シ"sv,   u8"し"sv,
    u8"ス"sv,   u8"す"sv,   u8"セ"sv,   u8"せ"sv,   u8"ソ"sv,   u8"そ"sv,
    u8"タ"sv,   u8"た"sv,   u8"チ"sv,   u8"ち"sv,   u8"ツ"sv,   u8"つ"sv,
    u8"テ"sv,   u8"て"sv,   u8"ト"sv,   u8"と"sv,   u8"ウ"sv,   u8"う"sv,
    u8"ア"sv,   u8"あ"sv,   u8"イ"sv,   u8"い"sv,   u8"エ"sv,   u8"え"sv,
    u8"オ"sv,   u8"お"sv,   u8"ナ"sv,   u8"な"sv,   u8"ニ"sv,   u8"に"sv,
    u8"ヌ"sv,   u8"ぬ"sv,   u8"ネ"sv,   u8"ね"sv,   u8"ノ"sv,   u8"の"sv,
    u8"マ"sv,   u8"ま"sv,   u8"ミ"sv,   u8"み"sv,   u8"ム"sv,   u8"む"sv,
    u8"メ"sv,   u8"め"sv,   u8"モ"sv,   u8"も"sv,   u8"ラ"sv,   u8"ら"sv,
    u8"リ"sv,   u8"り"sv,   u8"ル"sv,   u8"る"sv,   u8"レ"sv,   u8"れ"sv,
    u8"ロ"sv,   u8"ろ"sv,   u8"ヤ"sv,   u8"や"sv,   u8"ユ"sv,   u8"ゆ"sv,
    u8"ヨ"sv,   u8"よ"sv,   u8"ワ"sv,   u8"わ"sv,   u8"ン"sv,   u8"ん"sv,
    u8"ヲ"sv,   u8"を"sv,   u8"ッ"sv,   u8"っ"sv,   u8"ャ"sv,   u8"ゃ"sv,
    u8"ュ"sv,   u8"ゅ"sv,   u8"ョ"sv,   u8"ょ"sv,   u8"ァ"sv,   u8"ぁ"sv,
    u8"ィ"sv,   u8"ぃ"sv,   u8"ゥ"sv,   u8"ぅ"sv,   u8"ェ"sv,   u8"ぇ"sv,
    u8"ォ"sv,   u8"ぉ"sv,   u8"Ａ"sv,   u8"Ｂ"sv,   u8"Ｃ"sv,   u8"Ｄ"sv,
    u8"Ｅ"sv,   u8"Ｆ"sv,   u8"Ｇ"sv,   u8"Ｈ"sv,   u8"Ｉ"sv,   u8"Ｊ"sv,
    u8"Ｋ"sv,   u8"Ｌ"sv,   u8"Ｍ"sv,   u8"Ｎ"sv,   u8"Ｏ"sv,   u8"Ｐ"sv,
    u8"Ｑ"sv,   u8"Ｒ"sv,   u8"Ｓ"sv,   u8"Ｔ"sv,   u8"Ｕ"sv,   u8"Ｖ"sv,
    u8"Ｗ"sv,   u8"Ｘ"sv,   u8"Ｙ"sv,   u8"Ｚ"sv,   u8"！"sv,   u8"？"sv,
    u8"…"sv,    u8"＋"sv,   u8"－"sv,   u8"＝"sv,   u8"＊"sv,   u8"／"sv,
    u8"％"sv,   u8"＆"sv,   u8"「"sv,   u8"」"sv,   u8"（"sv,   u8"）"sv,
    u8"収"sv,   u8"容"sv,   u8"所"sv,   u8"駅"sv,   u8"・"sv,   u8"．"sv,
    u8"，"sv,   u8"："sv,   u8"～"sv,   u8"ー"sv
  };

public:
  // static constexpr std::array LangCodes = { "en"sv, "fr"sv, "es"sv, "it"sv,
  // "de"sv, "jp" };
  static constexpr auto &get_code_page()
  {
    if constexpr (langVal == LangT::jp) {
      return JP_CODE_PAGE;
    } else {
      return EU_CODE_PAGE;
    }
  }
  [[nodiscard]] auto static decode(uint8_t key) noexcept
  {
    return get_code_page().at(key);
  }
  [[nodiscard]] auto static decode(const std::span<const char> &buffer)
  {
    using u8stringstream =
      std::basic_stringstream<char8_t, std::char_traits<char8_t>>;
    if (std::empty(buffer)) {
      return std::u8string{};
    }
    u8stringstream ss{};
    for (auto key : buffer) {
      const auto value = decode(static_cast<uint8_t>(key));
      ss << value;
    }
    return ss.str();
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_FF8STRING_HPP
