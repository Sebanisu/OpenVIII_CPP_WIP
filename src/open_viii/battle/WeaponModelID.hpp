//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_WEAPONMODELID_HPP
#define OPENVIII_CPP_WIP_WEAPONMODELID_HPP
#include "CharacterID.hpp"

namespace open_viii {
/**
 * @brief Enum class representing the Weapon Model ID for each character.
 */
enum class WeaponModelID : std::uint8_t
{
  /**
   * @defgroup SquallWeapons Squall Leonhart's Weapons
   * @{
   */
  Revolver       = 0, /**< Revolver weapon */
  ShearTrigger   = 1, /**< Shear Trigger weapon */
  CuttingTrigger = 2, /**< Cutting Trigger weapon */
  FlameSaber     = 3, /**< Flame Saber weapon */
  TwinLance      = 4, /**< Twin Lance weapon */
  Punishment     = 5, /**< Punishment weapon */
  LionHeart      = 6, /**< Lion Heart weapon */
  /** @} */           // end of SquallWeapons group

  /**
   * @defgroup ZellWeapons Zell Dincht's Weapons
   * @{
   */
  MetalKnuckle   = 8,  /**< Metal Knuckle weapon */
  Maverick       = 9,  /**< Maverick weapon */
  Gauntlet       = 10, /**< Gauntlet weapon */
  Ehrgeiz        = 11, /**< Ehrgeiz weapon */
  /** @} */            // end of ZellWeapons group

  // Group 2: Irvine Kinneas
  Valiant        = 13,
  Ulysses        = 14,
  Bismarck       = 15,
  Exeter         = 16,

  // Group 3: Quistis Trepe
  ChainWhip      = 18,
  SlayingTail    = 19,
  RedScorpion    = 20,
  SaveTheQueen   = 21,

  // Group 4: Rinoa Heartilly
  Pinwheel       = 23,
  Valkyrie       = 24,
  RisingSun      = 25,
  Cardinal       = 26,
  ShootingStar   = 27,

  // Group 5: Selphie Tilmitt
  Flail          = 28,
  MorningStar    = 29,
  CrescentWish   = 30,
  StrangeVision  = 31,

  // Group 6: Seifer Almasy
  Hyperion       = 33,

  // Group 7: Edea Kramer
  // No weapon models

  // Group 8: Laguna Loire
  MachineGun     = 35,

  // Group 9: Kiros Seagill
  Katal          = 37,

  // Group A (10): Ward Zabac
  Harpoon        = 39,
};

/**
 * @brief Convert a WeaponModelID to a corresponding CharacterID.
 * @param weaponID WeaponModelID to be converted.
 * @return CharacterID corresponding to the given WeaponModelID.
 */
inline constexpr CharacterID
  WeaponModelIDToCharacterID(WeaponModelID weaponID) noexcept
{
  switch (weaponID) {
  case WeaponModelID::Revolver:
  case WeaponModelID::ShearTrigger:
  case WeaponModelID::CuttingTrigger:
  case WeaponModelID::FlameSaber:
  case WeaponModelID::TwinLance:
  case WeaponModelID::Punishment:
  case WeaponModelID::LionHeart:
    return CharacterID::SquallLeonhart;
  case WeaponModelID::MetalKnuckle:
  case WeaponModelID::Maverick:
  case WeaponModelID::Gauntlet:
  case WeaponModelID::Ehrgeiz:
    return CharacterID::ZellDincht;
  case WeaponModelID::Valiant:
  case WeaponModelID::Ulysses:
  case WeaponModelID::Bismarck:
  case WeaponModelID::Exeter:
    return CharacterID::IrvineKinneas;
  case WeaponModelID::ChainWhip:
  case WeaponModelID::SlayingTail:
  case WeaponModelID::RedScorpion:
  case WeaponModelID::SaveTheQueen:
    return CharacterID::QuistisTrepe;
  case WeaponModelID::Pinwheel:
  case WeaponModelID::Valkyrie:
  case WeaponModelID::RisingSun:
  case WeaponModelID::Cardinal:
  case WeaponModelID::ShootingStar:
    return CharacterID::RinoaHeartilly;
  case WeaponModelID::Flail:
  case WeaponModelID::MorningStar:
  case WeaponModelID::CrescentWish:
  case WeaponModelID::StrangeVision:
    return CharacterID::SelphieTilmitt;
  case WeaponModelID::Hyperion:
    return CharacterID::SeiferAlmasy;
  case WeaponModelID::MachineGun:
    return CharacterID::LagunaLoire;
  case WeaponModelID::Katal:
    return CharacterID::KirosSeagill;
  case WeaponModelID::Harpoon:
    return CharacterID::WardZabac;
  default:
    return CharacterID::BlankCharacter;
  }
}

/**
 * @brief Compare WeaponModelID and CharacterID for equality.
 * @param model_id WeaponModelID to be compared.
 * @param character_id CharacterID to be compared.
 * @return True if the WeaponModelID corresponds to the CharacterID, false
 * otherwise.
 */
inline constexpr bool
  operator==(WeaponModelID model_id, CharacterID character_id) noexcept
{
  return WeaponModelIDToCharacterID(model_id) == character_id;
}

/**
 * @brief Compare CharacterID and WeaponModelID for equality.
 * @param character_id CharacterID to be compared.
 * @param model_id WeaponModelID to be compared.
 * @return True if the CharacterID corresponds to the WeaponModelID, false
 * otherwise.
 */
inline constexpr bool
  operator==(CharacterID character_id, WeaponModelID model_id) noexcept
{
  return WeaponModelIDToCharacterID(model_id) == character_id;
}
static_assert(WeaponModelID::Flail != CharacterID::SquallLeonhart);
}
#endif// OPENVIII_CPP_WIP_WEAPONMODELID_HPP
