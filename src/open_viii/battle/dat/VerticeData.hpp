//
// Created by pcvii on 4/5/2023.
//

#ifndef OPENVIII_CPP_WIP_VERTICEDATA_HPP
#define OPENVIII_CPP_WIP_VERTICEDATA_HPP
#include "open_viii/graphics/Vertice.hpp"
#include <cstdint>
namespace open_viii::battle::dat {
struct VerticeData
{
  std::uint16_t                              bone_id{};
  std::uint16_t                              vertices_count{};
  open_viii::graphics::Vertice<std::int16_t> vertice{};
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_VERTICEDATA_HPP
