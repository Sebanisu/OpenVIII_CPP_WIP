//
// Created by pcvii on 3/27/2023.
//

#ifndef OPENVIII_CPP_WIP_RAWCLUT_HPP
#define OPENVIII_CPP_WIP_RAWCLUT_HPP
#include <bit>
#include <cstdint>
namespace open_viii::battle::stage {
class RawClut
{
private:
  std::uint16_t                  m_clut_raw{};
  static constexpr std::uint16_t MASK_4_BIT  = 0xFU;
  static constexpr std::int32_t  SHIFT_2_BIT = 2;

public:
  RawClut() = default;
  RawClut(std::uint16_t clut_raw) : m_clut_raw(clut_raw) {}

  // Overload the assignment operator
  RawClut &
    operator=(std::uint8_t value) noexcept
  {
    m_clut_raw
      = static_cast<std::uint16_t>(
          m_clut_raw
          & static_cast<std::uint16_t>(~std::rotr(MASK_4_BIT, SHIFT_2_BIT)))
      | std::rotr(static_cast<std::uint16_t>(value & MASK_4_BIT), SHIFT_2_BIT);

    return *this;
  }

  /**
   * @brief Calculate the CLUT from raw CLUT value. Implicit conversion to
   * std::uint8_t
   * @param in_clut_raw The raw CLUT value.
   * @return The calculated CLUT value.
   */
  operator std::uint8_t() const noexcept
  {
    return (std::rotl(m_clut_raw, SHIFT_2_BIT) & MASK_4_BIT);
  }
};
}// namespace open_viii::battle::stage
#endif// OPENVIII_CPP_WIP_RAWCLUT_HPP
