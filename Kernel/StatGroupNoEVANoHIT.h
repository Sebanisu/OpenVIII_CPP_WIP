//
// Created by pcvii on 6/17/2020.
//

#ifndef VIIIARCHIVE_STATGROUPNOEVANOHIT_H
#define VIIIARCHIVE_STATGROUPNOEVANOHIT_H

namespace OpenVIII::Kernel {
template<typename T> struct StatGroupNoEVANoHIT
{
private:
  T HP_{};
  T STR_{};
  T VIT_{};
  T MAG_{};
  T SPR_{};
  T SPD_{};
  T LUCK_{};

public:
  [[nodiscard]] T &HP() const noexcept { return HP_; }
  [[nodiscard]] T &STR() const noexcept { return STR_; }
  [[nodiscard]] T &VIT() const noexcept { return VIT_; }
  [[nodiscard]] T &MAG() const noexcept { return MAG_; }
  [[nodiscard]] T &SPR() const noexcept { return SPR_; }
  [[nodiscard]] T &SPD() const noexcept { return SPD_; }
  [[nodiscard]] T &LUCK() const noexcept { return LUCK_; }
};
}
#endif// VIIIARCHIVE_STATGROUPNOEVANOHIT_H
