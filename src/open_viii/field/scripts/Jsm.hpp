//
// Created by pcvii on 1/18/2021.
//
#ifndef VIIIARCHIVE_JSM_HPP
#define VIIIARCHIVE_JSM_HPP
#include "JsmEntity.hpp"
#include "JsmHeader.hpp"
#include "JsmScriptEntity.hpp"
#include "open_viii/tools/Tools.hpp"
#include <span>
#include <vector>
namespace open_viii::field::scripts {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_JSM
 */
struct Jsm
{
private:
  JsmHeader                    m_header{};
  std::vector<JsmEntity>       m_door_entities{};
  std::vector<JsmEntity>       m_walk_mesh_entities{};
  std::vector<JsmEntity>       m_background_entities{};
  std::vector<JsmEntity>       m_other_entities{};
  std::vector<JsmScriptEntity> m_script_entities{};
  [[nodiscard]] auto
    get_door_entities(const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<JsmEntity>>(
      buffer.subspan(m_header.offset_door_entities()),
      m_header.count_door_entities());
  }
  [[nodiscard]] auto
    get_walk_mesh_entities(const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<JsmEntity>>(
      buffer.subspan(m_header.offset_walk_mesh_line_entities()),
      m_header.count_walk_mesh_line_entities());
  }
  [[nodiscard]] auto
    get_background_entities(const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<JsmEntity>>(
      buffer.subspan(m_header.offset_background_entities()),
      m_header.count_background_entities());
  }
  [[nodiscard]] auto
    get_other_entities(const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<JsmEntity>>(
      buffer.subspan(m_header.offset_other_entities()),
      m_header.count_other_entities());
  }
  [[nodiscard]] auto
    get_script_entities(const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<JsmScriptEntity>>(
      buffer.subspan(m_header.offset_section_1()), m_header.count_section_1());
  }

public:
  explicit Jsm(std::span<const char> buffer)
    : m_header(fix_jsm_header_counts(tools::read_val<JsmHeader>(buffer))),
      m_door_entities(get_door_entities(buffer)),
      m_walk_mesh_entities(get_walk_mesh_entities(buffer)),
      m_background_entities(get_background_entities(buffer)),
      m_other_entities(get_other_entities(buffer)),
      m_script_entities(get_script_entities(buffer))
  {
    assert(std::ranges::size(m_door_entities)
           == m_header.count_door_entities());
    assert(std::ranges::size(m_walk_mesh_entities)
           == m_header.count_walk_mesh_line_entities());
    assert(std::ranges::size(m_background_entities)
           == m_header.count_background_entities());
    assert(std::ranges::size(m_other_entities)
           == m_header.count_other_entities());
    assert(std::ranges::size(m_script_entities) == m_header.count_section_1());
    // std::cout << m_header.total_count() << '\t' << m_header.expected_count()
    // << std::endl; //counts must match
    assert(m_header.valid_count());
  }
};
}// namespace open_viii::field::scripts
#endif// VIIIARCHIVE_JSM_HPP
