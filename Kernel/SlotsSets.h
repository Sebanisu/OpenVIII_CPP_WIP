//
// Created by pcvii on 6/24/2020.
//

#ifndef VIIIARCHIVE_SLOTSSETS_H
#define VIIIARCHIVE_SLOTSSETS_H
#include "../Strings/EncodedStringOffset.h"
#include <cstdint>
#include <array>
namespace OpenVIII::Kernel {
struct Slot
{
private:
  std::uint8_t magicID_{};
  std::uint8_t count_{};

public:
  [[nodiscard]] auto MagicID() const noexcept { return magicID_; }
  [[nodiscard]] auto Count() const noexcept { return count_; }
  friend std::ostream &operator<<(std::ostream &os, const Slot &slot)
  {
    return os << '{' << static_cast<uint16_t>(slot.MagicID()) << ", " << static_cast<uint16_t>(slot.Count()) << '}';
  }
};
struct SlotsSets
{
  /*
   * https://github.com/DarkShinryu/doomtrain/wiki/Selphie-limit-break-sets
   * array of 16 bytes 8 total 2 bytes per Magic Slot { Magic ID and Count }
   */
private:
  static constexpr auto total_ = 8U;
  std::array<Slot, total_> slots_{};

public:
  [[nodiscard]] const auto &Slots() const noexcept { return slots_; }

  friend std::ostream &operator<<(std::ostream &os, const SlotsSets &set) { return set.Out(os, {}); }
  std::ostream &Out(std::ostream &os, [[maybe_unused]] const std::string_view &buffer) const
  {

    os << '{';
    bool first = true;
    for (const auto &item : Slots()) {
      if (!first) {
        os << ", ";
      } else {
        first = false;
      }
      os << item;
    }
    return os << '}';
  }
};
}// namespace OpenVIII::Kernel
#endif// VIIIARCHIVE_SLOTSSETS_H
