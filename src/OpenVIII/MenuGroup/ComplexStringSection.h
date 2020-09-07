//
// Created by pcvii on 8/17/2020.
//

#ifndef VIIIARCHIVE_COMPLEXSTRINGSECTION_H
#define VIIIARCHIVE_COMPLEXSTRINGSECTION_H
#include <cstdint>
#include <cstring>
#include <array>
#include <ostream>
#include <string_view>
#include "OpenVIII/Strings/FF8String.h"
namespace open_viii::menu_group {
struct ComplexStringSectionEntry
{
private:
  static constexpr auto UNK_COUNT = 6U;
  std::array<uint8_t, UNK_COUNT> m_unknown{};
  std::string_view m_buffer{};

  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings#String_Entry
public:
  explicit ComplexStringSectionEntry(const std::string_view &buffer)
  {
    uint16_t length{};
    std::memcpy(m_unknown.data(), buffer.data(), sizeof(m_unknown));
    std::memcpy(&length, buffer.data() + sizeof(m_unknown), sizeof(length));
    m_buffer = std::string_view{ buffer.data() + sizeof(m_unknown) + sizeof(length), length };
  }
  template<LangT langVal> std::ostream &out(std::ostream &os) const
  {
    return os << Tools::u8_to_sv(FF8String<langVal>::decode(m_buffer));
  }
};
struct ComplexStringSectionOffsets
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings#Seek_Struct
private:
  std::uint16_t m_offset{};
  std::uint16_t m_index{};

public:
  [[nodiscard]] auto offset() const noexcept { return m_offset; }
  [[nodiscard]] auto index() const noexcept { return m_index; }
  friend std::ostream &operator<<(std::ostream &os, const ComplexStringSectionOffsets &item)
  {
    return os << '{' << item.index() << ", " << item.offset() << '}';
  }
};
struct ComplexStringSection
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings
private:
  std::uint32_t m_count{};
  std::string_view m_buffer{};
  static constexpr auto SECTION_COUNT = 6U;
  std::array<std::string_view, SECTION_COUNT> m_data{};

public:
  ComplexStringSection(const std::string_view &buffer, const std::array<std::string_view, SECTION_COUNT> &data)
    : m_data{ data }
  {
    if (std::size(buffer) > sizeof(m_count)) {
      std::memcpy(&m_count, buffer.data(), sizeof(m_count));
      m_buffer = std::string_view{ buffer.data() + sizeof(m_count), std::size(buffer) - sizeof(m_count) };
    }
  }
  [[nodiscard]] auto size() const { return m_count; }
  [[nodiscard]] ComplexStringSectionOffsets at(const size_t &id) const
  {
    ComplexStringSectionOffsets d{};
    if (id >= m_count || sizeof(d) * id >= std::size(m_buffer)) {
      return {};// throw?
    }
    memcpy(&d, m_buffer.data() + (sizeof(d) * id), sizeof(d));
    return d;
  }

  [[nodiscard]] auto at(const ComplexStringSectionOffsets &offsets) const
  {
    const auto temp = m_data.at(offsets.index());
    return ComplexStringSectionEntry{ std::string_view{
      temp.data() + offsets.offset(), temp.size() - offsets.offset() } };
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_COMPLEXSTRINGSECTION_H
