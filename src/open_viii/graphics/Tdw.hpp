// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_TDW_HPP
#define VIIIARCHIVE_TDW_HPP
#include "Bit4Values.hpp"
#include "Tim.hpp"
#include "Uint8Range.hpp"
namespace open_viii::graphics {
/**
 * @brief Stores bitmap font character widths for an included TIM image.
 *
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_TDW
 */
struct Tdw
{
private:
  /**
   * @brief The constant value used for checking the widths offset.
   */
  static constexpr auto   WIDTHS_OFFSET_VALUE = std::uint32_t{ 8U };

  /**
   * @brief The offset value for the character widths in the buffer.
   */
  std::uint32_t           m_widths_offset{};

  /**
   * @brief The offset value for the TIM image in the buffer.
   */
  std::uint32_t           m_tim_offset{};

  /**
   * @brief A vector of Bit4Values containing the character widths.
   */
  std::vector<Bit4Values> m_widths{};

  /**
   * @brief The TIM image associated with the character widths.
   */
  Tim                     m_tim{};

  /**
   * @brief Calculates the size of the bitmap font character widths in bytes.
   *
   * @return The size of the bitmap font character widths in bytes.
   */
  [[nodiscard]] auto
    widths_size() const noexcept
  {
    return m_tim_offset - m_widths_offset;
  }

public:
  /**
   * @brief Default constructor.
   */
  Tdw() = default;

  /**
   * @brief Constructs a Tdw object from a buffer.
   *
   * @param buffer The input buffer containing Tdw data.
   */
  explicit Tdw([[maybe_unused]] std::span<const char> buffer)
  {
    const auto                   buffer_bak = buffer;
    constexpr static std::size_t sz32       = sizeof(std::uint32_t);
    if (std::ranges::size(buffer) < sz32) {
      return;
    }
    std::memcpy(&m_widths_offset, std::ranges::data(buffer), sz32);
    buffer = buffer.subspan(sz32);
    if (WIDTHS_OFFSET_VALUE != m_widths_offset) {
      m_widths_offset = 0U;
      return;
    }
    if (std::ranges::size(buffer) < sz32) {
      return;
    }
    std::memcpy(&m_tim_offset, std::ranges::data(buffer), sz32);
    buffer = buffer.subspan(sz32);
    if (std::ranges::size(buffer) < widths_size()) {
      return;
    }
    if (widths_size() > 0) {
      m_widths.resize(widths_size());
      std::memcpy(
        std::ranges::data(m_widths),
        std::ranges::data(buffer_bak.subspan(m_widths_offset)),
        widths_size());
    }
    m_tim = Tim{ buffer_bak.subspan(m_tim_offset) };
  }

  /**
   * @brief Accesses the character width at the specified index.
   *
   * @param i The index of the character width.
   * @return The character width at the specified index.
   */
  [[nodiscard]] auto
    operator[](size_t i) const
  {
    bool first = (i % 2U == 0U);
    i /= 2U;
    if (first) {
      return m_widths.at(i).first;
    }
    return m_widths.at(i).second;
  }

  /**
   * @brief Returns the number of character widths stored.
   *
   * @return The number of character widths.
   */
  [[nodiscard]] auto
    size() const
  {
    return std::ranges::size(m_widths) * 2U;
  }

  /**
   * @brief Accesses the TIM image.
   *
   * @return A const reference to the TIM image.
   */
  [[nodiscard]] const Tim &
    tim() const
  {
    return m_tim;
  }

  /**
   * @brief Saves the TIM image.
   *
   * @tparam Ts Parameter pack for the arguments to be forwarded.
   * @param ts Arguments to be forwarded to the save function of the TIM image.
   */
  template<class... Ts>
  void
    save(Ts &&...ts)
  {
    if (m_tim.area() != 0U) {
      m_tim.save(std::forward<Ts>(ts)...);
    }
  }

  /**
   * @brief Accesses the vector of character widths.
   *
   * @return A const reference to the vector of character widths.
   */
  [[nodiscard]] Uint8Range<std::vector<Bit4Values>>
    widths() const noexcept
  {
    return m_widths;
  }
};

/**
 * @brief Outputs a Tdw object to an ostream.
 *
 * @param os The ostream to output to.
 * @param t The Tdw object to be outputted.
 * @return A reference to the ostream after outputting the Tdw object.
 */
inline std::ostream &
  operator<<(std::ostream &os, const Tdw &t)
{
  os << t.size() << " char widths: ";
  for (const auto &width : t.widths()) {
    os << +width << ", ";
  }
  return os << '\n' << t.tim();
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_TDW_HPP
