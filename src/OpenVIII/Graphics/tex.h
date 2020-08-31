//
// Created by pcvii on 8/27/2020.
//

#ifndef VIIIARCHIVE_TEX_H
#define VIIIARCHIVE_TEX_H
#include "color.h"
#include "ppm.h"
#include <span>
#include <cstdint>
#include <cstring>
#include <ranges>
#include "OpenVIII/Graphics/tex/texHeader.h"
#include "OpenVIII/Graphics/tex/texPixelFormatHeader.h"
#include "OpenVIII/Graphics/tex/texHeader2.h"
#include "OpenVIII/Graphics/tex/texHeader2_Version2.h"
namespace OpenVIII::Graphics {
//{
//  // Palette Entry (BGRA)
//  /**
//   * Blue
//   */
//  std::uint8_t BLUE{};
//  /**
//   * Green
//   */
//  std::uint8_t GREEN{};
//  /**
//   * Red
//   */
//  std::uint8_t RED{};
//  /**
//   * Alpha
//   */
//  std::uint8_t ALPHA{};
//};
/**
 * @see https://github.com/MaKiPL/FF8_Rinoa_s_Toolset/blob/master/SerahToolkit_SharpGL/FF8_Core/TEX.cs
 * @see https://github.com/myst6re/vincent_tim/blob/master/TexFile.cpp
 * @see https://github.com/niemasd/PyFF7/blob/master/PyFF7/tex.py
 */
struct [[maybe_unused]] tex
{
private:
  texHeader texHeader_{};
  texPixelFormatHeader texPixelFormatHeader_{};
  texHeader2 texHeader2_{};
  texHeader2_Version2 texHeader2_Version2_{};
  std::span<const char> paletteData{};
  std::span<const char> imageData{};
  [[nodiscard]] color32<> GetColorFromPalette(std::uint32_t row, std::uint8_t key) const
  {
    if (texHeader_.NUM_PALETTES == 0 || key > texHeader_.NUM_COLORS_PER_PALETTE) {
      return {};
    }
    if (row > texHeader_.NUM_PALETTES) {
      row = 0U;
    }

    // const auto offset = (texHeader_.NUM_COLORS_PER_PALETTE * row + key) * sizeof(color32<>);
    const auto paletteSpan = std::span(reinterpret_cast<const color32<2, 1, 0, 3> *>(std::ranges::data(paletteData)),
      texHeader_.NUM_COLORS_PER_PALETTE * texHeader_.NUM_PALETTES)
                               .subspan(row * texHeader_.NUM_COLORS_PER_PALETTE);
    // auto paletteSpan = paletteData.subspan(offset);
    color32<> ret{ paletteSpan[key] };
    //    ret.B(paletteSpan[key].B());
    //    ret.G(paletteSpan[key].G());
    //    ret.R(paletteSpan[key].R());
    //    ret.A(paletteSpan[key].A());
    return ret;
  }


  [[nodiscard]] auto sizeOfPalette() const noexcept
  {
    return texHeader_.PALETTE_SIZE * sizeof(std::uint32_t);
  }// I think this is correct.
  [[nodiscard]] auto PaletteLocator() const noexcept
  {
    static constexpr auto version1size = 0xECU;
    static constexpr auto version2size = 0xF0U;
    if (texHeader_.VERSION >= 2) {
      return version2size;
    }
    return version1size;
  }
  [[nodiscard]] auto TextureLocator() const noexcept { return sizeOfPalette() + PaletteLocator(); }

public:
  tex() = default;
  [[maybe_unused]] explicit tex(std::span<const char> buffer)
  { //TODO add validation checks.
    auto bufferbak = buffer;
    const auto process = [&buffer](auto &member) {
      const auto localSpan = buffer.subspan(0, sizeof(member));
      std::memcpy(&member, std::ranges::data(localSpan), sizeof(member));
      buffer = buffer.subspan(sizeof(member));
    };
    process(texHeader_);
    process(texPixelFormatHeader_);
    process(texHeader2_);
    if (texHeader_.VERSION >= 2) {
      process(texHeader2_Version2_);
    }
    //std::cout << std::hex << TextureLocator() << ", " << PaletteLocator() << std::dec << '\n';
    if (texHeader_.PALETTE_FLAG != 0U) {
      paletteData = bufferbak.subspan(PaletteLocator(), sizeOfPalette());
    }
    imageData = bufferbak.subspan(TextureLocator());
  }
  [[maybe_unused]] [[nodiscard]] std::vector<color32<>> GetColors([[maybe_unused]] std::uint32_t paletteRow = 0U) const
  {

    std::vector<color32<>> ret{};
    ret.reserve(texHeader_.IMAGE_WIDTH * texHeader_.IMAGE_WIDTH);
    if (texHeader_.PALETTE_FLAG != 0U) {
      for (const auto &i : imageData) {
        ret.emplace_back(GetColorFromPalette(paletteRow, static_cast<std::uint8_t>(i)));
      }
    } else {
      switch (texHeader_.BITS_PER_PIXEL) {
      case 16U: {
        //        std::vector<color16> tmp(std::ranges::size(ret));
        //        auto size_ = std::min(
        //          std::min(texHeader_.IMAGE_HEIGHT * texHeader_.IMAGE_WIDTH * sizeof(color16),
        //          std::ranges::size(imageData)), std::ranges::size(tmp) * sizeof(color16));
        //        std::memcpy(std::ranges::data(tmp), std::ranges::data(imageData), size_);
        //
        //        for (const auto &i : tmp) { ret.emplace_back(i); }
        auto c16data = std::span(reinterpret_cast<const color16 *>(std::ranges::data(imageData)),
          std::ranges::size(imageData) / sizeof(color16));
        for (const auto &i : c16data) { ret.emplace_back(i); }
        break;
      }
        //      case 24: {
        //        break;
        //      }
      case 32U: {// untested
        auto c32data = std::span(reinterpret_cast<const color32<2, 1, 0, 3> *>(std::ranges::data(imageData)),
          std::ranges::size(imageData) / sizeof(color32<2, 1, 0, 3>));
        for (const auto &i : c32data) { ret.emplace_back(i); }
        break;
      }
      }
    }
    return ret;
  };
  [[maybe_unused]] void Save(std::string_view filename) const
  {
    if (texHeader_.NUM_PALETTES == 0) {
      ppm::save(GetColors(), texHeader_.IMAGE_WIDTH, texHeader_.IMAGE_HEIGHT, filename);
    } else {
      auto path = std::filesystem::path(filename);
      for (std::uint16_t i = 0; i < texHeader_.NUM_PALETTES; i++) {
        auto ss = std::stringstream{};
        ss << (path.parent_path() / path.stem()).string() << '_' << i << path.extension().string();
        ppm::save(GetColors(i), texHeader_.IMAGE_WIDTH, texHeader_.IMAGE_HEIGHT, ss.str());
      }
    }
  }
  friend std::ostream & operator << (std::ostream &os, const tex &t)
  {
    return os <<"{Version: " << t.texHeader_.VERSION <<", BPP: " << t.texHeader_.BITS_PER_PIXEL << ", Palette Count: "<< t.texHeader_.NUM_PALETTES <<", Width: "<< t.texHeader_.IMAGE_WIDTH << ", Height: "<< t.texHeader_.IMAGE_HEIGHT<<"}\n";
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_TEX_H
