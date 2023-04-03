//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_CHARACTERMODELID_HPP
#define OPENVIII_CPP_WIP_CHARACTERMODELID_HPP
#include "open_viii/kernel/CharactersT.hpp"

namespace open_viii {
enum class CharacterModelID : std::uint8_t
{
  // Group 0: Squall Leonhart
  squall_default  = 0,
  squall_uniform  = 1,

  // Group 1: Zell Dincht
  zell_default    = 3,
  zell_uniform    = 4,

  // Group 2: Irvine Kinneas
  irvine_default  = 6,

  // Group 3: Quistis Trepe
  quistis_default = 7,

  // Group 4: Rinoa Heartilly
  rinoa_default   = 9,

  // Group 5: Selphie Tilmitt
  selphie_default = 11,
  selphie_uniform = 12,

  // Group 6: Seifer Almasy
  seifer_default  = 14,

  // Group 7: Edea Kramer
  edea_default    = 16,

  // Group 8: Laguna Loire
  laguna_default  = 17,
  laguna_uniform  = 18,

  // Group 9: Kiros Seagill
  kiros_default   = 19,
  kiros_uniform   = 20,

  // Group A (10): Ward Zabac
  ward_default    = 21,
  ward_uniform    = 22
};
inline constexpr kernel::CharactersT
  characterModelIDToCharacterID(CharacterModelID modelID)
{
  using kernel::CharactersT;
  switch (modelID) {
  case CharacterModelID::squall_default:
  case CharacterModelID::squall_uniform:
    return CharactersT::squall_leonhart;
  case CharacterModelID::zell_default:
  case CharacterModelID::zell_uniform:
    return CharactersT::zell_dincht;
  case CharacterModelID::irvine_default:
    return CharactersT::irvine_kinneas;
  case CharacterModelID::quistis_default:
    return CharactersT::quistis_trepe;
  case CharacterModelID::rinoa_default:
    return CharactersT::rinoa_heartilly;
  case CharacterModelID::selphie_default:
  case CharacterModelID::selphie_uniform:
    return CharactersT::selphie_tilmitt;
  case CharacterModelID::seifer_default:
    return CharactersT::seifer_almasy;
  case CharacterModelID::edea_default:
    return CharactersT::edea_kramer;
  case CharacterModelID::laguna_default:
  case CharacterModelID::laguna_uniform:
    return CharactersT::laguna_loire;
  case CharacterModelID::kiros_default:
  case CharacterModelID::kiros_uniform:
    return CharactersT::kiros_seagill;
  case CharacterModelID::ward_default:
  case CharacterModelID::ward_uniform:
    return CharactersT::ward_zabac;
  default:
    return CharactersT::blank;
  }
}
inline constexpr bool
  operator==(
    CharacterModelID    model_id,
    kernel::CharactersT character_id) noexcept
{
  return characterModelIDToCharacterID(model_id) == character_id;
}
inline constexpr bool
  operator==(
    kernel::CharactersT character_id,
    CharacterModelID    model_id) noexcept
{
  return characterModelIDToCharacterID(model_id) == character_id;
}
static_assert(
  CharacterModelID::zell_default != kernel::CharactersT::squall_leonhart);
}// namespace open_viii
#endif// OPENVIII_CPP_WIP_CHARACTERMODELID_HPP
