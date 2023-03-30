//
// Created by pcvii on 3/30/2023.
//

#ifndef OPENVIII_CPP_WIP_DATFILE_HPP
#define OPENVIII_CPP_WIP_DATFILE_HPP
#include "open_viii/CardID.hpp"
#include "open_viii/GFID.hpp"
#include "open_viii/strings/FF8String.hpp"
#include "open_viii/tools/Read.hpp"
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>
namespace open_viii::battle {
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
  struct Item
  {
    uint8_t ID;
    uint8_t quantity;
  };
  struct Magic
  {
    uint8_t ID;
    uint8_t unk;
            operator GFID() noexcept
    {
      return ID > 0x4F || ID < 0x40 ? GFID::Blank
                                    : static_cast<GFID>(ID - 0x40);
    }
  };
  struct Section7_Information_and_Stats
  {
    enum class StatusFlags : std::uint8_t
    {
      None        = 0,
      Zombie      = 0x1,
      Fly         = 0x2,
      Zz1         = 0x4,
      Zz2         = 0x8,
      Zz3         = 0x10,
      AutoReflect = 0x20,
      AutoShell   = 0x40,
      AutoProtect = 0x80,
    };
    enum class StatusFlags2 : std::uint8_t
    {
      None          = 0,
      Zz1           = 0x1,
      Zz2           = 0x2,
      Unused1       = 0x4,
      Unused2       = 0x8,
      Unused3       = 0x10,
      Unused4       = 0x20,
      DiablosMissed = 0x40,
      AlwaysCard    = 0x80,
    };
    enum class UnkFlags : std::uint8_t
    {
      None = 0,
      Unk0 = 0x1,
      Unk1 = 0x2,
      Unk2 = 0x4,
      Unk3 = 0x8,
      Unk4 = 0x10,
      Unk5 = 0x20,
      Unk6 = 0x40,
      Unk7 = 0x80,
    };
    struct ElementalResist
    {
      // std::uint8_t NonElemental; not here.
      std::uint8_t Fire;
      std::uint8_t Ice;
      std::uint8_t Thunder;
      std::uint8_t Earth;
      std::uint8_t Poison;
      std::uint8_t Wind;
      std::uint8_t Water;
      std::uint8_t Holy;
    };
    struct MentalResist
    {
      std::uint8_t Death;
      std::uint8_t Poison;
      std::uint8_t Petrify;
      std::uint8_t Darkness;
      std::uint8_t Silence;
      std::uint8_t Berserk;
      std::uint8_t Zombie;
      std::uint8_t Sleep;
      std::uint8_t Haste;
      std::uint8_t Slow;
      std::uint8_t Stop;
      std::uint8_t Regen;
      std::uint8_t Reflect;
      std::uint8_t Doom;
      std::uint8_t SlowPetrify;
      std::uint8_t Float;
      std::uint8_t Confuse;
      std::uint8_t Drain;
      std::uint8_t Expulsion;
      std::uint8_t Percent;
    };
    std::array<char, 24>         encoded_name;
    std::int32_t                 hp;
    std::int32_t                 str;
    std::int32_t                 vit;
    std::int32_t                 mag;
    std::int32_t                 spr;
    std::int32_t                 spd;
    std::int32_t                 eva;
    std::array<std::int32_t, 16> abilities_low;
    std::array<std::int32_t, 16> abilities_med;
    std::array<std::int32_t, 16> abilities_high;
    std::uint8_t                 med_level_start;
    std::uint8_t                 high_level_start;
    UnkFlags                     unknown_flags1;
    StatusFlags                  status_flags;
    std::array<CardID, 3>        cards;
    std::array<std::uint8_t, 3>  devour;
    StatusFlags2                 status_flags2;
    UnkFlags                     unknown_flags2;
    std::int16_t                 extra_exp;
    std::int16_t                 exp;
    std::array<Magic, 4>         draw_low;
    std::array<Magic, 4>         draw_med;
    std::array<Magic, 4>         draw_high;
    std::array<Item, 4>          mug_low;
    std::array<Item, 4>          mug_med;
    std::array<Item, 4>          mug_high;
    std::array<Item, 4>          drop_low;
    std::array<Item, 4>          drop_med;
    std::array<Item, 4>          drop_high;
    std::uint8_t                 mug_rate;
    std::uint8_t                 drop_rate;
    std::uint8_t                 padding;
    std::uint8_t                 aps;
    std::array<std::uint8_t, 16> unknown;
    ElementalResist              elemental_resistance;
    MentalResist                 mental_resistance;
    template<LangT coo = LangT::en>
    [[nodiscard]] std::u8string
      u8name() const
    {
      return FF8String<coo>().decode(encoded_name);
    }

    template<LangT coo = LangT::en>
    [[nodiscard]] std::string
      name() const
    {
      return tools::u8_to_s(u8name<coo>());
    }
  };
  DatHeader                      m_header{};
  Section7_Information_and_Stats m_section7{};

public:
  DatFile() = default;
  DatFile(std::vector<char> in_buffer, std::filesystem::path path)
    : m_buffer(std::move(in_buffer)), m_path(std::move(path)),
      m_header(m_buffer)
  {
    if(m_header.m_count == 2)
    {
      m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(std::span(m_buffer).subspan(m_header.m_offsets.front()));
      return;
    }
    m_section7 = open_viii::tools::read_val<Section7_Information_and_Stats>(std::span(m_buffer).subspan(m_header.m_offsets[6]));
  }
  const Section7_Information_and_Stats &
    section_7() const
  {
    return m_section7;
  }
  Section7_Information_and_Stats &
    section_7()
  {
    return m_section7;
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_DATFILE_HPP
