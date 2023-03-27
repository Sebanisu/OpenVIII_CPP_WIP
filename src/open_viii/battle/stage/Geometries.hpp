//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRIES_HPP
#define VIIIARCHIVE_GEOMETRIES_HPP
#include "Geometry.hpp"
namespace open_viii::battle::stage {
/**
 * @brief Represents a collection of geometry objects in the battle stage.
 * There may be more than one Geometry object. Check for 01 00 01 00 to know if
 * there are one or more.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 */
struct Geometries
{
public:
  std::vector<Geometry> m_geometries{};///< @brief Vector of Geometry objects
                                       ///< (usually one, but can be more).

  Geometries() =default;
  Geometries(const char *const buffer_begin, std::span<const char> span)
  {
    const char *const model_group_ptr     = span.data();
    const char *const model_group_end_ptr = span.data() + span.size();

    std::cout << "\t\t Assigned Offset: " << std::hex << std::uppercase
              << std::distance(buffer_begin, span.data()) << std::dec
              << std::nouppercase << std::endl;
    const auto model_count = Geometry::read_val<std::uint32_t>(span);
    std::cout << "\t\t Model Count: " << model_count << std::endl;
    const auto m_model_offsets = Geometry::read_vals<std::uint32_t>(span, model_count);
    auto       m_model_pointers
      = m_model_offsets
      | std::views::transform([&](std::uint32_t offset) -> const char * {
          return model_group_ptr + offset;
        });
    span = std::span(m_model_pointers.front(), model_group_end_ptr);
    m_geometries.reserve(model_count);
    auto m_model_spans = m_model_pointers | std::views::transform([model_group_end_ptr](const char * const ptr){return std::span(ptr, model_group_end_ptr);});
    std::ranges::transform(m_model_spans,std::back_inserter(m_geometries),[buffer_begin](std::span<const char> out_span){return Geometry(buffer_begin,out_span);});
  }

  static constexpr std::array<char, 4> START_OF_HEADER
    = { 1, 0, 1, 0 };///< @brief Sequence indicating the start of a header.
};
}// namespace open_viii::battle::stage

#endif// VIIIARCHIVE_GEOMETRIES_HPP
