//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_TIMHEADER_H
#define VIIIARCHIVE_TIMHEADER_H
#include <cstdint>
#include <iostream>
#include "BPPT.h"
namespace open_viii::graphics {
/**
 * @struct open_viii::graphics::timHeader
 * @ingroup Graphics
 * @brief Color Lookup Table Header
 */
struct timHeader
{
private:
  /**
   * @brief expected value of tag_ is 0x10.
   */
  static constexpr auto tagVal_ = 0x10;
  /**
   * @brief expected value of version_ is 0x0.
   */
  static constexpr auto versionVal_ = 0x0;
  /**
   * @brief tag_ is first byte of a TIM file.
   */
  std::uint8_t tag_{};
  /**
   * @brief version_ is second byte of a TIM file.
   */
  std::uint8_t version_{};

  /**
   * @brief value should be 0.
   */
  std::uint8_t none0_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none1_{};

  /**
   * @brief bits per pixel flags and CLP flag
   */
  BPPT bpp_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none2_{};
  /**
   * @brief value should be 0.
   */
  std::uint8_t none3_{};
  /**
   * @brief value should be 0 or 22
   */
  std::uint8_t none4_{};

public:
  /**
   * @return bits per pixel flags and CLP flag
   */
  [[nodiscard]] auto BPP() const { return bpp_; }
  /**
   * All none values should be 0. version_ should be versionVal_. tag_ should be tagVal_. Only valid values for bpp are
   * 4bpp with CLP, 8bpp with CLP, 16bpp, or 24 bpp.
   * @brief Test member variables for valid values.
   * @return true if Check test passes.
   */
  [[nodiscard]] bool Check() const
  {
    return none0_ == 0 && none1_ == 0 && none2_ == 0 && none3_ == 0 && /*(none4_ == 0 || none4_ == 22) &&*/ tag_ == tagVal_
           && version_ == versionVal_ && bpp_.Check();
  }

  friend std::ostream &operator<<(std::ostream &os, const timHeader &input)
  {
    //[[maybe_unused]]static constexpr auto size_ = sizeof(input);
    return os << "{ Tag: " << static_cast<uint32_t>(input.tag_)
              << ", Version: " << static_cast<uint32_t>(input.version_) << ", " << input.bpp_ << '}';
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TIMHEADER_H
