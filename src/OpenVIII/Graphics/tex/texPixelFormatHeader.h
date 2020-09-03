//
// Created by pcvii on 8/28/2020.
//

#ifndef VIIIARCHIVE_TEXPIXELFORMATHEADER_H
#define VIIIARCHIVE_TEXPIXELFORMATHEADER_H
#include <cstdint>

namespace open_viii::graphics {
/**
 * Pixel format header. This tells how the colors are laid out. These are usually always the same.
 */
struct TexPixelFormatHeader
{
private:
  /**
   * Number of Red Bits
   */
  std::uint32_t m_num_red_bits{};
  /**
   * Number of Green Bits
   */
  std::uint32_t m_num_green_bits{};
  /**
   * Number of Blue Bits
   */
  std::uint32_t m_num_blue_bits{};
  /**
   * Number of Alpha Bits
   */
  std::uint32_t m_num_alpha_bits{};
  /**
   * Red Bitmask
   */
  std::uint32_t m_red_bitmask{};
  /**
   * Green Bitmask
   */
  std::uint32_t m_green_bitmask{};
  /**
   * Blue Bitmask
   */
  std::uint32_t m_blue_bitmask{};
  /**
   * Alpha Bitmask
   */
  std::uint32_t m_alpha_bitmask{};
  /**
   * Red Shift
   */
  std::uint32_t m_red_shift{};
  /**
   * Green Shift
   */
  std::uint32_t m_green_shift{};
  /**
   * Blue Shift
   */
  std::uint32_t m_blue_shift{};
  /**
   * Alpha Shift
   */
  std::uint32_t m_alpha_shift{};
  /**
   * 8 _ Number of Red Bits (always ignored)
   */
  std::uint32_t m_minus_8_num_red_bits{};
  /**
   * 8 _ Number of Green Bits (always ignored)
   */
  std::uint32_t m_minus_8_num_green_bits{};
  /**
   * 8 _ Number of Blue Bits (always ignored)
   */
  std::uint32_t m_minus_8_num_blue_bits{};
  /**
   * 8 _ Number of Alpha Bits (always ignored)
   */
  std::uint32_t m_minus_8_num_alpha_bits{};
  /**
   * Red Max
   */
  std::uint32_t m_red_max{};
  /**
   * Green Max
   */
  std::uint32_t m_green_max{};
  /**
   * Blue Max
   */
  std::uint32_t m_blue_max{};
  /**
   * Alpha Max
   */
  std::uint32_t m_alpha_max{};

public:
  // Pixel Format
  /**
   * Number of Red Bits
   */
  [[maybe_unused]] [[nodiscard]] auto num_red_bits() const noexcept { return m_num_red_bits; }
  /**
   * Number of Green Bits
   */
  [[maybe_unused]] [[nodiscard]] auto num_green_bits() const noexcept { return m_num_green_bits; }
  /**
   * Number of Blue Bits
   */
  [[maybe_unused]] [[nodiscard]] auto num_blue_bits() const noexcept { return m_num_blue_bits; }
  /**
   * Number of Alpha Bits
   */
  [[maybe_unused]] [[nodiscard]] auto num_alpha_bits() const noexcept { return m_num_alpha_bits; }
  /**
   * Red Bitmask
   */
  [[maybe_unused]] [[nodiscard]] auto red_bitmask() const noexcept { return m_red_bitmask; }
  /**
   * Green Bitmask
   */
  [[maybe_unused]] [[nodiscard]] auto green_bitmask() const noexcept { return m_green_bitmask; }
  /**
   * Blue Bitmask
   */
  [[maybe_unused]] [[nodiscard]] auto blue_bitmask() const noexcept { return m_blue_bitmask; }
  /**
   * Alpha Bitmask
   */
  [[maybe_unused]] [[nodiscard]] auto alpha_bitmask() const noexcept { return m_alpha_bitmask; }
  /**
   * Red Shift
   */
  [[maybe_unused]] [[nodiscard]] auto red_shift() const noexcept { return m_red_shift; }
  /**
   * Green Shift
   */
  [[maybe_unused]] [[nodiscard]] auto green_shift() const noexcept { return m_green_shift; }
  /**
   * Blue Shift
   */
  [[maybe_unused]] [[nodiscard]] auto blue_shift() const noexcept { return m_blue_shift; }
  /**
   * Alpha Shift
   */
  [[nodiscard]] auto alpha_shift() const noexcept { return m_alpha_shift; }
  /**
   * 8 _ Number of Red Bits (always ignored)
   */
  [[nodiscard]] auto minus_8_num_red_bits() const noexcept { return m_minus_8_num_red_bits; }
  /**
   * 8 _ Number of Green Bits (always ignored)
   */
  [[nodiscard]] auto minus_8_num_green_bits() const noexcept { return m_minus_8_num_green_bits; }
  /**
   * 8 _ Number of Blue Bits (always ignored)
   */
  [[maybe_unused]] [[nodiscard]] auto minus_8_num_blue_bits() const noexcept { return m_minus_8_num_blue_bits; }
  /**
   * 8 _ Number of Alpha Bits (always ignored)
   */
  [[nodiscard]] auto minus_8_num_alpha_bits() const noexcept { return m_minus_8_num_alpha_bits; }
  /**
   * Red Max
   */
  [[maybe_unused]] [[nodiscard]] auto red_max() const noexcept { return m_red_max; }
  /**
   * Green Max
   */
  [[maybe_unused]] [[nodiscard]] auto green_max() const noexcept { return m_green_max; }
  /**
   * Blue Max
   */
  [[maybe_unused]] [[nodiscard]] auto blue_max() const noexcept { return m_blue_max; }
  /**
   * Alpha Max
   */
  [[maybe_unused]] [[nodiscard]] auto alpha_max() const noexcept { return m_alpha_max; }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TEXPIXELFORMATHEADER_H
