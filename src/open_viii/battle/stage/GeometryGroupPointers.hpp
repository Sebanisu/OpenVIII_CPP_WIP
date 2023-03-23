//
// Created by pcvii on 3/23/2023.
//

#ifndef OPENVIII_CPP_WIP_GEOMETRYGROUPPOINTERS_HPP
#define OPENVIII_CPP_WIP_GEOMETRYGROUPPOINTERS_HPP
#include <cstdint>
struct GeometryGroupPointers
{
public:
  std::uint32_t number_of_sections;
  const char   *settings_1_pointer;
  const char   *object_list_pointer;
  const char   *settings_2_pointer;
  const char   *relative_eof;
};

struct GeometryGroupOffsets
{
public:
  std::uint32_t number_of_sections;
  std::uint32_t settings_1_offset;
  std::uint32_t object_list_offset;
  std::uint32_t settings_2_offset;
  std::uint32_t relative_eof;
  auto
    operator+(const char *const base_location) const -> GeometryGroupPointers
  {
    return { number_of_sections,
             base_location + settings_1_offset,
             base_location + object_list_offset,
             base_location + settings_2_offset,
             base_location + relative_eof };
  }
  friend auto
    operator+(const char *const base_location, GeometryGroupOffsets other)
      -> GeometryGroupPointers
  {
    return other + base_location;
  }
};
#endif// OPENVIII_CPP_WIP_GEOMETRYGROUPPOINTERS_HPP
