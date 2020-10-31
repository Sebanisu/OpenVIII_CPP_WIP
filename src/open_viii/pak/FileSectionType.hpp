//
// Created by pcvii on 10/30/2020.
//

#ifndef VIIIARCHIVE_FILESECTIONTYPE_HPP
#define VIIIARCHIVE_FILESECTIONTYPE_HPP
#include <cinttypes>

namespace open_viii {
enum struct FileSectionTypeT : std::uint32_t {
  none = 0,
  /**
   * Cam file
   * @remark F8P
   */
  cam = 0x503846,
  cam2 = 0x463850,
  /**
   * Bink Video
   */
  bik = 0x4B4942,
  /**
   * Bink Video Version 2.
   */
  kb2 = 0X32424B,
  /**
   * 3 Byte Mask to take uint and extract only the part we need.
   */
  mask_3b = 0xFFFFFF
};
}
#endif// VIIIARCHIVE_FILESECTIONTYPE_HPP
