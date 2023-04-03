//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_DATFILE_HPP
#define OPENVIII_CPP_WIP_DATFILE_HPP
#include "open_viii/CardID.hpp"
#include "open_viii/kernel/BattleOnlyStatusesT.hpp"
#include "open_viii/kernel/ElementT.hpp"
#include "open_viii/kernel/GFID.hpp"
#include "open_viii/kernel/MagicSpell.hpp"
#include "open_viii/kernel/PersistentStatusesT.hpp"
#include "open_viii/strings/FF8String.hpp"
#include "open_viii/tools/Read.hpp"
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>
namespace open_viii::battle {
/**
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT
 * @see https://forums.qhimm.com/index.php?topic=11137.msg154834
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/DatFile.cs
 * @see https://sourceforge.net/projects/ifrit/
 */
class DatFile
{
private:
  std::vector<char>     m_buffer{};
  std::filesystem::path m_path{};
  // Header Begin:
  struct DatHeader
  {
    std::uint32_t              m_count{};///< 11 or 2;
    std::vector<std::uint32_t> m_offsets{};
    DatHeader() = default;
    DatHeader(std::span<const char> buffer)
      : m_count(open_viii::tools::read_val<std::uint32_t>(buffer)),
        m_offsets(open_viii::tools::read_vals<std::uint32_t>(buffer, m_count))
    {
      // if count == 2 then only Section 7 and 8 exist.
    }
  };
  // Section 7: Informations & stats
  /**
   * @brief Represents an item with an ID and quantity.
   */
  struct Item
  {
    uint8_t ID;      ///< The unique identifier for the item.
    uint8_t quantity;///< The quantity of the item.
  };

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
    /**
     * @brief Enumeration of status flags for a character.
     */
    enum class StatusFlags : std::uint8_t
    {
      None        = 0,   ///< No status flag.
      Zombie      = 0x1, ///< Zombie status flag.
      Fly         = 0x2, ///< Fly status flag.
      Zz1         = 0x4, ///< Unused status flag 1.
      Zz2         = 0x8, ///< Unused status flag 2.
      Zz3         = 0x10,///< Unused status flag 3.
      AutoReflect = 0x20,///< Auto-Reflect status flag.
      AutoShell   = 0x40,///< Auto-Shell status flag.
      AutoProtect = 0x80,///< Auto-Protect status flag.
    };

    /**
     * @brief Enumeration of additional status flags for a character.
     */
    enum class StatusFlags2 : std::uint8_t
    {
      None          = 0,   ///< No status flag.
      Zz1           = 0x1, ///< Unused status flag 1.
      Zz2           = 0x2, ///< Unused status flag 2.
      Unused1       = 0x4, ///< Unused status flag 3.
      Unused2       = 0x8, ///< Unused status flag 4.
      Unused3       = 0x10,///< Unused status flag 5.
      Unused4       = 0x20,///< Unused status flag 6.
      DiablosMissed = 0x40,///< Diablos missed status flag.
      AlwaysCard    = 0x80,///< Always card status flag.
    };

    /**
     * @brief Enumeration of unknown flags.
     */
    enum class UnkFlags : std::uint8_t
    {
      None = 0,   ///< No flag.
      Unk0 = 0x1, ///< Unknown flag 0.
      Unk1 = 0x2, ///< Unknown flag 1.
      Unk2 = 0x4, ///< Unknown flag 2.
      Unk3 = 0x8, ///< Unknown flag 3.
      Unk4 = 0x10,///< Unknown flag 4.
      Unk5 = 0x20,///< Unknown flag 5.
      Unk6 = 0x40,///< Unknown flag 6.
      Unk7 = 0x80,///< Unknown flag 7.
    };

    /**
     * @brief Represents elemental resistances for a character.
     */
    struct ElementalResist
    {
      std::uint8_t Fire;   ///< Fire resistance.
      std::uint8_t Ice;    ///< Ice resistance.
      std::uint8_t Thunder;///< Thunder resistance.
      std::uint8_t Earth;  ///< Earth resistance.
      std::uint8_t Poison; ///< Poison resistance.
      std::uint8_t Wind;   ///< Wind resistance.
      std::uint8_t Water;  ///< Water resistance.
      std::uint8_t Holy;   ///< Holy resistance.

      /**
       * @brief Gets a range of set flags for a given Element value.
       *
       * @param elementFlags The Element value containing the flags.
       * @return A range of Element values corresponding to the set flags.
       */
      constexpr static auto
        getSetElementFlags(kernel::ElementT elementFlags)
      {
        std::uint8_t   minFlagOffset = 0U;
        std::uint8_t   maxFlagOffset = 8U;

        constexpr auto flagValue = [](std::uint8_t offset) -> kernel::ElementT {
          return static_cast<kernel::ElementT>(1U << offset);
        };

        const auto isSet = [=](kernel::ElementT flag) {
          return (static_cast<std::uint8_t>(flag)
                  & static_cast<std::uint8_t>(elementFlags))
              != 0U;
        };

        return std::views::iota(minFlagOffset, maxFlagOffset)
             | std::views::transform(flagValue) | std::views::filter(isSet);
      }

      /**
       * @brief Gets a range of references to the element resistances for a
       * given Element flag.
       *
       * @param element_flag The Element flag value.
       * @return A range of references to the corresponding element
       * resistances.
       */
      [[nodiscard]] auto
        getRefToElementResistances(kernel::ElementT element_flag)
      {
        return getSetElementFlags(element_flag)
             | std::views::transform(
                 [this](kernel::ElementT current_flag) -> std::uint8_t & {
                   return operator[](current_flag);
                 });
      }

      /**
       * @brief Gets a range of constant references to the element resistances
       * for a given Element flag.
       *
       * @param element_flag The Element flag value.
       * @return A range of constant references to the corresponding element
       * resistances.
       */
      [[nodiscard]] auto
        getRefToElementResistances(kernel::ElementT element_flag) const
      {
        return getSetElementFlags(element_flag)
             | std::views::transform(
                 [this](kernel::ElementT current_flag) -> const std::uint8_t & {
                   return operator[](current_flag);
                 });
      }

      /**
       * @brief Index operator to access the resistance value of a specific
       * element.
       *
       * @param element The element whose resistance value is to be accessed.
       * @return A reference to the corresponding resistance value.
       */
      std::uint8_t &
        operator[](kernel::ElementT element)
      {
        switch (element) {
        case kernel::ElementT::fire:
          return Fire;
        case kernel::ElementT::ice:
          return Ice;
        case kernel::ElementT::thunder:
          return Thunder;
        case kernel::ElementT::earth:
          return Earth;
        case kernel::ElementT::poison:
          return Poison;
        case kernel::ElementT::wind:
          return Wind;
        case kernel::ElementT::water:
          return Water;
        case kernel::ElementT::holy:
          return Holy;
        default:
          throw std::out_of_range("Invalid element");
        }
      }
      /**
       * @brief Index operator to access the resistance value of a specific
       * element.
       *
       * @param element The element whose resistance value is to be accessed.
       * @return A reference to the corresponding resistance value.
       */
      const std::uint8_t &
        operator[](kernel::ElementT element) const
      {
        switch (element) {
        case kernel::ElementT::fire:
          return Fire;
        case kernel::ElementT::ice:
          return Ice;
        case kernel::ElementT::thunder:
          return Thunder;
        case kernel::ElementT::earth:
          return Earth;
        case kernel::ElementT::poison:
          return Poison;
        case kernel::ElementT::wind:
          return Wind;
        case kernel::ElementT::water:
          return Water;
        case kernel::ElementT::holy:
          return Holy;
        default:
          throw std::out_of_range("Invalid element");
        }
      }
    };

    /**
     * @brief Represents mental resistances for a character.
     */
    struct MentalResist
    {
      std::uint8_t Death;      ///< Death resistance.
      std::uint8_t Poison;     ///< Poison resistance.
      std::uint8_t Petrify;    ///< Petrify resistance.
      std::uint8_t Darkness;   ///< Darkness resistance.
      std::uint8_t Silence;    ///< Silence resistance.
      std::uint8_t Berserk;    ///< Berserk resistance
      std::uint8_t Zombie;     ///< Zombie resistance
      std::uint8_t Sleep;      ///< Sleep resistance
      std::uint8_t Haste;      ///< Haste resistance
      std::uint8_t Slow;       ///< Slow resistance
      std::uint8_t Stop;       ///< Stop resistance
      std::uint8_t Regen;      ///< Regen resistance
      std::uint8_t Reflect;    ///< Reflect resistance
      std::uint8_t Doom;       ///< Doom resistance
      std::uint8_t SlowPetrify;///< SlowPetrify resistance
      std::uint8_t Float;      ///< Float resistance
      std::uint8_t Confuse;    ///< Confuse resistance
      std::uint8_t Drain;      ///< Drain resistance
      std::uint8_t Expulsion;  ///< Expulsion resistance
      std::uint8_t Percent;    ///< Percent resistance

      std::uint8_t &
        operator[](kernel::PersistentStatusesT status)
      {
        using kernel::PersistentStatusesT;
        switch (status) {
        case PersistentStatusesT::death:
          return Death;
        case PersistentStatusesT::poison:
          return Poison;
        case PersistentStatusesT::petrify:
          return Petrify;
        case PersistentStatusesT::darkness:
          return Darkness;
        case PersistentStatusesT::silence:
          return Silence;
        case PersistentStatusesT::berserk:
          return Berserk;
        case PersistentStatusesT::zombie:
          return Zombie;
        // ... (Other cases)
        default:
          throw std::invalid_argument("Invalid status");
        }
      }

      std::uint8_t &
        operator[](kernel::BattleOnlyStatusesT status)
      {
        using kernel::BattleOnlyStatusesT;
        switch (status) {
        case BattleOnlyStatusesT::sleep:
          return Sleep;
        case BattleOnlyStatusesT::haste:
          return Haste;
        case BattleOnlyStatusesT::slow:
          return Slow;
        case BattleOnlyStatusesT::stop:
          return Stop;
        case BattleOnlyStatusesT::regen:
          return Regen;
        case BattleOnlyStatusesT::reflect:
          return Reflect;
        case BattleOnlyStatusesT::doom:
          return Doom;
        case BattleOnlyStatusesT::petrifying:
          return SlowPetrify;
        case BattleOnlyStatusesT::floating:
          return Float;
        case BattleOnlyStatusesT::confuse:
          return Confuse;
        case BattleOnlyStatusesT::eject:
          return Expulsion;
        // case BattleOnlyStatusesT::???: return Percent;
        //  ... (Other cases)
        default:
          throw std::invalid_argument("Invalid status");
        }
      }

      const std::uint8_t &
        operator[](kernel::PersistentStatusesT status) const
      {
        using kernel::PersistentStatusesT;
        switch (status) {
        case PersistentStatusesT::death:
          return Death;
        case PersistentStatusesT::poison:
          return Poison;
        case PersistentStatusesT::petrify:
          return Petrify;
        case PersistentStatusesT::darkness:
          return Darkness;
        case PersistentStatusesT::silence:
          return Silence;
        case PersistentStatusesT::berserk:
          return Berserk;
        case PersistentStatusesT::zombie:
          return Zombie;
        // ... (Other cases)
        default:
          throw std::invalid_argument("Invalid status");
        }
      }

      const std::uint8_t &
        operator[](kernel::BattleOnlyStatusesT status) const
      {
        using kernel::BattleOnlyStatusesT;
        switch (status) {
        case BattleOnlyStatusesT::sleep:
          return Sleep;
        case BattleOnlyStatusesT::haste:
          return Haste;
        case BattleOnlyStatusesT::slow:
          return Slow;
        case BattleOnlyStatusesT::stop:
          return Stop;
        case BattleOnlyStatusesT::regen:
          return Regen;
        case BattleOnlyStatusesT::reflect:
          return Reflect;
        case BattleOnlyStatusesT::doom:
          return Doom;
        case BattleOnlyStatusesT::petrifying:
          return SlowPetrify;
        case BattleOnlyStatusesT::floating:
          return Float;
        case BattleOnlyStatusesT::confuse:
          return Confuse;
        case BattleOnlyStatusesT::eject:
          return Expulsion;
        // case BattleOnlyStatusesT::???: return Percent;
        //  ... (Other cases)
        default:
          throw std::invalid_argument("Invalid status");
        }
      }
    };
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
    UnkFlags                    unknown_flags1;///< Unknown flags 1.
    StatusFlags                 status_flags;  ///< Character's status flags.
    std::array<CardID, 3>       cards;         ///< Character's cards.
    std::array<std::uint8_t, 3> devour;        ///< Character's devour ability.
    StatusFlags2         status_flags2;///< Character's additional status flags.
    UnkFlags             unknown_flags2;///< Unknown flags 2.
    std::int16_t         extra_exp;     ///< Character's extra experience.
    std::int16_t         exp;           ///< Character's experience.
    std::array<Magic, 4> draw_low;      ///< Character's low-level draw magic.
    std::array<Magic, 4> draw_med; ///< Character's medium-level draw magic.
    std::array<Magic, 4> draw_high;///< Character's high-level draw magic.
    std::array<Item, 4>  mug_low;  ///< Character's low-level mug items.
    std::array<Item, 4>  mug_med;  ///< Character's medium-level mug items.
    std::array<Item, 4>  mug_high; ///< Character's high-level mug items.
    std::array<Item, 4>  drop_low; ///< Character's low-level drop items.
    std::array<Item, 4>  drop_med; ///< Character's medium-level drop items.
    std::array<Item, 4>  drop_high;///< Character's high-level drop items.
    std::uint8_t         mug_rate; ///< Character's mug rate.
    std::uint8_t         drop_rate;///< Character's drop rate.
    std::uint8_t         padding;  ///< Padding byte.
    std::uint8_t         aps;      ///< Character's ability points.
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

  DatHeader m_header{};///< The header data of the .dat file.

  /**
   * @brief Contains information and statistics about a character in the game.
   */
  Section7_Information_and_Stats m_section7{};

public:
  /**
   * @brief Default constructor for DatFile.
   */
  DatFile() = default;

  /**
   * @brief Constructs a DatFile with the given buffer and file path.
   *
   * @param in_buffer Input buffer containing the file data.
   * @param path File path of the .dat file.
   */
  DatFile(std::vector<char> in_buffer, std::filesystem::path path)
    : m_buffer(std::move(in_buffer)), m_path(std::move(path)),
      m_header(m_buffer)
  {
    if (m_header.m_count == 2) {
      m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(
        std::span(m_buffer).subspan(m_header.m_offsets.front()));
      return;
    }
    m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(
      std::span(m_buffer).subspan(m_header.m_offsets[6]));
  }

  /**
   * @brief Get a const reference to the Section7_Information_and_Stats object.
   *
   * @return Const reference to the Section7_Information_and_Stats object.
   */
  [[nodiscard]] const Section7_Information_and_Stats &
    section_7() const
  {
    return m_section7;
  }

  /**
   * @brief Get a non-const reference to the Section7_Information_and_Stats
   * object.
   *
   * @return Non-const reference to the Section7_Information_and_Stats object.
   */
  Section7_Information_and_Stats &
    section_7()
  {
    return m_section7;
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_DATFILE_HPP
