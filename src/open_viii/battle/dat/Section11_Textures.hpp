//
// Created by pcvii on 4/3/2023.
//

#ifndef OPENVIII_CPP_WIP_SECTION11_TEXTURES_HPP
#define OPENVIII_CPP_WIP_SECTION11_TEXTURES_HPP
#include "DatHeader.hpp"
#include "open_viii/graphics/Tim.hpp"
namespace open_viii::battle::dat {
struct Section11_Textures
{
  std::span<const char>      m_section_span{};
  DatHeader                  m_section_header{};
  std::uint32_t              m_EOF_offset{};
  std::vector<graphics::Tim> m_tims{};

  Section11_Textures() = default;
  explicit Section11_Textures(
    [[maybe_unused]] const char *const section_start,
    std::span<const char>              span)
    : m_section_span(span), m_section_header(span),
      m_EOF_offset(tools::read_val<std::uint32_t>(span))
  {
    // const char *const section_start = span.data();
    // const char *const section_end   = span.data() + span.size();

    for (const std::uint32_t offset : m_section_header.m_offsets) {
      m_tims.emplace_back(m_section_span.subspan(offset));
    }
  }
};
}// namespace open_viii::battle::dat
#endif// OPENVIII_CPP_WIP_SECTION11_TEXTURES_HPP
