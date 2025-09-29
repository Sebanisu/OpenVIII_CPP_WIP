//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION2_MODEL_GEOMETRY_HPP
#define OPENVIII_CPP_WIP_SECTION2_MODEL_GEOMETRY_HPP
#include "DatHeader.hpp"
#include "ObjectData.hpp"
#include <array>
namespace open_viii::battle::dat {
/**
 * @brief Model Geometry struct containing header and object data.
 * @see
 * https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT#Section_2:_Model_geometry
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Dat/Geometry.cs
 */
struct Section2_Model_Geometry
{
public:
  DatHeader               header{};
  std::vector<ObjectData> object_data{};
  Section2_Model_Geometry() = default;
  Section2_Model_Geometry(
    [[maybe_unused]] const char *const           file_start,
    [[maybe_unused]] const std::span<const char> span)
    : header(span)
  {
    object_data.reserve(header.m_count);
    for (std::uint32_t const offset : header.m_offsets) {
      object_data.emplace_back(file_start, span.subspan(offset));
    }
  }
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_SECTION2_MODEL_GEOMETRY_HPP
