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
#ifndef VIIIARCHIVE_ITEMIDT_HPP
#define VIIIARCHIVE_ITEMIDT_HPP
#include "cstdint"
namespace open_viii {
enum struct [[maybe_unused]] ItemIdT : std::uint8_t{
  none              = 0x00,
  potion            = 0x01,
  potion_plus       = 0x02,
  hi_potion         = 0x03,
  hi_potion_plus    = 0x04,
  x_potion          = 0x05,
  mega_potion       = 0x06,
  phoenix_down      = 0x07,
  mega_phoenix      = 0x08,
  elixir            = 0x09,
  megalixir         = 0x0A,
  antidote          = 0x0B,
  soft              = 0x0C,
  eye_drops         = 0x0D,
  echo_screen       = 0x0E,
  holy_water        = 0x0F,
  remedy            = 0x10,
  remedy_plus       = 0x11,
  hero_trial        = 0x12,
  hero              = 0x13,
  holy_war_trial    = 0x14,
  holy_war          = 0x15,
  shell_stone       = 0x16,
  protect_stone     = 0x17,
  aura_stone        = 0x18,
  death_stone       = 0x19,
  holy_stone        = 0x1A,
  flare_stone       = 0x1B,
  meteor_stone      = 0x1C,
  ultima_stone      = 0x1D,
  gysahl_greens     = 0x1E,
  phoenix_pinion    = 0x1F,
  friendship        = 0x20,
  tent              = 0x21,
  pet_house         = 0x22,
  cottage           = 0x23,
  g_potion          = 0x24,
  g_hi_potion       = 0x25,
  g_mega_potion     = 0x26,
  g_returner        = 0x27,
  rename_card       = 0x28,
  amnesia_greens    = 0x29,
  hp_j_scroll       = 0x2A,
  str_j_scroll      = 0x2B,
  vit_j_scroll      = 0x2C,
  mag_j_scroll      = 0x2D,
  spr_j_scroll      = 0x2E,
  spd_j_scroll      = 0x2F,
  luck_j_scroll     = 0x30,
  aegis_amulet      = 0x31,
  elem_atk          = 0x32,
  elem_guard        = 0x33,
  status_atk        = 0x34,
  status_guard      = 0x35,
  rosetta_stone     = 0x36,
  magic_scroll      = 0x37,
  gf_scroll         = 0x38,
  draw_scroll       = 0x39,
  item_scroll       = 0x3A,
  gambler_spirit    = 0x3B,
  healing_ring      = 0x3C,
  phoenix_spirit    = 0x3D,
  med_kit           = 0x3E,
  bomb_spirit       = 0x3F,
  hungry_cookpot    = 0x40,
  mog_s_amulet      = 0x41,
  steel_pipe        = 0x42,
  star_fragment     = 0x43,
  energy_crystal    = 0x44,
  samantha_soul     = 0x45,
  healing_mail      = 0x46,
  silver_mail       = 0x47,
  gold_armor        = 0x48,
  diamond_armor     = 0x49,
  regen_ring        = 0x4A,
  giant_s_ring      = 0x4B,
  gaea_s_ring       = 0x4C,
  strength_love     = 0x4D,
  power_wrist       = 0x4E,
  hyper_wrist       = 0x4F,
  turtle_shell      = 0x50,
  orichalcum        = 0x51,
  adamantine        = 0x52,
  rune_armlet       = 0x53,
  force_armlet      = 0x54,
  magic_armlet      = 0x55,
  circlet           = 0x56,
  hypno_crown       = 0x57,
  royal_crown       = 0x58,
  jet_engine        = 0x59,
  rocket_engine     = 0x5A,
  moon_curtain      = 0x5B,
  steel_curtain     = 0x5C,
  glow_curtain      = 0x5D,
  accelerator       = 0x5E,
  monk_s_code       = 0x5F,
  knight_s_code     = 0x60,
  doc_s_code        = 0x61,
  hundred_needles   = 0x62,
  three_stars       = 0x63,
  ribbon            = 0x64,
  normal_ammo       = 0x65,
  shotgun_ammo      = 0x66,
  dark_ammo         = 0x67,
  fire_ammo         = 0x68,
  demolition_ammo   = 0x69,
  fast_ammo         = 0x6A,
  ap_ammo           = 0x6B,
  pulse_ammo        = 0x6C,
  m_stone_piece     = 0x6D,
  magic_stone       = 0x6E,
  wizard_stone      = 0x6F,
  ochu_tentacle     = 0x70,
  healing_water     = 0x71,
  cockatrice_pinion = 0x72,
  zombie_powder     = 0x73,
  lightweight       = 0x74,
  sharp_spike       = 0x75,
  screw             = 0x76,
  saw_blade         = 0x77,
  mesmerize_blade   = 0x78,
  vampire_fang      = 0x79,
  fury_fragment     = 0x7A,
  betrayal_sword    = 0x7B,
  sleep_powder      = 0x7C,
  life_ring         = 0x7D,
  dragon_fang       = 0x7E,
  spider_web        = 0x7F,
  coral_fragment    = 0x80,
  curse_spike       = 0x81,
  black_hole        = 0x82,
  water_crystal     = 0x83,
  missile           = 0x84,
  mystery_fluid     = 0x85,
  running_fire      = 0x86,
  inferno_fang      = 0x87,
  malboro_tentacle  = 0x88,
  whisper           = 0x89,
  laser_cannon      = 0x8A,
  barrier           = 0x8B,
  power_generator   = 0x8C,
  dark_matter       = 0x8D,
  bomb_fragment     = 0x8E,
  red_fang          = 0x8F,
  arctic_wind       = 0x90,
  north_wind        = 0x91,
  dynamo_stone      = 0x92,
  shear_feather     = 0x93,
  venom_fang        = 0x94,
  steel_orb         = 0x95,
  moon_stone        = 0x96,
  dino_bone         = 0x97,
  windmill          = 0x98,
  dragon_skin       = 0x99,
  fish_fin          = 0x9A,
  dragon_fin        = 0x9B,
  silence_powder    = 0x9C,
  poison_powder     = 0x9D,
  dead_spirit       = 0x9E,
  chef_s_knife      = 0x9F,
  cactus_thorn      = 0xA0,
  shaman_stone      = 0xA1,
  fuel              = 0xA2,
  girl_next_door    = 0xA3,
  sorceress_letter  = 0xA4,
  chocobo_s_tag     = 0xA5,
  pet_nametag       = 0xA6,
  solomon_ring      = 0xA7,
  magical_lamp      = 0xA8,
  hp_up             = 0xA9,
  str_up            = 0xAA,
  vit_up            = 0xAB,
  mag_up            = 0xAC,
  spr_up            = 0xAD,
  spd_up            = 0xAE,
  luck_up           = 0xAF,
  luv_luv_g         = 0xB0,
  weapons_mon_1st   = 0xB1,
  weapons_mon_mar   = 0xB2,
  weapons_mon_apr   = 0xB3,
  weapons_mon_may   = 0xB4,
  weapons_mon_jun   = 0xB5,
  weapons_mon_jul   = 0xB6,
  weapons_mon_aug   = 0xB7,
  combat_king_1     = 0xB8,
  combat_king_2     = 0xB9,
  combat_king_3     = 0xBA,
  combat_king_4     = 0xBB,
  combat_king_5     = 0xBC,
  pet_pals_vol_1    = 0xBD,
  pet_pals_vol_2    = 0xBE,
  pet_pals_vol_3    = 0xBF,
  pet_pals_vol_4    = 0xC0,
  pet_pals_vol_5    = 0xC1,
  pet_pals_vol_6    = 0xC2,
  occult_fan_i      = 0xC3,
  occult_fan_ii     = 0xC4,
  occult_fan_iii    = 0xC5,
  occult_fan_iv     = 0xC6,
};
}
#endif// VIIIARCHIVE_ITEMIDT_HPP
