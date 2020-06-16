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
#include <vector>
#include <string_view>
#include <string>
#include <array>
#include <cstring>
#include <sstream>
#include <iostream>
#include <algorithm>
#ifndef VIIIARCHIVE_KERNEL_HEADER_H
#define VIIIARCHIVE_KERNEL_HEADER_H
// will be moving kernel into it"s own project once it"svs more fully baked.
namespace OpenVIII::Kernel {
enum class SectionTypes {
  // https://github.com/alexfilth/doomtrain/wiki/Header
  BattleCommands,
  Magic,
  JunctionableGFs,
  EnemyAttacks,
  Weapons,
  RenzokukenFinishers,
  Characters,
  BattleItems,
  NonBattleItems,
  NonJunctionableGFs,
  CommandAbilitiesData,
  JunctionAbilities,
  CommandAbilities,
  StatPercentIncreaseAbilities,
  CharacterAbilities,
  PartyAbilities,
  GFAbilities,
  MenuAbilities,
  TeamLagunaLimitBreaks,
  QuistisBlueMagicLimitBreak,
  QuistisBlueMagicLimitBreakParams,
  IrvineShotLimitBreak,
  ZellDuelLimitBreak,
  ZellDuelLimitBreakParams,
  RinoaLimitBreakPart1,
  RinoaLimitBreakPart2,
  SlotsArray,
  SlotsSets,
  Devour,
  Misc,
  MiscText,
  BattleCommandsText,
  MagicText,
  JunctionableGFsText,
  EnemyAttacksText,
  WeaponsText,
  RenzokukenFinishersText,
  CharactersText,
  BattleItemsText,
  NonBattleItemsText,
  NonJunctionableGFsText,
  JunctionAbilitiesText,
  CommandAbilitiesText,
  StatPercentIncreaseAbilitiesText,
  CharacterAbilitiesText,
  PartyAbilitiesText,
  GFAbilitiesText,
  MenuAbilitiesText,
  TeamLagunaLimitBreaksText,
  QuistisBlueMagicLimitBreakText,
  IrvineShotLimitBreakText,
  ZellDuelLimitBreakText,
  RinoaLimitBreakPart1Text,
  RinoaLimitBreakPart2Text,
  DevourText,
  MiscText2,
  Count,
  First = BattleCommands
};
using namespace std::literals;
struct FF8String
{
private:
  static constexpr std::array euCodePage = { "\x00"sv,
    "\x01"sv,
    "\n"sv,
    "\x03"sv,
    "\x04"sv,
    "\x05"sv,
    "\x06"sv,
    "\x07"sv,
    "\x08"sv,
    "\x09"sv,
    "\x0A"sv,
    "\x0B"sv,
    "\x0C"sv,
    "\x0D"sv,
    "\x0E"sv,
    "\x0F"sv,
    "\x10"sv,
    "\x11"sv,
    "\x12"sv,
    "\x13"sv,
    "\x14"sv,
    "\x15"sv,
    "\x16"sv,
    "\x17"sv,
    "\x18"sv,
    "\x19"sv,
    "\x1A"sv,
    "\x1B"sv,
    "\x1C"sv,
    "\x1D"sv,
    "\x1E"sv,
    "\x1F"sv,
    " "sv,
    "0"sv,
    "1"sv,
    "2"sv,
    "3"sv,
    "4"sv,
    "5"sv,
    "6"sv,
    "7"sv,
    "8"sv,
    "9"sv,
    "%"sv,
    "/"sv,
    ":"sv,
    "!"sv,
    "?"sv,
    "…"sv,
    "+"sv,
    "-"sv,
    "="sv,
    "*"sv,
    "&"sv,
    "\u300C"sv,
    "\u300D"sv,
    "("sv,
    ")"sv,
    "·"sv,
    "."sv,
    ","sv,
    "~"sv,
    "“"sv,
    "”"sv,
    "‘"sv,
    "#"sv,
    "$"sv,
    R"(")"sv,
    "_"sv,
    "A"sv,
    "B"sv,
    "C"sv,
    "D"sv,
    "E"sv,
    "F"sv,
    "G"sv,
    "H"sv,
    "I"sv,
    "J"sv,
    "K"sv,
    "L"sv,
    "M"sv,
    "N"sv,
    "O"sv,
    "P"sv,
    "Q"sv,
    "R"sv,
    "S"sv,
    "T"sv,
    "U"sv,
    "V"sv,
    "W"sv,
    "X"sv,
    "Y"sv,
    "Z"sv,
    "a"sv,
    "b"sv,
    "c"sv,
    "d"sv,
    "e"sv,
    "f"sv,
    "g"sv,
    "h"sv,
    "i"sv,
    "j"sv,
    "k"sv,
    "l"sv,
    "m"sv,
    "n"sv,
    "o"sv,
    "p"sv,
    "q"sv,
    "r"sv,
    "s"sv,
    "t"sv,
    "u"sv,
    "v"sv,
    "w"sv,
    "x"sv,
    "y"sv,
    "z"sv,
    "À"sv,
    "Á"sv,
    "Â"sv,
    "Ä"sv,
    "Ç"sv,
    "È"sv,
    "É"sv,
    "Ê"sv,
    "Ë"sv,
    "Ì"sv,
    "Í"sv,
    "Î"sv,
    "Ï"sv,
    "Ñ"sv,
    "Ò"sv,
    "Ó"sv,
    "Ô"sv,
    "Ö"sv,
    "Ù"sv,
    "Ú"sv,
    "Û"sv,
    "Ü"sv,
    "Œ"sv,
    "ß"sv,
    "à"sv,
    "á"sv,
    "â"sv,
    "ä"sv,
    "ç"sv,
    "è"sv,
    "é"sv,
    "ê"sv,
    "ë"sv,
    "ì"sv,
    "í"sv,
    "î"sv,
    "ï"sv,
    "ñ"sv,
    "ò"sv,
    "ó"sv,
    "ô"sv,
    "ö"sv,
    "ù"sv,
    "ú"sv,
    "û"sv,
    "ü"sv,
    "œ"sv,
    "\u2167"sv,
    "["sv,
    "]"sv,
    "\u25A0"sv,
    "\u25CE"sv,
    "\u2666"sv,
    "\u3016"sv,
    "\u3017"sv,
    "\u25A1"sv,
    "\u2605"sv,
    "\u300E"sv,
    "\u300F"sv,
    "\u25BD"sv,
    ";"sv,
    "\u25BC"sv,
    "\u203E"sv,
    "\u2A2F"sv,
    "\u2606"sv,
    "\u02CA"sv,
    "\u2193"sv,
    "°"sv,
    "¡"sv,
    "¿"sv,
    "\u2500"sv,
    "«"sv,
    "»"sv,
    "±"sv,
    "\u266C"sv,
    "\xC4"sv,
    "\u2191"sv,
    "V"sv,
    "III"sv,
    "¡"sv,
    "\u2122"sv,
    "<"sv,
    ">"sv,
    "GA"sv,
    "ME"sv,
    "FOL"sv,
    "D"sv,
    "ER"sv,
    "Sl"sv,
    "ot"sv,
    "lng"sv,
    "St"sv,
    "ec"sv,
    "kp"sv,
    "la1"sv,
    ":z"sv,
    "Fe"sv,
    "nte"sv,
    "lng"sv,
    "lre"sv,
    ":S:"sv,
    "S0"sv,
    "Ra"sv,
    "nu"sv,
    "ra"sv,
    "\u00AE"sv,
    "\xE3"sv,
    "\xE4"sv,
    "\xE5"sv,
    "\xE6"sv,
    "\xE7"sv,
    "in"sv,
    "e "sv,
    "ne"sv,
    "to"sv,
    "re"sv,
    "HP"sv,
    "l "sv,
    "ll"sv,
    "GF"sv,
    "nt"sv,
    "il"sv,
    "o "sv,
    "ef"sv,
    "on"sv,
    " w"sv,
    " r"sv,
    "wi"sv,
    "fi"sv,
    "\xFA"sv,
    "s "sv,
    "ar"sv,
    "\u2C77"sv,
    " S"sv,
    "ag"sv };

public:
  static constexpr auto &GetCodePage() { return euCodePage; }
  [[nodiscard]] auto static Decode(uint8_t key) noexcept
  {
    using namespace std::literals;
    return GetCodePage().at(key);
  }
  [[nodiscard]] auto static Decode(const std::string_view &buffer)
  {
    if (std::empty(buffer)) {
      return std::string{};
    }
    //    std::transform_reduce(buffer.begin(),buffer.end(),r,[](const auto &key)
    //    {return Decode(static_cast<uint8_t>(key));},[](auto a, auto b)){});
    std::stringstream ss{};
    for (auto key : buffer) {
      const auto value = Decode(static_cast<uint8_t>(key));
      ss << value;
    }
    return ss.str();
  }
};
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
    if (sizeof(spanT) == 0) {
      return 0;
    }
    return std::size(span_) / sizeof(spanT);
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
enum class Target : std::uint8_t {
  None = 0x00,
  Dead = 0x01,
  Unk02 = 0x02,
  Ally = 0x04,
  SingleSide = 0x08,
  SingleTarget = 0x10,
  Unk20 = 0x20,
  Enemy = 0x40,
  Unk80 = 0x80,
};
enum class Element : std::uint8_t {
  NonElemental = 0x00,
  Fire = 0x01,
  Ice = 0x02,
  Thunder = 0x04,
  Earth = 0x08,
  Poison = 0x10,
  Wind = 0x20,
  Water = 0x40,
  Holy = 0x80,
};
struct EncodedStringOffset
{
private:
  std::uint16_t offset_{};

  [[nodiscard]] static intmax_t FindStringSize(const std::string_view &buffer, const intmax_t offset)
  {
    if (static_cast<unsigned>(offset) > std::size(buffer)) {
      return -1;
    }
    auto first = buffer.begin() + offset;
    auto last = buffer.end();
    for (intmax_t i = 0; first != last; ++first, i++) {
      if (*first == '\0') {
        return i;
      }
    }
    return static_cast<signed>(std::size(buffer)) - offset;
  }
  [[nodiscard]] static auto GetStringAtOffset(const std::string_view &buffer, uint16_t offset)
  {
    if (offset != INT16_MAX && !std::empty(buffer) && std::size(buffer) > offset) {
      auto size = FindStringSize(buffer, static_cast<intmax_t>(offset));
      if (size >= 0) {
        return std::string_view(buffer.data() + offset, static_cast<size_t>(size));
      }
    }
    return ""sv;
  }

public:
  [[nodiscard]] auto RawBytes(const std::string_view &buffer) const { return GetStringAtOffset(buffer, offset_); }
  [[nodiscard]] auto DecodedString(const std::string_view &buffer) const { return FF8String::Decode(RawBytes(buffer)); }
};
struct BattleCommands
{
  /*
   * https://github.com/alexfilth/doomtrain/wiki/Battle-commands
   * Offset	Length	Description
   * 	2 bytes	Offset to ability name
   * 	2 bytes	Offset to ability description
   * 	1 byte	Ability data ID
   * 	1 byte	Unknown Flags
   * 	1 byte	Target
   * 	1 byte	Unknown / Unused
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t abilityDataID_{};
  std::uint8_t unknownFlags_{};
  Target target_{};
  std::uint8_t unknown_{};


public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  [[nodiscard]] auto AbilityDataID() const noexcept { return abilityDataID_; }
  [[nodiscard]] auto UnknownFlags() const noexcept { return unknownFlags_; }
  [[nodiscard]] auto GetTarget() const noexcept { return target_; }
  [[nodiscard]] auto Unknown() const noexcept { return unknown_; }

  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
template<typename T> struct StatGroup
{
private:
  T HP_{};
  T STR_{};
  T VIT_{};
  T MAG_{};
  T SPR_{};
  T SPD_{};
  T EVA_{};
  T HIT_{};
  T LUCK_{};

public:
  [[nodiscard]] T &HP() const noexcept { return HP_; }
  [[nodiscard]] T &STR() const noexcept { return STR_; }
  [[nodiscard]] T &VIT() const noexcept { return VIT_; }
  [[nodiscard]] T &MAG() const noexcept { return MAG_; }
  [[nodiscard]] T &SPR() const noexcept { return SPR_; }
  [[nodiscard]] T &SPD() const noexcept { return SPD_; }
  [[nodiscard]] T &EVA() const noexcept { return EVA_; }
  [[nodiscard]] T &HIT() const noexcept { return HIT_; }
  [[nodiscard]] T &LUCK() const noexcept { return LUCK_; }
};
template<typename T> struct GFGroup
{
private:
  T Quezacolt_{};
  T Shiva_{};
  T Ifrit_{};
  T Siren_{};
  T Brothers_{};
  T Diablos_{};
  T Carbuncle_{};
  T Leviathan_{};
  T Pandemona_{};
  T Cerberus_{};
  T Alexander_{};
  T Doomtrain_{};
  T Bahamut_{};
  T Cactuar_{};
  T Tonberry_{};
  T Eden_{};

public:
  [[nodiscard]] T &Quezacolt() const noexcept { return Quezacolt_; }
  [[nodiscard]] T &Shiva() const noexcept { return Shiva_; }
  [[nodiscard]] T &Ifrit() const noexcept { return Ifrit_; }
  [[nodiscard]] T &Siren() const noexcept { return Siren_; }
  [[nodiscard]] T &Brothers() const noexcept { return Brothers_; }
  [[nodiscard]] T &Diablos() const noexcept { return Diablos_; }
  [[nodiscard]] T &Carbuncle() const noexcept { return Carbuncle_; }
  [[nodiscard]] T &Leviathan() const noexcept { return Leviathan_; }
  [[nodiscard]] T &Pandemona() const noexcept { return Pandemona_; }
  [[nodiscard]] T &Cerberus() const noexcept { return Cerberus_; }
  [[nodiscard]] T &Alexander() const noexcept { return Alexander_; }
  [[nodiscard]] T &Doomtrain() const noexcept { return Doomtrain_; }
  [[nodiscard]] T &Bahamut() const noexcept { return Bahamut_; }
  [[nodiscard]] T &Cactuar() const noexcept { return Cactuar_; }
  [[nodiscard]] T &Tonberry() const noexcept { return Tonberry_; }
  [[nodiscard]] T &Eden() const noexcept { return Eden_; }
};
struct Magic
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Magic-data
   * Offset	Length	Description
   * 	2 bytes	Offset to spell name
   * 	2 bytes	Offset to spell description
   * 	2 bytes	[[Magic ID
   * 	1 byte	Unknown
   * 	1 byte	Attack type
   * 	1 byte	Spell power (used in damage formula)
   * 	1 byte	Unknown
   * 	1 byte	Default target
   * 	1 byte	Attack Flags
   * 	1 byte	Draw resist (how hard is the magic to draw)
   * 	1 byte	Hit count (works with meteor animation, not sure about others)
   * 	1 byte	[[Element
   * 	1 byte	Unknown
   * 	4 bytes	[[Statuses 1
   * 	2 bytes	[[Statuses 0
   * 	1 byte	Status attack enabler
   * 	1 byte	Characters HP junction value
   * 	1 byte	Characters STR junction value
   * 	1 byte	Characters VIT junction value
   * 	1 byte	Characters MAG junction value
   * 	1 byte	Characters SPR junction value
   * 	1 byte	Characters SPD junction value
   * 	1 byte	Characters EVA junction value
   * 	1 byte	Characters HIT junction value
   * 	1 byte	Characters LUCK junction value
   * 	1 byte	[[Characters J-Elem attack
   * 	1 byte	Characters J-Elem attack value
   * 	1 byte	[[Characters J-Elem defense
   * 	1 byte	Characters J-Elem defense value
   * 	1 byte	Characters J-Status attack value
   * 	1 byte	Characters J-Status defense value
   * 	2 bytes	[[Characters J-Statuses Attack
   * 	2 bytes	[[Characters J-Statuses Defend
   * 	1 byte	Quezacolt compatibility
   * 	1 byte	Shiva compatibility
   * 	1 byte	Ifrit compatibility
   * 	1 byte	Siren compatibility
   * 	1 byte	Brothers compatibility
   * 	1 byte	Diablos compatibility
   * 	1 byte	Carbuncle compatibility
   * 	1 byte	Leviathan compatibility
   * 	1 byte	Pandemona compatibility
   * 	1 byte	Cerberus compatibility
   * 	1 byte	Alexander compatibility
   * 	1 byte	Doomtrain compatibility
   * 	1 byte	Bahamut compatibility
   * 	1 byte	Cactuar compatibility
   * 	1 byte	Tonberry compatibility
   * 	1 byte	Eden compatibility
   * 	2 bytes	Unknown
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  uint16_t magicID_{};
  uint8_t unknown0_{};
  uint8_t attackType_{};
  uint8_t spellPower_{};
  uint8_t unknown1_{};
  Target target_{};
  uint8_t attackFlags_;
  uint8_t drawResist_{};
  uint8_t hitCount_{};
  Element elements_{};
  uint8_t unknown2_{};
  uint32_t statuses1_{};
  uint16_t statuses0_{};
  uint8_t statusAttack_{};
  StatGroup<uint8_t> Junction_{};
  uint8_t JElemAttackFlag_{};
  uint8_t JElemAttackValue_{};
  uint8_t JElemDefenseFlag_{};
  uint8_t JElemDefenseValue_{};
  uint8_t JStatusAttackValue_{};
  uint8_t JStatusDefenseValue_{};
  uint16_t JStatusesAttackFlag_{};
  uint16_t JStatusesDefendFlag_{};
  GFGroup<uint8_t> compatibility_{};
  uint16_t unknown3_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct JunctionableGFs
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct EnemyAttacks
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct Weapons
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct RenzokukenFinishers
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct Characters
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct BattleItems
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct NonBattleItems
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct NonJunctionableGFs
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct CommandAbilitiesData
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct JunctionAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct CommandAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct StatPercentIncreaseAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct CharacterAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct PartyAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct GFAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct MenuAbilities
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct TeamLagunaLimitBreaks
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct QuistisBlueMagicLimitBreak
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
// struct QuistisBlueMagicLimitBreakParams
//{
//};
struct IrvineShotLimitBreak
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct ZellDuelLimitBreak
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
// struct ZellDuelLimitBreakParams
//{
//};
struct RinoaLimitBreakPart1
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct RinoaLimitBreakPart2
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct SlotsArray
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
// struct SlotsSets
//{
//};
struct Devour
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
struct Misc
{
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer)
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    if (!std::empty(description)) {
      os << ", " << description;
    }
    return os;
  }
};
// struct MiscText
//{
//};
// struct BattleCommandsText
//{
//};
// struct MagicText
//{
//};
// struct JunctionableGFsText
//{
//};
// struct EnemyAttacksText
//{
//};
// struct WeaponsText
//{
//};
// struct RenzokukenFinishersText
//{
//};
// struct CharactersText
//{
//};
// struct BattleItemsText
//{
//};
// struct NonBattleItemsText
//{
//};
// struct NonJunctionableGFsText
//{
//};
// struct JunctionAbilitiesText
//{
//};
// struct CommandAbilitiesText
//{
//};
// struct StatPercentIncreaseAbilitiesText
//{
//};
// struct CharacterAbilitiesText
//{
//};
// struct PartyAbilitiesText
//{
//};
// struct GFAbilitiesText
//{
//};
// struct MenuAbilitiesText
//{
//};
// struct TeamLagunaLimitBreaksText
//{
//};
// struct QuistisBlueMagicLimitBreakText
//{
//};
// struct IrvineShotLimitBreakText
//{
//};
// struct ZellDuelLimitBreakText
//{
//};
// struct RinoaLimitBreakPart1Text
//{
//};
// struct RinoaLimitBreakPart2Text
//{
//};
// struct DevourText
//{
//};
// struct MiscText2
//{
//};


struct Header
{
  // https://github.com/alexfilth/doomtrain/wiki/Header
private:
  std::vector<char> buffer_{};
  std::vector<std::uint32_t> sectionOffsets_{};

public:
  template<SectionTypes sectionType>[[nodiscard]] constexpr auto GetSpan() const
  {
    if constexpr (static_cast<int>(sectionType) >= static_cast<int>(SectionTypes::Count)
                  || static_cast<int>(sectionType) < 0) {
      return nullptr;// failure
    }
    auto length = [this]() {
      if constexpr (static_cast<int>(sectionType) >= static_cast<int>(SectionTypes::Count) - 1) {
        return std::size(buffer_) - sectionOffsets_.at(static_cast<size_t>(sectionType));
      }
      return static_cast<size_t>(sectionOffsets_.at(static_cast<size_t>(sectionType) + 1)
                                 - sectionOffsets_.at(static_cast<size_t>(sectionType)));
    }();
    return std::string_view(buffer_.data() + sectionOffsets_.at(static_cast<size_t>(sectionType)), length);
  }
  template<SectionTypes sectionType>[[nodiscard]] auto GetSectionData() const
  {
    using namespace std::string_literals;
    if constexpr (sectionType == SectionTypes::BattleCommands) {
      return SectionData<BattleCommands>{ GetSpan<sectionType>(), GetSpan<SectionTypes::BattleCommandsText>() };
    } else if constexpr (sectionType == SectionTypes::Magic) {
      return SectionData<Magic>{ GetSpan<sectionType>(), GetSpan<SectionTypes::MagicText>() };
    } else if constexpr (sectionType == SectionTypes::JunctionableGFs) {
      return SectionData<JunctionableGFs>{ GetSpan<sectionType>(), GetSpan<SectionTypes::JunctionableGFsText>() };
    } else if constexpr (sectionType == SectionTypes::EnemyAttacks) {
      return SectionData<EnemyAttacks>{ GetSpan<sectionType>(), GetSpan<SectionTypes::EnemyAttacksText>() };
    } else if constexpr (sectionType == SectionTypes::Weapons) {
      return SectionData<Weapons>{ GetSpan<sectionType>(), GetSpan<SectionTypes::WeaponsText>() };
    } else if constexpr (sectionType == SectionTypes::RenzokukenFinishers) {
      return SectionData<RenzokukenFinishers>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::RenzokukenFinishersText>() };
    } else if constexpr (sectionType == SectionTypes::Characters) {
      return SectionData<Characters>{ GetSpan<sectionType>(), GetSpan<SectionTypes::CharactersText>() };
    } else if constexpr (sectionType == SectionTypes::BattleItems) {
      return SectionData<BattleItems>{ GetSpan<sectionType>(), GetSpan<SectionTypes::BattleItemsText>() };
    } else if constexpr (sectionType == SectionTypes::NonBattleItems) {
      return SectionData<NonBattleItems>{ GetSpan<sectionType>(), GetSpan<SectionTypes::NonBattleItemsText>() };
    } else if constexpr (sectionType == SectionTypes::NonJunctionableGFs) {
      return SectionData<NonJunctionableGFs>{ GetSpan<sectionType>(), GetSpan<SectionTypes::NonJunctionableGFsText>() };
    } else if constexpr (sectionType == SectionTypes::CommandAbilitiesData) {
      return SectionData<CommandAbilitiesData>{ GetSpan<sectionType>(), GetSpan<SectionTypes::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::JunctionAbilities) {
      return SectionData<JunctionAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypes::JunctionAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::CommandAbilities) {
      return SectionData<CommandAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypes::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::StatPercentIncreaseAbilities) {
      return SectionData<StatPercentIncreaseAbilities>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::StatPercentIncreaseAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::CharacterAbilities) {
      return SectionData<CharacterAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypes::CommandAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::PartyAbilities) {
      return SectionData<PartyAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypes::PartyAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::GFAbilities) {
      return SectionData<GFAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypes::GFAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::MenuAbilities) {
      return SectionData<MenuAbilities>{ GetSpan<sectionType>(), GetSpan<SectionTypes::MenuAbilitiesText>() };
    } else if constexpr (sectionType == SectionTypes::TeamLagunaLimitBreaks) {
      return SectionData<TeamLagunaLimitBreaks>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::TeamLagunaLimitBreaksText>() };
    } else if constexpr (sectionType == SectionTypes::QuistisBlueMagicLimitBreak) {
      return SectionData<QuistisBlueMagicLimitBreak>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::QuistisBlueMagicLimitBreakText>(),
        GetSpan<SectionTypes::QuistisBlueMagicLimitBreakParams>() };
    } else if constexpr (sectionType == SectionTypes::IrvineShotLimitBreak) {
      return SectionData<IrvineShotLimitBreak>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::IrvineShotLimitBreakText>() };
    } else if constexpr (sectionType == SectionTypes::ZellDuelLimitBreak) {
      return SectionData<ZellDuelLimitBreak>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::ZellDuelLimitBreakText>(),
        GetSpan<SectionTypes::ZellDuelLimitBreakParams>() };
    } else if constexpr (sectionType == SectionTypes::RinoaLimitBreakPart1) {
      return SectionData<RinoaLimitBreakPart1>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::RinoaLimitBreakPart1Text>() };
    } else if constexpr (sectionType == SectionTypes::RinoaLimitBreakPart2) {
      return SectionData<RinoaLimitBreakPart2>{ GetSpan<sectionType>(),
        GetSpan<SectionTypes::RinoaLimitBreakPart2Text>() };
    } else if constexpr (sectionType == SectionTypes::SlotsArray) {
      return SectionData<SlotsArray>{ GetSpan<sectionType>(), {}, GetSpan<SectionTypes::SlotsSets>() };
    }

    else if constexpr (sectionType == SectionTypes::Devour) {
      return SectionData<Devour>{ GetSpan<sectionType>(), GetSpan<SectionTypes::DevourText>() };
    } else if constexpr (sectionType == SectionTypes::Misc) {
      return SectionData<Misc>{
        GetSpan<sectionType>(), GetSpan<SectionTypes::MiscText>(), GetSpan<SectionTypes::MiscText2>()
      };
    } else {
      return nullptr;
    }
  }

  template<SectionTypes sectionType>[[nodiscard]] constexpr std::string_view GetSectionName() const
  {
    using namespace std::string_literals;
    if constexpr (sectionType == SectionTypes::BattleCommands) {
      return "BattleCommands"sv;
    }
    if constexpr (sectionType == SectionTypes::Magic) {
      return "Magic"sv;
    }
    if constexpr (sectionType == SectionTypes::JunctionableGFs) {
      return "JunctionableGFs"sv;
    }
    if constexpr (sectionType == SectionTypes::EnemyAttacks) {
      return "EnemyAttacks"sv;
    }
    if constexpr (sectionType == SectionTypes::Weapons) {
      return "Weapons"sv;
    }
    if constexpr (sectionType == SectionTypes::RenzokukenFinishers) {
      return "RenzokukenFinishers"sv;
    }
    if constexpr (sectionType == SectionTypes::Characters) {
      return "Characters"sv;
    }
    if constexpr (sectionType == SectionTypes::BattleItems) {
      return "BattleItems"sv;
    }
    if constexpr (sectionType == SectionTypes::NonBattleItems) {
      return "NonBattleItems"sv;
    }
    if constexpr (sectionType == SectionTypes::NonJunctionableGFs) {
      return "NonJunctionableGFs"sv;
    }
    if constexpr (sectionType == SectionTypes::CommandAbilitiesData) {
      return "CommandAbilitiesData"sv;
    }
    if constexpr (sectionType == SectionTypes::JunctionAbilities) {
      return "JunctionAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::CommandAbilities) {
      return "CommandAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::StatPercentIncreaseAbilities) {
      return "StatPercentIncreaseAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::CharacterAbilities) {
      return "CharacterAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::PartyAbilities) {
      return "PartyAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::GFAbilities) {
      return "GFAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::MenuAbilities) {
      return "MenuAbilities"sv;
    }
    if constexpr (sectionType == SectionTypes::TeamLagunaLimitBreaks) {
      return "TeamLagunaLimitBreaks"sv;
    }
    if constexpr (sectionType == SectionTypes::QuistisBlueMagicLimitBreak) {
      return "QuistisBlueMagicLimitBreak"sv;
    }
    if constexpr (sectionType == SectionTypes::QuistisBlueMagicLimitBreakParams) {
      return "QuistisBlueMagicLimitBreakParams"sv;
    }
    if constexpr (sectionType == SectionTypes::IrvineShotLimitBreak) {
      return "IrvineShotLimitBreak"sv;
    }
    if constexpr (sectionType == SectionTypes::ZellDuelLimitBreak) {
      return "ZellDuelLimitBreak"sv;
    }
    if constexpr (sectionType == SectionTypes::ZellDuelLimitBreakParams) {
      return "ZellDuelLimitBreakParams"sv;
    }
    if constexpr (sectionType == SectionTypes::RinoaLimitBreakPart1) {
      return "RinoaLimitBreakPart1"sv;
    }
    if constexpr (sectionType == SectionTypes::RinoaLimitBreakPart2) {
      return "RinoaLimitBreakPart2"sv;
    }
    if constexpr (sectionType == SectionTypes::SlotsArray) {
      return "SlotsArray"sv;
    }
    if constexpr (sectionType == SectionTypes::SlotsSets) {
      return "SlotsSets"sv;
    }
    if constexpr (sectionType == SectionTypes::Devour) {
      return "Devour"sv;
    }
    if constexpr (sectionType == SectionTypes::Misc) {
      return "Misc"sv;
    }
    if constexpr (sectionType == SectionTypes::MiscText) {
      return "MiscText"sv;
    }
    if constexpr (sectionType == SectionTypes::BattleCommandsText) {
      return "BattleCommandsText"sv;
    }
    if constexpr (sectionType == SectionTypes::MagicText) {
      return "MagicText"sv;
    }
    if constexpr (sectionType == SectionTypes::JunctionableGFsText) {
      return "JunctionableGFsText"sv;
    }
    if constexpr (sectionType == SectionTypes::EnemyAttacksText) {
      return "EnemyAttacksText"sv;
    }
    if constexpr (sectionType == SectionTypes::WeaponsText) {
      return "WeaponsText"sv;
    }
    if constexpr (sectionType == SectionTypes::RenzokukenFinishersText) {
      return "RenzokukenFinishersText"sv;
    }
    if constexpr (sectionType == SectionTypes::CharactersText) {
      return "CharactersText"sv;
    }
    if constexpr (sectionType == SectionTypes::BattleItemsText) {
      return "BattleItemsText"sv;
    }
    if constexpr (sectionType == SectionTypes::NonBattleItemsText) {
      return "NonBattleItemsText"sv;
    }
    if constexpr (sectionType == SectionTypes::NonJunctionableGFsText) {
      return "NonJunctionableGFsText"sv;
    }
    if constexpr (sectionType == SectionTypes::JunctionAbilitiesText) {
      return "JunctionAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::CommandAbilitiesText) {
      return "CommandAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::StatPercentIncreaseAbilitiesText) {
      return "StatPercentIncreaseAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::CharacterAbilitiesText) {
      return "CharacterAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::PartyAbilitiesText) {
      return "PartyAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::GFAbilitiesText) {
      return "GFAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::MenuAbilitiesText) {
      return "MenuAbilitiesText"sv;
    }
    if constexpr (sectionType == SectionTypes::TeamLagunaLimitBreaksText) {
      return "TeamLagunaLimitBreaksText"sv;
    }
    if constexpr (sectionType == SectionTypes::QuistisBlueMagicLimitBreakText) {
      return "QuistisBlueMagicLimitBreakText"sv;
    }
    if constexpr (sectionType == SectionTypes::IrvineShotLimitBreakText) {
      return "IrvineShotLimitBreakText"sv;
    }
    if constexpr (sectionType == SectionTypes::ZellDuelLimitBreakText) {
      return "ZellDuelLimitBreakText"sv;
    }
    if constexpr (sectionType == SectionTypes::RinoaLimitBreakPart1Text) {
      return "RinoaLimitBreakPart1Text"sv;
    }
    if constexpr (sectionType == SectionTypes::RinoaLimitBreakPart2Text) {
      return "RinoaLimitBreakPart2Text"sv;
    }
    if constexpr (sectionType == SectionTypes::DevourText) {
      return "DevourText"sv;
    }
    if constexpr (sectionType == SectionTypes::MiscText2) {
      return "MiscText2"sv;
    }
    return ""sv;
  }

  template<typename mainT> explicit Header(const mainT &main)
  {
    buffer_ = main.GetEntryData(std::string_view("kernel.bin"));
    if (std::size(buffer_) < sizeof(std::uint32_t)) {
      return;
    }
    std::uint32_t sectionCount{};
    std::memcpy(&sectionCount, buffer_.data(), sizeof(sectionCount));
    if (std::size(buffer_) < sizeof(std::uint32_t) * (sectionCount + 1)) {
      return;
    }
    sectionOffsets_.reserve(sectionCount);
    size_t i{ 1 };
    while (sectionCount-- > 0) {
      std::memcpy(
        &sectionOffsets_.emplace_back(), buffer_.data() + (sizeof(sectionCount) * (i++)), sizeof(sectionCount));
    }
  }
  [[nodiscard]] const auto &Buffer() const noexcept { return buffer_; }
  [[nodiscard]] auto SectionCount() const noexcept { return sectionOffsets_.size(); }
  [[nodiscard]] const auto &SectionOffsets() const noexcept { return sectionOffsets_; }


  template<int First, int Count, typename Lambda> void static_for(Lambda const &f)
  {// https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
    if constexpr (First < Count) {
      constexpr auto sectionType = std::integral_constant<SectionTypes, static_cast<SectionTypes>(First)>{};
      const auto & data = GetSectionData<sectionType>();
        f(GetSectionName<sectionType>(), GetSpan<sectionType>(), data);

      static_for<First + 1, Count>(f);
    }
  }
};// namespace OpenVIII::Kernel
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_KERNEL_HEADER_H
