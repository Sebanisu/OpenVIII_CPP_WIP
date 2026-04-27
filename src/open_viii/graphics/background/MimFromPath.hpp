//
// Created by pcvii on 10/5/2020.
//
#ifndef VIIIARCHIVE_MIMFROMPATH_HPP
#define VIIIARCHIVE_MIMFROMPATH_HPP
#include "Mim.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include <fmt/format.h>
namespace open_viii::graphics::background {

struct TexturesByPalettes
{
  static constexpr auto                        MAX_PALETTES_PLUS_1 = 18U;

  mutable std::array<Ppm, MAX_PALETTES_PLUS_1> palettes{};

  [[nodiscard]] auto &
    at(size_t palette) const
  {
    return palettes.at(palette);
  }

  void
    set(size_t palette, Ppm &&value) const
  {
    palettes.at(palette) = std::move(value);
  }
};
}// namespace open_viii::graphics::background

template<>
struct fmt::formatter<open_viii::graphics::background::TexturesByPalettes>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const auto &data, FormatContext &ctx) const
  {
    auto out = ctx.out();
    for (const auto &ppm : data.palettes) {
      if (!ppm.empty()) {
        out = fmt::format_to(out, "{}", ppm);
      }
    }
    return out;
  }
};
namespace open_viii::graphics::background {
struct PalettesByTexturePages
{
  static constexpr auto MAX_TEXTURE_PAGES = 14U;

  mutable std::array<TexturesByPalettes, MAX_TEXTURE_PAGES> texture_pages{};

  [[nodiscard]] auto &
    at(size_t texture_page) const
  {
    return texture_pages.at(texture_page);
  }

  [[nodiscard]] auto &
    at(size_t texture_page, size_t palette) const
  {
    return texture_pages.at(texture_page).at(palette);
  }

  void
    set(size_t texture_page, size_t palette, Ppm &&value) const
  {
    texture_pages.at(texture_page).set(palette, std::move(value));
  }
};
}// namespace open_viii::graphics::background
template<>
struct fmt::formatter<open_viii::graphics::background::PalettesByTexturePages>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const auto &data, FormatContext &ctx) const
  {
    auto out = ctx.out();
    for (const auto &page : data.texture_pages) {
      out = fmt::format_to(out, "{}", page);
    }
    return out;
  }
};
namespace open_viii::graphics::background {
struct MimFromPath
{
  static constexpr auto DEFAULT_PALETTE
    = TexturesByPalettes::MAX_PALETTES_PLUS_1 - 1;

  MimType                      mim_type{};
  const std::filesystem::path &dir_path{};
  std::string_view             dir_name{};
  PalettesByTexturePages       textures{};

private:
  auto
    get_textures() const
  {
    PalettesByTexturePages textures{};

    open_viii::tools::execute_on_directory(
      dir_path,
      { dir_name },
      { ".ppm" },
      [&textures, this](const std::filesystem::path &file_path) {
        const auto filename = file_path.filename().stem().string();

        if (!filename.ends_with(')')) {
          return;
        }

        const auto view   = std::string_view(filename);
        const auto suffix = view.substr(dir_name.size());

        if (!suffix.starts_with('(')) {
          return;
        }

        std::size_t    last   = 1;
        constexpr auto search = ")_";

        std::size_t    found  = suffix.find_first_of(search, last);

        std::uint8_t   texture_page{};
        std::uint8_t   palette{ static_cast<std::uint8_t>(DEFAULT_PALETTE) };

        for (size_t i = 0; found != std::string::npos; ++i) {
          auto          number = suffix.substr(last, found - last);

          constexpr int base   = 10;

          switch (i) {
          case 0:
            texture_page = static_cast<std::uint8_t>(
              std::strtol(number.data(), nullptr, base));
            break;

          case 1:
            palette = static_cast<std::uint8_t>(
              std::strtol(number.data(), nullptr, base));
            break;
          }

          last  = found + 1;
          found = suffix.find_first_of(search, last);
        }

        // Debug output (you may want to remove or convert to spdlog)
        fmt::print(
          "texture page: {:2} palette: {:2} path: \"{}\"\n",
          static_cast<uint16_t>(texture_page),
          static_cast<uint16_t>(palette),
          file_path.string());

        textures.set(texture_page, palette, Ppm(file_path));
      });

    fmt::print("{}\n", textures);

    return textures;
  }

public:
  MimFromPath() = default;

  explicit MimFromPath(
    MimType                      mim_type_,
    const std::filesystem::path &dir_path_,
    std::string_view             dir_name_,
    const std::string &)
    : mim_type(mim_type_), dir_path(dir_path_), dir_name(dir_name_),
      textures(get_textures())
  {}

  std::size_t
    get_raw_width(const BPPT &depth) const
  {
    return Mim::get_raw_width(depth, mim_type.width());
  }

  Color auto
    get_color(
      const std::size_t           &x,
      const std::size_t           &y,
      [[maybe_unused]] const BPPT &depth,
      const std::uint8_t          &palette,
      const std::uint8_t          &texture_id) const
  {
    const auto &palette_texture = textures.at(texture_id, palette);

    if (!palette_texture.empty()) {
      const auto color = palette_texture.color(x, y);
      if (!color.is_black()) {
        return color;
      }
    }

    const auto &fallback = textures.at(texture_id, DEFAULT_PALETTE);
    return fallback.color(x, y);
  }
};

}// namespace open_viii::graphics::background

template<>
struct fmt::formatter<open_viii::graphics::background::MimFromPath>
{
  constexpr auto
    parse(fmt::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  template<typename FormatContext>
  auto
    format(const auto &m, FormatContext &ctx) const
  {
    return fmt::format_to(
      ctx.out(),
      "{{dir: {}, mim_type: {}}}",
      m.dir_path.string(),
      m.mim_type);
  }
};
#endif// VIIIARCHIVE_MIMFROMPATH_HPP
