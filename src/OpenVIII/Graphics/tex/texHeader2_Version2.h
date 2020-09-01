//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXHEADER2_VERSION2_H
#define VIIIARCHIVE_TEXHEADER2_VERSION2_H
#include <cstdint>

namespace open_viii::graphics {

/**
 * This appends to texHeader2 if the file is version 2. used by ff8 only.
 */
struct TexHeader2Version2
{
private:
  /**
   * Unknown 11 (TEX version 2 only)
   */
  std::uint32_t m_unknown_11{};

public:

  /**
   * Unknown 11 (TEX version 2 only)
   */
  [[maybe_unused]] [[nodiscard]] std::uint32_t unknown_11() const noexcept
  {
    return m_unknown_11;
  }
};


}// namespace open_viii::graphics

#endif// VIIIARCHIVE_TEXHEADER2_VERSION2_H
