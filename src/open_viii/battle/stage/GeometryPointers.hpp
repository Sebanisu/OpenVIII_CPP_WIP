//
// Created by pcvii on 3/23/2023.
//

#ifndef OPENVIII_CPP_WIP_GEOMETRYPOINTERS_HPP
#define OPENVIII_CPP_WIP_GEOMETRYPOINTERS_HPP
#include <cstdint>
struct GeometryPointers
{
public:
  const char *group_1_pointer;
  const char *group_2_pointer;
  const char *group_3_pointer;
  const char *group_4_pointer;
  const char *texture_unused_pointer;
  const char *texture_pointer;
  const char *eof;
};
struct GeometryOffsets
{
public:
  std::uint32_t group_1_offset;
  std::uint32_t group_2_offset;
  std::uint32_t group_3_offset;
  std::uint32_t group_4_offset;
  std::uint32_t texture_unused_offset;
  std::uint32_t texture_offset;
  std::uint32_t eof;
  auto
    operator+(const char *const base_location) const -> GeometryPointers
  {
    return { base_location + group_1_offset,
             base_location + group_2_offset,
             base_location + group_3_offset,
             base_location + group_4_offset,
             base_location + texture_unused_offset,
             base_location + texture_offset,
             base_location + eof };
  }
  friend auto
    operator+(const char *const base_location, GeometryOffsets other)
      -> GeometryPointers
  {
    return other + base_location;
  }
};
#endif// OPENVIII_CPP_WIP_GEOMETRYPOINTERS_HPP
