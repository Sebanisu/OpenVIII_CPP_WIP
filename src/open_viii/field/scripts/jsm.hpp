//
// Created by pcvii on 1/18/2021.
//

#ifndef VIIIARCHIVE_JSM_HPP
#define VIIIARCHIVE_JSM_HPP
#include "jsm_entity.hpp"
#include "jsm_header.hpp"
#include "jsm_script_entity.hpp"
#include "open_viii/tools/Tools.hpp"
#include <span>
#include <vector>
namespace open_viii::field::scripts {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_JSM
 */
struct jsm
{
private:
  jsm_header m_header{};
  std::vector<jsm_entity> m_door_entities{};
  std::vector<jsm_entity> m_walk_mesh_entities{};
  std::vector<jsm_entity> m_background_entities{};
  std::vector<jsm_entity> m_other_entities{};
  std::vector<jsm_script_entity> m_script_entities{};
  [[nodiscard]] auto get_door_entities(
    const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<jsm_entity>>(
      buffer.subspan(sizeof(m_header)), m_header.count_door_entities());
  }

  [[nodiscard]] auto get_walk_mesh_entities(
    const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<jsm_entity>>(
      buffer.subspan(
        sizeof(m_header) + sizeof(jsm_entity) * m_header.count_door_entities()),
      m_header.count_walk_mesh_line_entities());
  }
  [[nodiscard]] auto get_background_entities(
    const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<jsm_entity>>(
      buffer.subspan(sizeof(m_header)
                     + sizeof(jsm_entity)
                         * (m_header.count_door_entities()
                            + m_header.count_walk_mesh_line_entities())),
      m_header.count_background_entities());
  }
  [[nodiscard]] auto get_other_entities(
    const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<jsm_entity>>(
      buffer.subspan(sizeof(m_header)
                     + sizeof(jsm_entity)
                         * (m_header.count_door_entities()
                            + m_header.count_walk_mesh_line_entities())
                     + m_header.count_background_entities()),
      m_header.count_other_entities());
  }
  constexpr static auto script_count(const jsm_header &header)
  {
    return (header.offset_script_data() - header.offset_section_1())
           / sizeof(jsm_script_entity);
  }
  [[nodiscard]] auto get_script_entities(
    const std::span<const char> &buffer) const
  {
    return tools::read_val<std::vector<jsm_script_entity>>(
      buffer.subspan(m_header.offset_section_1()), script_count(m_header));
  }

public:
  explicit jsm(std::span<const char> buffer)
    : m_header(tools::read_val<jsm_header>(buffer)),
      m_door_entities(get_door_entities(buffer)),
      m_walk_mesh_entities(get_walk_mesh_entities(buffer)),
      m_background_entities(get_background_entities(buffer)),
      m_other_entities(get_other_entities(buffer)),
      m_script_entities(get_script_entities(buffer))

  {
    assert(
      std::ranges::size(m_door_entities) == m_header.count_door_entities());
    assert(std::ranges::size(m_walk_mesh_entities)
           == m_header.count_walk_mesh_line_entities());
    assert(std::ranges::size(m_background_entities)
           == m_header.count_background_entities());
    assert(
      std::ranges::size(m_other_entities) == m_header.count_other_entities());
    assert(std::ranges::size(m_script_entities) == script_count(m_header));
  }
};
}// namespace open_viii::field::scripts
#endif// VIIIARCHIVE_JSM_HPP
