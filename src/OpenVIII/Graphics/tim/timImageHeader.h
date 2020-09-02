//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_TIMIMAGEHEADER_H
#define VIIIARCHIVE_TIMIMAGEHEADER_H
#include "OpenVIII/Graphics/Rectangle.h"
#include <cstdint>
namespace open_viii::graphics {
/**
 * Base Header for the following raw 16bit color image data.
 */
struct TimImageHeader
{
private:
  std::uint32_t m_size{};
  Rectangle<std::uint16_t> m_rectangle{};

public:
  /**
   * Total Header and Data size
   * @brief size in bytes
   */
  auto size() const { return m_size; }
  /**
   * Dimensions of data.
   */
  auto rectangle() const { return m_rectangle; }


  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto data_size() const { return m_size - sizeof(TimImageHeader); };

  friend std::ostream &operator<<(std::ostream &os, const TimImageHeader &input)
  {
    return os << '{' << input.size() << " bytes, " << input.m_rectangle << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMIMAGEHEADER_H
