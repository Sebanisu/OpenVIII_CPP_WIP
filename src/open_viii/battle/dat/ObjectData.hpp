//
// Created by pcvii on 4/5/2023.
//

#ifndef OPENVIII_CPP_WIP_OBJECTDATA_HPP
#define OPENVIII_CPP_WIP_OBJECTDATA_HPP
#include "DatQuad.hpp"
#include "DatTriangle.hpp"
#include "open_viii/tools/Read.hpp"
#include "VerticeData.hpp"
#include <array>
#include <cstdint>
#include <span>
#include <vector>

namespace open_viii::battle::dat {
struct ObjectData
{
  std::uint16_t                vertice_data_count{};
  std::vector<VerticeData>     vertice_datas{};
  // padding 4 - (absolutePosition % 4)
  std::uint16_t                triangle_count{};
  std::uint16_t                quad_count{};
  std::array<std::uint8_t, 8U> unused{};
  std::vector<DatTriangle>     triangles{};
  std::vector<DatQuad>         quads{};

  ObjectData() = default;
  ObjectData(
    [[maybe_unused]] const char *const     file_start,
    [[maybe_unused]] std::span<const char> span)
  {
    vertice_data_count = tools::read_val<std::uint16_t>(span);
    for (std::uint16_t i = 0; i != vertice_data_count; ++i) {
      vertice_datas.emplace_back(span);
    }
    if ((std::distance(file_start, span.data()) % 4) != 0) {
      span = span.subspan(static_cast<std::span<const char>::size_type>(
        4 - (std::distance(file_start, span.data()) % 4)));
    }
    triangle_count = tools::read_val<std::uint16_t>(span);

    quad_count     = tools::read_val<std::uint16_t>(span);
    unused         = tools::read_val<std::array<std::uint8_t, 8U>>(span);
    triangles      = tools::read_vals<DatTriangle>(span, triangle_count);
    quads          = tools::read_vals<DatQuad>(span, quad_count);
  }
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_OBJECTDATA_HPP
