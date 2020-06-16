//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_ATTACKFLAGST_H
#define VIIIARCHIVE_ATTACKFLAGST_H

#include <cstdint>
namespace OpenVIII::Kernel {
enum class AttackFlagsT : std::uint8_t {
  None = 0x0,
  Shelled = 0x1,
  Unk0X2 = 0x2,
  Unk0X4 = 0x4,
  BreakDamageLimit = 0x8,
  Reflected = 0x10,
  Unk0X20 = 0x20,
  Unk0X40 = 0x40,
  Revive = 0x80
};
}
#endif// VIIIARCHIVE_ATTACKFLAGST_H
