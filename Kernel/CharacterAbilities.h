//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_CHARACTERABILITIES_H
#define VIIIARCHIVE_CHARACTERABILITIES_H

#include "EncodedStringOffset.h"
#include "CharacterAbilityFlagsT.h"
#include <cstring>
namespace OpenVIII::Kernel {
struct CharacterAbilities
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Character-abilities
   * 0x0000	2 bytes	Offset to ability name
   * 0x0002	2 bytes	Offset to ability description
   * 0x0004	1 byte	AP Required to learn ability
   * 0x0005	3 byte	Flags
   */
private:
  EncodedStringOffset nameOffset_{};
  EncodedStringOffset descriptionOffset_{};
  std::uint8_t apRequired_{};
  // uint32_t characterAbilityFlags_ : 3;// cpp20 allows default member initializers for bitfields add {} in cpp20.
  std::array<std::uint8_t, 3> characterAbilityFlags_{};

public:
  [[nodiscard]] auto &NameOffset() const noexcept { return nameOffset_; }
  [[nodiscard]] auto &DescriptionOffset() const noexcept { return descriptionOffset_; }

  [[nodiscard]] auto APRequired() const noexcept { return apRequired_; }
  [[nodiscard]] auto CharacterAbilityFlags() const
  {
    // I think this is okay.
    // The size of the enum is 4 bytes but the field is 3 bytes.
    // return static_cast<CharacterAbilityFlagsT>(characterAbilityFlags_);
    CharacterAbilityFlagsT out{};
    std::memcpy(&out, characterAbilityFlags_.data(), characterAbilityFlags_.size());
    // out = static_cast<CharacterAbilityFlagsT>(static_cast<uint32_t>(out) << 1U);
    return out;
  }
  std::ostream &Out(std::ostream &os, const std::string_view &buffer) const
  {
    auto name = nameOffset_.DecodedString(buffer);
    auto description = descriptionOffset_.DecodedString(buffer);
    if (!std::empty(name)) {
      os << name;
    }
    auto size = sizeof(CharacterAbilities);
    if (!std::empty(description)) {
      os << ", " << description;
    }
    os << ", " << size;
    auto test = CharacterAbilityFlags();
    os << ", " << static_cast<uint32_t>(test);

    return os;
  }
};
}
#endif// VIIIARCHIVE_CHARACTERABILITIES_H
