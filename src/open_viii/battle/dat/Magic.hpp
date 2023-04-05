//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_MAGIC_HPP
#define OPENVIII_CPP_WIP_MAGIC_HPP
#include <cstdint>
#include "open_viii/kernel/MagicSpell.hpp"
namespace open_viii::battle::dat {
/**
   * @brief Represents a magic with an ID, an unknown value, and an overloaded
   * operator to cast to GFID.
 */
struct Magic
{
  kernel::MagicSpell ID; ///< The unique identifier for the magic.
  uint8_t            unk;///< An unknown value associated with the magic.

                         /**
     * @brief Casts the Magic object to a GFID object.
     *
     * This operator will return a Blank GFID if the ID is not within the valid
     * range.
     *
     * @return A GFID object representing the Magic object.
                          */
                     operator kernel::GFID() const noexcept
  {
    switch (ID) {
    default:
      return kernel::GFID::Blank;
    case kernel::MagicSpell::Quezacotl:
      return kernel::GFID::Quezacotl;
    case kernel::MagicSpell::Shiva:
      return kernel::GFID::Shiva;
    case kernel::MagicSpell::Ifrit:
      return kernel::GFID::Ifrit;
    case kernel::MagicSpell::Siren:
      return kernel::GFID::Siren;
    case kernel::MagicSpell::Brothers:
      return kernel::GFID::Brothers;
    case kernel::MagicSpell::Diablos:
      return kernel::GFID::Diablos;
    case kernel::MagicSpell::Carbuncle:
      return kernel::GFID::Carbuncle;
    case kernel::MagicSpell::Leviathan:
      return kernel::GFID::Leviathan;
    case kernel::MagicSpell::Pandemona:
      return kernel::GFID::Pandemona;
    case kernel::MagicSpell::Cerberus:
      return kernel::GFID::Cerberus;
    case kernel::MagicSpell::Alexander:
      return kernel::GFID::Alexander;
    case kernel::MagicSpell::Doomtrain:
      return kernel::GFID::Doomtrain;
    case kernel::MagicSpell::Bahamut:
      return kernel::GFID::Bahamut;
    case kernel::MagicSpell::Cactuar:
      return kernel::GFID::Cactuar;
    case kernel::MagicSpell::Tonberry:
      return kernel::GFID::Tonberry;
    case kernel::MagicSpell::Eden:
      return kernel::GFID::Eden;
    }
  }
};
}
#endif// OPENVIII_CPP_WIP_MAGIC_HPP
