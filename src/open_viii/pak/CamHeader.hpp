//
// Created by pcvii on 12/31/2020.
//
#ifndef VIIIARCHIVE_CAMHEADER_HPP
#define VIIIARCHIVE_CAMHEADER_HPP
namespace open_viii::pak {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_PAK#CAM_files
 */
struct CamHeader
{
private:
  std::array<char, 3U> m_start{ 'F', '8', 'P' };
  std::array<char, 3U> m_unk{};
  std::uint16_t        m_count{};

public:
  constexpr CamHeader() = default;
  /**
   * Get Start "F8P" 3 bytes
   */
  [[nodiscard]] constexpr auto
    start() const noexcept
  {
    return m_start;
  }
  /**
   * Set Start "F8P" 3 bytes
   */
  [[nodiscard]] constexpr auto
    with_start(std::array<char, 3U> in_start) const noexcept
  {
    auto ret    = *this;
    ret.m_start = in_start;
    return ret;
  }
  /**
   * Get Unknown 3 bytes
   */
  [[nodiscard]] constexpr auto
    unk() const noexcept
  {
    return m_unk;
  }
  /**
   * Set Unknown 3 bytes
   */
  [[nodiscard]] constexpr auto
    with_unk(std::array<char, 3U> in_unk) const noexcept
  {
    auto ret  = *this;
    ret.m_unk = in_unk;
    return ret;
  }
  /**
   * Get Count of frames. Sometimes there are extra frames in the file.
   */
  [[nodiscard]] constexpr auto
    count() const noexcept
  {
    return m_count;
  }
  /**
   * Set Count of frames.
   */
  [[nodiscard]] constexpr auto
    with_count(std::uint16_t in_count) const noexcept
  {
    auto ret    = *this;
    ret.m_count = in_count;
    return ret;
  }
  /**
   * Expected "F8P" start marker
   */
  constexpr static std::string_view EXPECTED_START{ "F8P" };
  /**
   * verify start() == "F8P"
   */
  [[nodiscard]] constexpr bool
    valid_start() const noexcept
  {
    return std::ranges::equal(m_start, EXPECTED_START);
  }
  /**
   * Expected Size of struct in bytes
   */
  constexpr static auto EXPECTED_SIZE{ 8U };
  friend std::ostream &
    operator<<(std::ostream &os, const CamHeader &cam_header)
  {
    return os << '{' << cam_header.m_start[0] << cam_header.m_start[1]
              << cam_header.m_start[2] << ',' << cam_header.m_unk[0]
              << cam_header.m_unk[1] << cam_header.m_unk[2] << ','
              << cam_header.m_count << '}';
  }
};
static_assert(CamHeader().valid_start());
static_assert(std::is_trivially_copyable_v<CamHeader>);
static_assert(std::is_default_constructible_v<CamHeader>);
static_assert(sizeof(CamHeader) == CamHeader::EXPECTED_SIZE);
}// namespace open_viii::pak
#endif// VIIIARCHIVE_CAMHEADER_HPP
