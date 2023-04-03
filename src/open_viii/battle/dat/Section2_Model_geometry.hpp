//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION2_MODEL_GEOMETRY_HPP
#define OPENVIII_CPP_WIP_SECTION2_MODEL_GEOMETRY_HPP
#include "DatHeader.hpp"
#include "open_viii/graphics/Point.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include <array>
namespace open_viii::battle {
/**
 * @see https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Section_2:_Model_geometry
 */
struct Section2_Model_geometry
{
  DatHeader m_header{};
  struct triangle
  {
    std::array<std::uint16_t, 3U>
      m_raw_face_indeces{};///< vertex_indexes[i] & 0xFFF, other bits are unknown
    open_viii::graphics::Point<std::uint8_t> m_uv1;
    open_viii::graphics::Point<std::uint8_t> m_uv2;
    std::uint16_t                            m_raw_texture_id{}; ///< m_raw_texture_id >> 6 & 0b111U
    open_viii::graphics::Point<std::uint8_t> m_uv3;
    std::uint16_t                            u{};// textureID_related2
    static constexpr std::size_t             EXPECTED_SIZE = 16U;
  };
  static_assert(sizeof(triangle) == triangle::EXPECTED_SIZE);
  struct quad
  {
    std::array<std::uint16_t, 4U>
                                             m_raw_face_indeces{};// vertex_indexes[0] &= 0xFFF, other bits are unknown
    open_viii::graphics::Point<std::uint8_t> m_uv1;
    std::uint16_t                            m_raw_texture_id{};  ///< m_raw_texture_id >> 6 & 0b111U
    open_viii::graphics::Point<std::uint8_t> m_uv2;
    std::uint16_t                            u{};// textureID_related2
    open_viii::graphics::Point<std::uint8_t> m_uv3;
    open_viii::graphics::Point<std::uint8_t> m_uv4;
    static constexpr std::size_t             EXPECTED_SIZE = 20U;
  };
  static_assert(sizeof(quad) == quad::EXPECTED_SIZE);
  struct VerticeData
  {
    std::uint16_t                                           m_bone_id{};
    std::uint16_t                                           m_vertices_count{};
    std::vector<open_viii::graphics::Vertice<std::int16_t>> m_vertices{};
  };
  struct ObjectData
  {
    std::uint16_t                m_object_count{};
    std::vector<VerticeData>     m_objects{};
    // padding 4 - (absolutePosition % 4)
    std::uint16_t                m_triable_count{};
    std::uint16_t                m_quad_count{};
    std::array<std::uint8_t, 8U> m_unused{};
    std::vector<triangle>        m_triangles{};
    std::vector<quad>            m_quads{};
  };
};
}// namespace open_viii::battle
#endif// OPENVIII_CPP_WIP_SECTION2_MODEL_GEOMETRY_HPP
