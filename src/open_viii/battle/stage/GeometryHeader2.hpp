//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRYHEADER2_HPP
#define VIIIARCHIVE_GEOMETRYHEADER2_HPP
#include <cstdint>
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 * Offset	Length	Description
0	4 bytes	Always 01 00 01 00 / Header of object
4	2 bytes	Uint16 / number of vertices.
6	Number of vertices * 6	Vertice data, short X; short Y; short Z;
6 + (Number of vertices *6)	(AbsolutePosition MOD 4) + 4	Padding
varies (just after above)	2 bytes	uint16 / number of triangles
varies (just after above)	2 bytes	uint16 / number of quads
varies (just after above)	4 bytes	padding
varies (just after above)	number of triangles * 20	Triangle data.
If NumOfTriangles = 0, then instead of any triangle data, there's quad data.
varies (just after above)	number of quads * 24	Triangle data. If
NumOfQuads = 0, then instead of any quad data, there's either next header 01 00
01 00, or end of group.
 */
namespace open_viii::battle::stage {
struct GeometryHeader2
{
private:
  std::uint16_t m_triangle_count{};
  std::uint16_t m_quad_count{};
  std::uint32_t m_padding{};

public:
  [[nodiscard]] const auto &
    triangle_count() const noexcept
  {
    return m_triangle_count;
  }
  [[nodiscard]] const auto &
    quad_count() const noexcept
  {
    return m_quad_count;
  };
  static constexpr std::size_t EXPECTED_SIZE = 8U;
};
static_assert(sizeof(GeometryHeader2) == GeometryHeader2::EXPECTED_SIZE);
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GEOMETRYHEADER2_HPP
