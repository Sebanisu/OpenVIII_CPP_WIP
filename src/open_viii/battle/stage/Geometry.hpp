//
// Created by pcvii on 11/17/2020.
//

#ifndef VIIIARCHIVE_GEOMETRY_HPP
#define VIIIARCHIVE_GEOMETRY_HPP
#include "GeometryHeader1.hpp"
#include "GeometryHeader2.hpp"
#include "Shapes.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include <vector>
namespace open_viii::battle::stage {


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
struct Geometry
{
private:
  GeometryHeader1 m_geometry_header1{};
  std::vector<graphics::Vertice<std::int16_t>>
    m_vertices{};// use calc pad after reading this to see how many bytes to
                 // skip
  GeometryHeader2 m_geometry_header2{};
  std::vector<Triangle> m_triangles{};
  std::vector<Triangle> m_quads{};
  static long calc_pad(long position)
  {
    return (position % 4) + 4;
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GEOMETRY_HPP
