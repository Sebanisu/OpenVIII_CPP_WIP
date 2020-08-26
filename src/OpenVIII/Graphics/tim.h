//
// Created by pcvii on 8/20/2020.
//

#ifndef VIIIARCHIVE_TIM_H
#define VIIIARCHIVE_TIM_H
#include "OpenVIII/Graphics/tim/BPPT.h"
#include "OpenVIII/Graphics/tim/timHeader.h"
#include "OpenVIII/Graphics/tim/timClutHeader.h"
#include "OpenVIII/Graphics/color.h"
namespace OpenVIII::Graphics {
struct tim
{
private:
  timHeader timHeader_{};
  timClutHeader timClutHeader_{};
  std::string_view timClutData_{};
  timImageHeader timImageHeader_{};
  std::string_view timImageData_{};

  [[nodiscard]] color16 getColor([[maybe_unused]] std::uint16_t row, [[maybe_unused]] std::uint8_t colorKey) const
  {
    // clangTidy says this function can be static which it cannot be static.
    color16 rv{};
    if (timClutHeader_.Rectangle().Height() != 0) {

      if (row > timClutHeader_.Rectangle().Height()) {
        row = 0;
      }
      [[maybe_unused]] const auto index{ (colorKey * 2U) + (row * timClutHeader_.Rectangle().Width()) };
      if (index < timClutData_.size()) {

        memcpy(&rv, timClutData_.data() + index, sizeof(rv));
      }
    }
    return rv;
  }
  [[nodiscard]] color16 getColor16(std::size_t index) const
  {
    index *= 2U;
    if (index >= timImageData_.size()) {
      return {};
    }
    color16 rv{};
    memcpy(&rv, timImageData_.data() + index, sizeof(rv));
    return rv;
  }


  [[nodiscard]] color24<2, 1, 0> getColor24(std::size_t index) const
  {
    index *= 2U;
    if (index >= timImageData_.size()) {
      return {};
    }
    color24<2, 1, 0> rv{};
    memcpy(&rv, timImageData_.data() + index, sizeof(rv));
    return rv;
  }

public:
  explicit tim(const std::string_view &buffer)
  {
    if (std::size(buffer) == 0) {
      return;
    }
    auto ptr_ = buffer.data();
    auto size_ = buffer.size();
    const auto setData = [&ptr_, &size_, &buffer](std::string_view &item, const auto &bytes) {
      item = std::string_view{ ptr_, bytes };
      ptr_ += bytes;
      size_ -= bytes;
    };
    const auto getValue = [&ptr_, &size_, &buffer](auto &item) {
      memcpy(&item, ptr_, sizeof(item));
      ptr_ += sizeof(item);
      size_ -= sizeof(item);
    };
    getValue(timHeader_);
    if (timHeader_.BPP().ColorLookupTablePresent()) {
      getValue(timClutHeader_);
      setData(timClutData_, timClutHeader_.dataSize());
    }
    getValue(timImageHeader_);
    setData(timImageData_, timImageHeader_.dataSize());
  }
  [[nodiscard]] bool Check() const
  {
    return timHeader_.Check() && (!timHeader_.BPP().Check() || timClutHeader_.Check());
  }
  [[nodiscard]] auto Width() const
  {
    static constexpr auto _4bpp_step{ 4 };
    static constexpr auto _8bpp_step{ 2 };
    static constexpr auto _24bpp_step{ 1.5 };
    if (timHeader_.BPP()._4bpp()) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() * _4bpp_step);// 4pp
    }
    if (timHeader_.BPP()._8bpp()) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() * _8bpp_step);// 8pp
    }
    if (timHeader_.BPP()._16bpp()) {
      return timImageHeader_.Rectangle().Width();// 16bpp
    }
    if (timHeader_.BPP()._24bpp()) {
      return static_cast<decltype(timImageHeader_.Rectangle().Width())>(
        timImageHeader_.Rectangle().Width() / _24bpp_step);// 24 bpp
    }
    return static_cast<decltype(timImageHeader_.Rectangle().Width())>(0);// invalid value
  }
  [[nodiscard]] auto Height() const { return timImageHeader_.Rectangle().Height(); }
  [[nodiscard]] auto X() const { return timImageHeader_.Rectangle().X(); }
  [[nodiscard]] auto Y() const { return timImageHeader_.Rectangle().Y(); }

  [[maybe_unused]] [[nodiscard]] auto ClutX() const { return timClutHeader_.Rectangle().X(); }
  [[maybe_unused]] [[nodiscard]] auto ClutY() const { return timClutHeader_.Rectangle().Y(); }
  [[nodiscard]] auto size() const { return sizeof(timHeader_) + timClutHeader_.size() + timImageHeader_.size(); }
  [[nodiscard]] auto ClutRows() const { return timClutHeader_.Rectangle().Height(); }
  [[maybe_unused]] [[nodiscard]] auto ClutColors() const { return timClutHeader_.Rectangle().Width(); }

  friend std::ostream &operator<<(std::ostream &os, const tim &input)
  {
    return os << '{' << input.timHeader_ << ", " << input.timClutHeader_ << ", " << input.timImageHeader_
              << ", Corrected Width: " << input.Width() << '}';
  }
  [[nodiscard]] std::vector<color32<>> GetColors([[maybe_unused]] std::uint16_t row = 0U) const
  {

    std::vector<color32<>> output{};
    output.reserve(static_cast<std::size_t>(Width()) * static_cast<std::size_t>(Height()));
    switch (static_cast<int>(timHeader_.BPP())) {

    case 4U: {
      for (size_t i{}; i < output.capacity(); i++) {
        std::bitset<8> bs{ static_cast<std::uint8_t>(timImageData_.at(i / 2U)) };
        static constexpr std::bitset<8> oct1{ 0xF };
        static constexpr std::bitset<8> oct2{ 0xF0 };
        if (i % 2 == 0) {
          output.emplace_back(getColor(row, static_cast<std::uint8_t>((bs & oct1).to_ulong())));
        } else {
          output.emplace_back(getColor(row, static_cast<std::uint8_t>(((bs & oct2) >> 4U).to_ulong())));
        }
      }
      break;
    }
    case 8U: {
      for (size_t i{}; i < output.capacity(); i++) {
        output.emplace_back(getColor(row, static_cast<std::uint8_t>(timImageData_.at(i))));
      }
      break;
    }
    case 16U: {
      for (size_t i{}; i < output.capacity(); i++) { output.emplace_back(getColor16(i)); }
      break;
    }
    case 24U: {
      for (size_t i{}; i < output.capacity(); i++) { output.emplace_back(getColor24(i)); }
      break;
    }
    }
    return output;
  }
};

#endif// VIIIARCHIVE_TIM_H
}