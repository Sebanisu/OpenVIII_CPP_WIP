//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_TIMIMAGEHEADER_H
#define VIIIARCHIVE_TIMIMAGEHEADER_H
#include "OpenVIII/Graphics/rectangle.h"
#include <cstdint>
namespace OpenVIII::Graphics {
/**
 * Base Header for the following raw 16bit color image data.
 */
struct timImageHeader
{
private:
  std::uint32_t size_{};
  rectangle<std::uint16_t> rectangle_{};

public:
  /**
   * Total Header and Data size
   * @brief size in bytes
   */
  auto size() const { return size_; }
  /**
   * Dimensions of data.
   */
  auto Rectangle() const { return rectangle_; }


  /**
   * Total size of Color Lookup Table data without header.
   * @brief Size in bytes.
   */
  [[nodiscard]] auto dataSize() const { return size_ - sizeof(timImageHeader); };

  friend std::ostream &operator<<(std::ostream &os, const timImageHeader &input)
  {
    //[[maybe_unused]]static constexpr auto sizev_ = sizeof(input);
    return os << '{' << input.size() << " bytes, " << input.rectangle_ << '}';
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TIMIMAGEHEADER_H
