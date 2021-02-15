//
// Created by pcvii on 1/18/2021.
//
#ifndef VIIIARCHIVE_JSMHEADER_HPP
#define VIIIARCHIVE_JSMHEADER_HPP
#include "JsmEntity.hpp"
#include "JsmOldEntity.hpp"
#include "JsmScriptEntity.hpp"
#include <cstdint>
#include <iostream>
namespace open_viii::field::scripts {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_JSM
 */
struct JsmHeader
{
private:
  /**
   * Count of door entity
   */
  std::uint8_t m_count_door_entities{};
  /**
   * Count of walkmesh line entity
   */
  std::uint8_t m_count_walk_mesh_line_entities{};
  /**
   * Count of background entity
   */
  std::uint8_t m_count_background_entities{};
  /**
   * Count of other entity
   */
  std::uint8_t m_count_other_entities{};
  /**
   * Offset section 1
   */
  std::uint16_t m_offset_section_1{};
  /**
   * Offset script data
   */
  std::uint16_t m_offset_script_data{};

public:
  constexpr JsmHeader() = default;
  constexpr JsmHeader(std::uint8_t  in_count_door_entities,
                      std::uint8_t  in_count_walk_mesh_line_entities,
                      std::uint8_t  in_count_background_entities,
                      std::uint8_t  in_count_other_entities,
                      std::uint16_t in_offset_section_1,
                      std::uint16_t in_offset_script_data)
    : m_count_door_entities(in_count_door_entities),
      m_count_walk_mesh_line_entities(in_count_walk_mesh_line_entities),
      m_count_background_entities(in_count_background_entities),
      m_count_other_entities(in_count_other_entities),
      m_offset_section_1(in_offset_section_1),
      m_offset_script_data(in_offset_script_data)
  {}
  [[nodiscard]] std::size_t constexpr total_count() const noexcept
  {
    return static_cast<std::size_t>(
      count_door_entities() + count_walk_mesh_line_entities()
      + count_background_entities() + count_other_entities());
  }
  [[nodiscard]] std::size_t constexpr expected_count() const noexcept
  {
    return (offset_section_1() - EXPECTED_SIZE) / sizeof(JsmEntity);
  };
  [[nodiscard]] bool constexpr valid_count() const noexcept
  {
    return expected_count() == total_count();
  };
  /**
   * Count of door entity
   */
  [[nodiscard]] std::uint8_t constexpr count_door_entities() const noexcept
  {
    return m_count_door_entities;
  }
  /**
   * offset of door entity
   */
  [[nodiscard]] auto constexpr offset_door_entities() const noexcept
  {
    return EXPECTED_SIZE;
  }
  /**
   * Count of walk mesh line entity
   */
  [[nodiscard]] std::uint8_t constexpr count_walk_mesh_line_entities()
    const noexcept
  {
    return m_count_walk_mesh_line_entities;
  }
  /**
   * offset of walk mesh line entity
   */
  [[nodiscard]] auto constexpr offset_walk_mesh_line_entities() const noexcept
  {
    return offset_door_entities() + sizeof(JsmEntity) * count_door_entities();
  }
  /**
   * Count of background entity
   */
  [[nodiscard]] std::uint8_t constexpr count_background_entities()
    const noexcept
  {
    return m_count_background_entities;
  }
  /**
   * offset of background entity
   */
  [[nodiscard]] auto constexpr offset_background_entities() const noexcept
  {
    return offset_walk_mesh_line_entities()
           + sizeof(JsmEntity) * count_walk_mesh_line_entities();
  }
  /**
   * Count of other entity
   */
  [[nodiscard]] std::uint8_t constexpr count_other_entities() const noexcept
  {
    return m_count_other_entities;
  }
  /**
   * offset of other entity
   */
  [[nodiscard]] auto constexpr offset_other_entities() const noexcept
  {
    return offset_background_entities()
           + sizeof(JsmEntity) * count_background_entities();
  }
  /**
   * count section 1
   */
  [[nodiscard]] constexpr auto count_section_1() const noexcept
  {
    return (offset_script_data() - offset_section_1())
           / sizeof(JsmScriptEntity);
  }
  /**
   * Offset section 1
   */
  [[nodiscard]] std::uint16_t constexpr offset_section_1() const noexcept
  {
    return m_offset_section_1;
  }
  /**
   * Offset script data
   */
  [[nodiscard]] std::uint16_t constexpr offset_script_data() const noexcept
  {
    return m_offset_script_data;
  }
  static constexpr std::size_t EXPECTED_SIZE = 8U;
};
static_assert(sizeof(JsmHeader) == JsmHeader::EXPECTED_SIZE);
/**
 * if count is > expected_count it can't be valid so any count greater is set to
 * 0.
 * @param in read in header data.
 * @return possible valid header.
 */
static constexpr JsmHeader fix_jsm_header_counts(JsmHeader in) noexcept
{
  const auto expected_count = in.expected_count();
  const auto fix = [&expected_count]<std::unsigned_integral T>(T count) -> T {
    // std::cout << expected_count << "\t<\t" <<
    // static_cast<std::size_t>(count)<<std::endl; // if we remove this line it
    // can be constexpr
    if (expected_count < count) {
      return static_cast<T>(0U);
    }
    return count;
  };
  return JsmHeader(fix(in.count_door_entities()),
                   fix(in.count_walk_mesh_line_entities()),
                   fix(in.count_background_entities()),
                   fix(in.count_other_entities()),
                   in.offset_section_1(),
                   in.offset_script_data());
}
}// namespace open_viii::field::scripts
#endif// VIIIARCHIVE_JSMHEADER_HPP
