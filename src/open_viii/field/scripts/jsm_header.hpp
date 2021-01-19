//
// Created by pcvii on 1/18/2021.
//

#ifndef VIIIARCHIVE_JSM_HEADER_HPP
#define VIIIARCHIVE_JSM_HEADER_HPP
#include "jsm_entity.hpp"
#include "jsm_script_entity.hpp"
#include <cstdint>
namespace open_viii::field::scripts {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_JSM
 */
struct jsm_header
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
  constexpr jsm_header() = default;
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
    return offset_door_entities() + sizeof(jsm_entity) * count_door_entities();
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
           + sizeof(jsm_entity) * count_walk_mesh_line_entities();
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
           + sizeof(jsm_entity) * count_background_entities();
  }

  /**
   * count section 1
   */
  [[nodiscard]] constexpr auto count_section_1() const noexcept
  {
    return (offset_script_data() - offset_section_1())
           / sizeof(jsm_script_entity);
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
static_assert(sizeof(jsm_header) == jsm_header::EXPECTED_SIZE);
// static constexpr std::size_t size_of_entities(const jsm_header & header)
//{
//  return header.offset_script_data()-header.offset_section_1();
//}
// static constexpr std::size_t size_of_script_entities(const jsm_header &
// header)
//{
//  return header.offset_section_1()-sizeof(jsm_header);
//}

}// namespace open_viii::field::scripts
#endif// VIIIARCHIVE_JSM_HEADER_HPP
