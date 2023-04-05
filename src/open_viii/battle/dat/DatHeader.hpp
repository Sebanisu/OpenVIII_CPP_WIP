//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_DATHEADER_HPP
#define OPENVIII_CPP_WIP_DATHEADER_HPP
#include <cstdint>
#include <vector>
namespace open_viii::battle::dat {
/**
 * @brief Header structure for the DAT file.
 */
struct DatHeader
{
  /**
   * @brief Number of sections in the DAT file. Typically 11 or 2.
   */
  std::uint32_t              m_count{};///< 11 or 2;
  std::vector<std::uint32_t> m_offsets{};
  DatHeader() = default;

  /**
   * @brief Offsets of each section in the DAT file.
   */
  explicit DatHeader(std::span<const char> &buffer)
    : m_count(open_viii::tools::read_val<std::uint32_t>(buffer)),
      m_offsets(open_viii::tools::read_vals<std::uint32_t>(buffer, m_count))
  {
    // if count == 2 then only Section 7 and 8 exist.
  }

  /**
   * @brief Offsets of each section in the DAT file.
   */
  explicit DatHeader(const std::span<const char> &buffer)
    : m_count(open_viii::tools::read_val<std::uint32_t>(buffer)),
      m_offsets(
        (buffer.size() > sizeof(std::uint32_t))
          ? (open_viii::tools::read_vals<std::uint32_t>(
            buffer.subspan(sizeof(std::uint32_t)),
            m_count))
          : std::vector<std::uint32_t>{})
  {
    // if count == 2 then only Section 7 and 8 exist.
  }
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_DATHEADER_HPP
