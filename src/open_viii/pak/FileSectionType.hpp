//
// Created by pcvii on 10/30/2020.
//

#ifndef VIIIARCHIVE_FILESECTIONTYPE_HPP
#define VIIIARCHIVE_FILESECTIONTYPE_HPP
#include <cinttypes>
#include <string_view>

namespace open_viii {
struct FileSectionTypeT{
public:
  constexpr static auto NONE = std::string_view("");
  /**
   * Cam file
   * @remark F8P
   */
  constexpr static auto CAM = std::string_view("F8P");
  /**
   * Bink Video
   */

  constexpr static auto BIK = std::string_view("BIK");
  /**
   * Bink Video Version 2.
   */
  constexpr static auto KB2 = std::string_view("KB2");
};
}
#endif// VIIIARCHIVE_FILESECTIONTYPE_HPP
