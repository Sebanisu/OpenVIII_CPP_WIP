//
// Created by pcvii on 8/11/2020.
//

#ifndef VIIIARCHIVE_ZELLDUELMOVEST_H
#define VIIIARCHIVE_ZELLDUELMOVEST_H

namespace OpenVIII::Kernel {
enum struct ZellDuelMoveT : std::uint8_t {
  // https://github.com/DarkShinryu/doomtrain/wiki/Zell's-Duel-Moves
  // Offset	Description
  //  0x00	Punch Rush
  //  0x01	Booya
  //  0x02	Heel Drop
  //  0x03	Mach Kick
  //  0x04	Dolphin Blow
  //  0x05	Meteor Strike
  //  0x06	Burning Rave
  //  0x07	Meteor Barret
  //  0x08	Different Beat
  //  0x09	My Final Heaven

  PunchRush,
  Booya,
  HeelDrop,
  MachKick,
  DolphinBlow,
  MeteorStrike,
  BurningRave,
  MeteorBarret,
  DifferentBeat,
  MyFinalHeaven
};
}
#endif// VIIIARCHIVE_ZELLDUELMOVEST_H
