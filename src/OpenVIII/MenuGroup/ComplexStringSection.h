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
namespace OpenVIII::MenuGroup {
struct ComplexStringSectionEntry
{
private:
  static constexpr auto unkCount = 6U;
  std::array<uint8_t, unkCount> unknown_{};
  std::string_view buffer_{};

  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings#String_Entry
public:
  explicit ComplexStringSectionEntry(const std::string_view &buffer)
  {
    uint16_t length_{};
    std::memcpy(unknown_.data(), buffer.data(), sizeof(unknown_));
    std::memcpy(&length_, buffer.data() + sizeof(unknown_), sizeof(length_));
    buffer_ = std::string_view{ buffer.data() + sizeof(unknown_) + sizeof(length_), length_ };
  }
  template<LangT langVal> std::ostream &out(std::ostream &os) const
  {
    return os << FF8String<langVal>::Decode(buffer_);
  }
};
struct ComplexStringSectionOffsets
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings#Seek_Struct
private:
  std::uint16_t offset_{};
  std::uint16_t index_{};

public:
  [[nodiscard]] auto offset() const noexcept { return offset_; }
  [[nodiscard]] auto index() const noexcept { return index_; }
  friend std::ostream &operator<<(std::ostream &os, const ComplexStringSectionOffsets &item)
  {
    return os << '{' << item.index() << ", " << item.offset() << '}';
  }
};
struct ComplexStringSection
{
  // http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_complex_strings
private:
  std::uint32_t count_{};
  std::string_view buffer_{};
  static constexpr auto sectionCount_ = 6U;
  std::array<std::string_view, sectionCount_> data_{};

public:
  ComplexStringSection(const std::string_view &buffer, const std::array<std::string_view, sectionCount_> &data)
    : data_{ data }
  {
    if (std::size(buffer) > sizeof(count_)) {
      std::memcpy(&count_, buffer.data(), sizeof(count_));
      buffer_ = std::string_view{ buffer.data() + sizeof(count_), std::size(buffer) - sizeof(count_) };
    }
  }
  [[nodiscard]] auto size() const { return count_; }
  [[nodiscard]] ComplexStringSectionOffsets at(const size_t &id) const
  {
    ComplexStringSectionOffsets d{};
    if (id >= count_ || sizeof(d) * id >= std::size(buffer_)) {
      return {};// throw?
    }
    memcpy(&d, buffer_.data() + (sizeof(d) * id), sizeof(d));
    return d;
  }

  [[nodiscard]] auto at(const ComplexStringSectionOffsets &offsets) const
  {
    const auto temp = data_.at(offsets.index());
    return ComplexStringSectionEntry{ std::string_view{
      temp.data() + offsets.offset(), temp.size() - offsets.offset() } };
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_COMPLEXSTRINGSECTION_H
