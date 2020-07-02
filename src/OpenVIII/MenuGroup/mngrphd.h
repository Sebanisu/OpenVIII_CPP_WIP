//
// Created by pcvii on 7/1/2020.
//

#ifndef VIIIARCHIVE_MNGRPHD_H
#define VIIIARCHIVE_MNGRPHD_H
#include <cstdint>
#include <cstring>
#include <string_view>
#include <array>
namespace OpenVIII::MenuGroup {
struct MenuGroupHeaderSection
{
  //http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrphd_bin
  //http://wiki.ffrtt.ru/index.php?title=FF8/Menu_mngrp_bin
private:
  static constexpr std::uint32_t invalidSize_ = 0U;
  static constexpr std::uint32_t invalidFileOffset_ = 0xFFFFFFFF;
  static constexpr std::uint8_t permOffset_{ 1 };
  std::uint32_t fileOffset_{};
  std::uint32_t size_{};

public:
  [[nodiscard]] auto FileOffset() const noexcept { return fileOffset_ - permOffset_; }
  [[nodiscard]] auto Size() const noexcept { return size_; }
  template<typename charContainer = std::string_view>
  [[nodiscard]] std::string_view GetSectionBuffer(const charContainer &fileBuffer) const
  {
    if (Size() == invalidSize_ || fileOffset_ == invalidFileOffset_ || FileOffset() + Size() > std::size(fileBuffer)) {
      return {};// returns empty buffer if invalid or error.
    }
    return { std::data(fileBuffer) + FileOffset(), Size() };
  }
};
struct MenuGroupHeader
{
private:
  static constexpr auto size_{ 2048U };
  static constexpr auto count_{ size_ / sizeof(MenuGroupHeaderSection) };
  std::array<MenuGroupHeaderSection, count_> sections_{};

public:
  static constexpr auto FileName = std::string_view{ "mngrphd.bin" };
  [[nodiscard]] const auto &Sections() const { return sections_; }

  template<typename mainT> explicit MenuGroupHeader(const mainT &main)
  {
    auto buffer = main.GetEntryData(FileName);
    if (std::size(buffer) < sizeof(std::uint32_t) || size_ != std::size(buffer)) {
      return;
    }
    std::memcpy(sections_.data(), buffer.data(),std::size(buffer));
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_MNGRPHD_H
