//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_CHARACTERMODELID_HPP
#define OPENVIII_CPP_WIP_CHARACTERMODELID_HPP
#include "CharacterID.hpp"

namespace open_viii {
enum class CharacterModelID : std::uint8_t
{
  // Group 0: Squall Leonhart
  SquallDefault  = 0,
  SquallUniform  = 1,

  // Group 1: Zell Dincht
  ZellDefault    = 3,
  ZellUniform    = 4,

  // Group 2: Irvine Kinneas
  IrvineDefault  = 6,

  // Group 3: Quistis Trepe
  QuistisDefault = 7,

  // Group 4: Rinoa Heartilly
  RinoaDefault   = 9,

  // Group 5: Selphie Tilmitt
  SelphieDefault = 11,
  SelphieUniform = 12,

  // Group 6: Seifer Almasy
  SeiferDefault  = 14,

  // Group 7: Edea Kramer
  EdeaDefault    = 16,

  // Group 8: Laguna Loire
  LagunaDefault  = 17,
  LagunaUniform  = 18,

  // Group 9: Kiros Seagill
  KirosDefault   = 19,
  KirosUniform   = 20,

  // Group A (10): Ward Zabac
  WardDefault    = 21,
  WardUniform    = 22
};
inline constexpr CharacterID
  characterModelIDToCharacterID(CharacterModelID modelID)
{
  switch (modelID) {
  case CharacterModelID::SquallDefault:
  case CharacterModelID::SquallUniform:
    return CharacterID::SquallLeonhart;
  case CharacterModelID::ZellDefault:
  case CharacterModelID::ZellUniform:
    return CharacterID::ZellDincht;
  case CharacterModelID::IrvineDefault:
    return CharacterID::IrvineKinneas;
  case CharacterModelID::QuistisDefault:
    return CharacterID::QuistisTrepe;
  case CharacterModelID::RinoaDefault:
    return CharacterID::RinoaHeartilly;
  case CharacterModelID::SelphieDefault:
  case CharacterModelID::SelphieUniform:
    return CharacterID::SelphieTilmitt;
  case CharacterModelID::SeiferDefault:
    return CharacterID::SeiferAlmasy;
  case CharacterModelID::EdeaDefault:
    return CharacterID::EdeaKramer;
  case CharacterModelID::LagunaDefault:
  case CharacterModelID::LagunaUniform:
    return CharacterID::LagunaLoire;
  case CharacterModelID::KirosDefault:
  case CharacterModelID::KirosUniform:
    return CharacterID::KirosSeagill;
  case CharacterModelID::WardDefault:
  case CharacterModelID::WardUniform:
    return CharacterID::WardZabac;
  default:
    return CharacterID::BlankCharacter;
  }
}
inline constexpr bool
  operator==(CharacterModelID model_id, CharacterID character_id) noexcept
{
  return characterModelIDToCharacterID(model_id) == character_id;
}
inline constexpr bool
  operator==(CharacterID character_id, CharacterModelID model_id) noexcept
{
  return characterModelIDToCharacterID(model_id) == character_id;
}
static_assert(CharacterModelID::ZellDefault != CharacterID::SquallLeonhart);
}
#endif// OPENVIII_CPP_WIP_CHARACTERMODELID_HPP
