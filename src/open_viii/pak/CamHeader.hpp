//
// Created by pcvii on 12/31/2020.
//

#ifndef VIIIARCHIVE_CAMHEADER_HPP
#define VIIIARCHIVE_CAMHEADER_HPP

#include <algorithm>
#include <array>
#include <cstdint>
#include <string_view>
namespace open_viii {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_PAK#CAM_files
 */
struct CamHeader
{
private:
  std::array<char, 3U> m_start{ 'F', '8', 'P' };
  std::array<char, 3U> m_unk{};
  std::uint16_t m_count{};

public:
  CamHeader() = default;
  [[nodiscard]] constexpr start() const noexcept
  {
    return m_start;
  }

  [[nodiscard]] constexpr unk() const noexcept
  {
    return m_unk;
  }

  [[nodiscard]] constexpr count() const noexcept
  {
    return m_count;
  }
  constexpr static std::string_view EXPECTED_START{ "F8U" };
  constexpr static auto EXPECTED_SIZE{ 8U };
};
static_assert(
  std::ranges::equal(CamHeader().start(), CamHeader::EXPECTED_START));
static_assert(sizeof(CamHeader) == CamHeader::EXPECTED_SIZE));
}// namespace open_viii
#endif// VIIIARCHIVE_CAMHEADER_HPP
