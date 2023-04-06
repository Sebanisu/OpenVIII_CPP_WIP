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
  std::vector<open_viii::graphics::Vertice<std::int16_t>> vertices{};
  VerticeData() = default;
  VerticeData(std::span<const char> & span)
	  :
	  bone_id(tools::read_val<std::uint16_t>(span)),
	  vertices_count(tools::read_val<std::uint16_t>(span)),
	  vertices(tools::read_vals<open_viii::graphics::Vertice<std::int16_t>>(span,vertices_count))
  {

  }
  VerticeData(const std::span<const char>& const_span)	  
  {
	  auto span = const_span;
	  bone_id = tools::read_val<std::uint16_t>(span);
	  vertices_count = tools::read_val<std::uint16_t>(span);
	  vertices = tools::read_vals<open_viii::graphics::Vertice<std::int16_t>>(span, vertices_count);
  }
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_VERTICEDATA_HPP
