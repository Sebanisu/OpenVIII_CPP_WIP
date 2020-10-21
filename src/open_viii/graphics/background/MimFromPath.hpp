//
// Created by pcvii on 10/5/2020.
//

#ifndef VIIIARCHIVE_MIMFROMPATH_HPP
#define VIIIARCHIVE_MIMFROMPATH_HPP
#include "Mim.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include <array>
namespace open_viii::graphics::background {
/**
 * This surfaces the same interface as Mim but the backend will handle multiple images located in path.
 */
struct MimFromPath
{
private:
  struct TexturesByPalettes
  {
  public:
    static constexpr auto MAX_PALETTES_PLUS_1 = 18U;
    auto &at(const size_t &palette) const
    {
      return m_palettes.at(palette);
    }

    void set(const size_t &palette, Ppm &&value) const
    {
      m_palettes.at(palette) = std::move(value);
    }

  private:
    mutable std::array<Ppm, MAX_PALETTES_PLUS_1> m_palettes{};
  };

  struct PalettesByTexturePages
  {

  public:
    static constexpr auto MAX_TEXTURE_PAGES = 14U;
    auto &at(const size_t &texture_page) const
    {
      return m_texture_pages.at(texture_page);
    }
    auto &at(const size_t &texture_page, const size_t &palette) const
    {
      return m_texture_pages.at(texture_page).at(palette);
    }

    void set(const size_t &texture_page, const size_t &palette, Ppm &&value) const
    {
      m_texture_pages.at(texture_page).set(palette, std::move(value));
    }

  private:
    mutable std::array<TexturesByPalettes, MAX_TEXTURE_PAGES> m_texture_pages{};
  };
  static constexpr auto DEFAULT_PALETTE = TexturesByPalettes::MAX_PALETTES_PLUS_1 - 1;
  const MimType m_mim_type{};
  const std::filesystem::path &m_dir_path{};
  const std::string_view m_dir_name{};
  const std::string &m_output_prefix{};
  const PalettesByTexturePages m_textures{};

  auto get_textures() const
  {
    PalettesByTexturePages textures{};
    open_viii::Tools::execute_on_directory(m_dir_path,
      { m_dir_name },
      { ".ppm" },
      [textures, this]([[maybe_unused]] const std::filesystem::path &file_path) {
        const auto filename = file_path.filename().stem().string();
        if (!filename.ends_with(')')) {
          return;
        }
        const auto view_of_filename = std::string_view(filename);// substring of a view is better than string.
        const auto suffix = view_of_filename.substr(std::ranges::size(m_dir_name));
        if (!suffix.starts_with('(')) {
          return;
        }
        std::size_t last = 1;
        constexpr auto search_params = ")_";
        std::size_t found = suffix.find_first_of(search_params, last);
        std::uint8_t texture_page{};
        std::uint8_t palette{ static_cast<std::uint8_t>(DEFAULT_PALETTE) };
        for (size_t i = 0; found != std::string::npos; i++) {
          auto number = suffix.substr(last, found - last);
          constexpr static auto base = 10;
          switch (i) {
          case 0: {
            texture_page = static_cast<std::uint8_t>(std::strtol(std::ranges::data(number), nullptr, base));
          } break;
          case 1: {
            palette = static_cast<std::uint8_t>(std::strtol(std::ranges::data(number), nullptr, base));
          } break;
          }
          // maybe should throw if not 0 or 1.
          last = found + 1;
          found = suffix.find_first_of(search_params, last);
        }
        std::cout << "texture page: " << static_cast<uint16_t>(texture_page)
                  << "\t palette: " << static_cast<uint16_t>(palette) << '\n';
        // Load the image into the correct part of array.
        textures.set(texture_page, palette, Ppm(file_path));
      });
    return textures;
  }

public:
  MimFromPath() = default;
  explicit MimFromPath(MimType mim_type,
    const std::filesystem::path &dir_path,
    const std::string_view &dir_name,
    const std::string &output_prefix)
    : m_mim_type(mim_type),
      m_dir_path(dir_path),
      m_dir_name(dir_name),
      m_output_prefix(output_prefix),
      m_textures(get_textures())
  {}
  std::size_t get_raw_width(const BPPT &depth) const
  {
    return Mim::get_raw_width(depth, m_mim_type.width());
  }
  Color auto get_color([[maybe_unused]] const std::size_t &x,
    [[maybe_unused]] const std::size_t &y,
    [[maybe_unused]] const BPPT &depth,
    [[maybe_unused]] const std::uint8_t &palette,
    [[maybe_unused]] const std::uint8_t &texture_id) const
  {
    const auto &palette_texture = m_textures.at(texture_id, palette);
    if (!palette_texture.empty()) {
      const auto color = palette_texture.color(x, y);

      if (!color.is_black()) {
        return color;
      }
    }
    const auto &default_texture = m_textures.at(texture_id, DEFAULT_PALETTE);
    return default_texture.color(x, y);
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_MIMFROMPATH_HPP
