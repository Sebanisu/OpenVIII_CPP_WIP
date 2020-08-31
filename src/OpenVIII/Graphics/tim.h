//
// Created by pcvii on 8/20/2020.
//

#ifndef VIIIARCHIVE_TIM_H
#define VIIIARCHIVE_TIM_H
#include "OpenVIII/Graphics/tim/BPPT.h"
#include "OpenVIII/Graphics/tim/timHeader.h"
#include "OpenVIII/Graphics/tim/timClutHeader.h"
#include "OpenVIII/Graphics/color.h"
#include <sstream>
#include "_4bitValues.h"
namespace OpenVIII::Graphics {
/**
 * TIM, or PSX TIM, is an uncompressed raster image file format associated with the Sony PlayStation family of video
 * game consoles. It supports 4- and 8-bit paletted images, and 15- and 24-bit full color images.
 * @see https://github.com/myst6re/vincent-tim
 * @see https://github.com/myst6re/deling/blob/master/FF8Image.cpp#L30
 * @see http://www.raphnet.net/electronique/psx_adaptor/Playstation.txt
 * @see http://www.psxdev.net/forum/viewtopic.php?t=109
 * @see https://mrclick.zophar.net/TilEd/download/timgfx.txt
 * @see http://www.elisanet.fi/6581/PSX/doc/Playstation_Hardware.pdf
 * @see http://www.elisanet.fi/6581/PSX/doc/psx.pdf
 * @see http://www.romhacking.net/documents/31/
 * @see http://mrclick.zophar.net/TilEd/download/timgfx.txt
 * @see http://fileformats.archiveteam.org/wiki/TIM_(PlayStation_graphics)
 */
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
    //    // clangTidy says this function can be static which it cannot be static.
    //    color16 rv{};
    //    if (timClutHeader_.Rectangle().Height() != 0) {
    //
    //      if (row > timClutHeader_.Rectangle().Height()) {
    //        row = 0;
    //      }
    //      [[maybe_unused]] const auto index{ (colorKey * 2U) + (row * timClutHeader_.Rectangle().Width()) };
    //      if (index < timClutData_.size()) {
    //
    //        memcpy(&rv, timClutData_.data() + index, sizeof(rv));
    //      }
    //    }
    //    return rv;

    const auto paletteSpan = std::span(reinterpret_cast<const color16 *>(std::ranges::data(timClutData_)),
      timClutHeader_.Rectangle().Width() * timClutHeader_.Rectangle().Height())
                               .subspan(row * timClutHeader_.Rectangle().Width());
    return paletteSpan[colorKey];
  }
  template<typename dstT> [[nodiscard]] dstT getColorT(std::size_t index) const
  {
    dstT rv{};
    index *= sizeof(rv);
    if (index + sizeof(rv) >= timImageData_.size()) {
      return {};
    }
    memcpy(&rv, timImageData_.data() + index, sizeof(rv));
    return rv;
  }

public:
  tim() = default;
  explicit tim([[maybe_unused]] std::span<const char> buffer)
  {
    if (std::ranges::size(buffer) == 0) {
      return;
    }
    bool fail = false;
    const auto setData = [&buffer, &fail](std::string_view &item, const std::size_t &bytes) {
      if (bytes > std::ranges::size(buffer)) {
        fail = true;
        return;
      }
      item = std::string_view{ std::ranges::data(buffer), bytes };
      buffer = buffer.subspan(bytes);
    };
    const auto getValue = [&buffer, &fail](auto &item) {
      const std::size_t sz = sizeof(item);
      if (sz > std::ranges::size(buffer)) {
        fail = true;
        return;
      }
      memcpy(&item, std::ranges::data(buffer), sz);
      buffer = buffer.subspan(sz);
    };

    getValue(timHeader_);
    if (!timHeader_.Check() || fail) {
      timHeader_ = {};
      return;
    }
    if (timHeader_.BPP().ColorLookupTablePresent()) {
      getValue(timClutHeader_);
      if (!timClutHeader_.Check() || fail) {
        timClutHeader_ = {};
        timHeader_ = {};
        return;
      }
      setData(timClutData_, timClutHeader_.dataSize());
      if (fail) {
        return;
      }
    }
    getValue(timImageHeader_);
    if (fail) {
      return;
    }
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
  [[maybe_unused]] [[nodiscard]] auto X() const { return timImageHeader_.Rectangle().X(); }
  [[maybe_unused]] [[nodiscard]] auto Y() const { return timImageHeader_.Rectangle().Y(); }

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
  template<typename dstT = color32<>>
  [[nodiscard]] std::vector<dstT> GetColors([[maybe_unused]] std::uint16_t row = 0U) const
  {
    static constexpr auto bpp4 = 4U;
    static constexpr auto bpp8 = 8U;
    static constexpr auto bpp16 = 16U;
    static constexpr auto bpp24 = 24U;
    std::vector<dstT> output{};
    const auto outSize = static_cast<std::size_t>(Width()) * static_cast<std::size_t>(Height());
    output.reserve(outSize);
    switch (static_cast<int>(timHeader_.BPP())) {

    case bpp4: {

      const auto s = std::span(
        reinterpret_cast<const _4bitValues *>(std::ranges::data(timImageData_)), std::ranges::size(timImageData_));
      // break;
      for (size_t i{}; i < outSize / 2; i++) {
        // std::bitset<8> bs{ static_cast<std::uint8_t>(timImageData_.at(i / 2U)) };
        // static constexpr std::bitset<8> oct1{ 0xF };
        // static constexpr std::bitset<8> oct2{ 0xF0 };
        //        if (i % 2 == 0) {
        //          output.emplace_back(getColor(row, static_cast<std::uint8_t>((bs & oct1).to_ulong())));
        //        } else {
        //          output.emplace_back(getColor(row, static_cast<std::uint8_t>(((bs & oct2) >> 4U).to_ulong())));
        //        }
        const auto values = s[i];
        output.emplace_back(getColor(row, values.first));
        output.emplace_back(getColor(row, values.second));
      }
      break;
    }
    case bpp8: {
      // break;
      for (size_t i{}; i < outSize; i++) {
        output.emplace_back(getColor(row, static_cast<std::uint8_t>(timImageData_.at(i))));
      }
      break;
    }
    case bpp16: {
      const auto sz = std::ranges::size(timImageData_) / sizeof(color16);
      const auto s = std::span(reinterpret_cast<const color16 *>(std::ranges::data(timImageData_)), sz);

      for (const Color auto i : s) { output.emplace_back(i); }
      //      for (size_t i{}; i < std::min(outSize, timImageData_.size() / sizeof(color16)); i++) {
      //        output.emplace_back(getColorT<color16>(i));
      //      }
      break;
    }
    case bpp24: {
      for (size_t i{}; i < outSize; i++) { output.emplace_back(getColorT<color24<2, 1, 0>>(i)); }
      break;
    }
    }
    return output;
  }

  [[maybe_unused]] void Save(std::string_view filename) const
  {
    if (ClutRows() == 0) {
      ppm::save(GetColors<color16>(), Width(), Height(), filename);
    } else {
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i = 0; i < ClutRows(); i++) {
        auto ss = std::stringstream{};
        ss << (path.parent_path() / path.stem()).string() << '_' << i << path.extension().string();
        ppm::save(GetColors<color16>(i), Width(), Height(), ss.str());
      }
    }
  }
};

}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TIM_H