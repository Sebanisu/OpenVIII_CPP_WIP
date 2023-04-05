//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION7_INFORMATION_AND_STATS_HPP
#define OPENVIII_CPP_WIP_SECTION7_INFORMATION_AND_STATS_HPP
#include "ElementalResist.hpp"
#include "Item.hpp"
#include "Magic.hpp"
#include "MentalResist.hpp"
#include "StatusFlags.hpp"
#include "StatusFlags2.hpp"
#include "UnkFlags.hpp"
#include <array>
#include <cstdint>
#include <string>
namespace open_viii::battle {
/**
 * @brief Contains information and statistics about a character in the game.
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/Information.cs
 * @see
 * http://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Section_7:_Informations_.26_stats
 * @see https://sourceforge.net/projects/ifrit/
 */
struct Section7_Information_and_Stats
{
  std::array<char, 24> encoded_name;///< Encoded name of the character.
  std::int32_t         hp;          ///< Character's HP.
  std::int32_t         str;         ///< Character's strength.
  std::int32_t         vit;         ///< Character's vitality.
  std::int32_t         mag;         ///< Character's magic.
  std::int32_t         spr;         ///< Character's spirit.
  std::int32_t         spd;         ///< Character's speed.
  std::int32_t         eva;         ///< Character's evasion.
  std::array<std::int32_t, 16>
    abilities_low;///< Character's low-level abilities.
  std::array<std::int32_t, 16>
    abilities_med;///< Character's medium-level abilities.
  std::array<std::int32_t, 16>
    abilities_high;///< Character's high-level abilities.
  std::uint8_t
    med_level_start;///< Character's medium-level ability starting point.
  std::uint8_t
           high_level_start;///< Character's high-level ability starting point.
  UnkFlags unknown_flags1;  ///< Unknown flags 1.
  StatusFlags                 status_flags;///< Character's status flags.
  std::array<CardID, 3>       cards;       ///< Character's cards.
  std::array<std::uint8_t, 3> devour;      ///< Character's devour ability.
  StatusFlags2         status_flags2;  ///< Character's additional status flags.
  UnkFlags             unknown_flags2; ///< Unknown flags 2.
  std::int16_t         extra_exp;      ///< Character's extra experience.
  std::int16_t         exp;            ///< Character's experience.
  std::array<Magic, 4> draw_low;       ///< Character's low-level draw magic.
  std::array<Magic, 4> draw_med;       ///< Character's medium-level draw magic.
  std::array<Magic, 4> draw_high;      ///< Character's high-level draw magic.
  std::array<Item, 4>  mug_low;        ///< Character's low-level mug items.
  std::array<Item, 4>  mug_med;        ///< Character's medium-level mug items.
  std::array<Item, 4>  mug_high;       ///< Character's high-level mug items.
  std::array<Item, 4>  drop_low;       ///< Character's low-level drop items.
  std::array<Item, 4>  drop_med;       ///< Character's medium-level drop items.
  std::array<Item, 4>  drop_high;      ///< Character's high-level drop items.
  std::uint8_t         mug_rate;       ///< Character's mug rate.
  std::uint8_t         drop_rate;      ///< Character's drop rate.
  std::uint8_t         padding;        ///< Padding byte.
  std::uint8_t         aps;            ///< Character's ability points.
  std::array<std::uint8_t, 16> unknown;///< Unknown byte array.
  ElementalResist elemental_resistance;///< Character's elemental resistance.
  MentalResist    mental_resistance;   ///< Character's mental resistance.

  /**
   * @brief Returns the character's name as a UTF-8 encoded string.
   *
   * @tparam coo Language for the character's name (default: en).
   * @return Character's name in UTF-8 encoding.
   */
  template<LangT coo = LangT::en>
  [[nodiscard]] std::u8string
    u8name() const
  {
    return FF8String<coo>().decode(encoded_name);
  }
  /**
   * @brief Returns the character's name as a std::string.
   *
   * @tparam coo Language for the character's name (default: en).
   * @return Character's name as a std::string.
   */
  template<LangT coo = LangT::en>
  [[nodiscard]] std::string
    name() const
  {
    return tools::u8_to_s(u8name<coo>());
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_SECTION7_INFORMATION_AND_STATS_HPP
