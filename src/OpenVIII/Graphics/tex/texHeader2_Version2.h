//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXHEADER2_VERSION2_H
#define VIIIARCHIVE_TEXHEADER2_VERSION2_H
#include <cstdint>

namespace OpenVIII::Graphics {

/**
 * This appends to texHeader2 if the file is version 2. used by ff8 only.
 */
struct texHeader2_Version2
{
  /**
   * Unknown 11 (TEX version 2 only)
   */
  std::uint32_t UNKNOWN11{};
};


}

#endif// VIIIARCHIVE_TEXHEADER2_VERSION2_H
