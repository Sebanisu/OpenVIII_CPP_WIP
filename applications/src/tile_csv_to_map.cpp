//
// Created by pcvii on 5/31/2021.
//

#include "tile_csv_to_map.hpp"
#include "open_viii/graphics/background/Map.hpp"
#include <charconv>
#include <ctre.hpp>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
void
  save_map(const std::variant<open_viii::graphics::background::Map<
                                open_viii::graphics::background::Tile1>,
                              open_viii::graphics::background::Map<
                                open_viii::graphics::background::Tile2>,
                              open_viii::graphics::background::Map<
                                open_viii::graphics::background::Tile3>> &map,
           const std::basic_string_view<char> &csv_path);
template<typename T>
requires(std::integral<std::decay_t<T>>)
  std::optional<std::decay_t<T>> get_number(const std::string_view num,
                                            const int              base = 10)
noexcept
{
  std::decay_t<T>        return_value{};
  std::from_chars_result data
    = std::from_chars(num.data(), num.data() + num.size(), return_value, base);
  if (data.ec == std::errc{})
    return return_value;
  // todo maybe log error here.
  return std::nullopt;
}
template<>
std::optional<bool>
  get_number<bool>(const std::string_view num, const int base) noexcept
{
  if(auto i = get_number<std::uint8_t>(num, base);i.has_value())
    return *i != 0;
  else
  return std::nullopt;
}

template<typename T>
requires(std::floating_point<std::decay_t<T>>)
  std::optional<std::decay_t<T>> get_number(const std::string_view  num,
                                            const std::chars_format fmt
                                            = std::chars_format::general)
noexcept
{
  std::decay_t<T>        return_value{};
  std::from_chars_result data
    = std::from_chars(num.data(), num.data() + num.size(), return_value, fmt);
  if (data.ec == std::errc{})
    return return_value;
  // todo maybe log error here.
  return std::nullopt;
}
template<typename K>
std::vector<char>
  out_hex_to_vector(K re_result)
{
  std::string_view  view = re_result.to_view();
  std::vector<char> vector;
  vector.reserve(view.size() / 2U);
  for (std::size_t i = 0; i != view.size(); i += 2) {
    auto       sv_num = view.substr(i, 2U);
    const auto value  = get_number<std::uint8_t>(sv_num, 16);
    if (value.has_value()) {
      vector.push_back(static_cast<char>(*value));
    }
    else
      break;
  }
  return vector;
}
template<typename T, typename retT>
void
  if_num_assign(retT &tile, std::string_view re, std::function<retT(T)> with)
{
  if (const auto num = get_number<T>(re); num.has_value())
    tile = with(*num);
}
// enum class lex_type
//{
//   unknown,
//   hex,
//   string,
//   number,
// };
// template<typename lambdaT>
// constexpr void
//   parse_csv_row(const std::string_view input, lambdaT lambda)
//{
//   using lex_pair = std::pair<lex_type, std::string_view>;
//   constexpr auto csv_pattern2
//     = ctll::fixed_string{ R"vv("0x([0-9A-Fa-f]+)",|"([^"]+)",|-?(\d+),)vv" };
//   for (const auto &match : ctre::range<csv_pattern2>(input)) {
//     const auto &[m, hex, string, number] = match;
//     if (!m)
//       continue;
//     if (hex)
//       lambda(lex_pair{ lex_type::hex, hex });
//     else if (string)
//       lambda(lex_pair{ lex_type::string, string });
//     else if (number)
//       lambda(lex_pair{ lex_type::number, number });
//     else
//       lambda(lex_pair{});
//   }
// }
int
  main([[maybe_unused]] const int         argc,
       [[maybe_unused]] const char *const argv[])
{
  if (argc != 2) {
    puts("Missing command-line argument\n\tRequires path to csv file.");
    return EXIT_FAILURE;
  }
  puts(argv[1]);
  auto       csv_path       = std::string_view(argv[1]);

  const auto tile_type_view = [&csv_path]() -> std::optional<std::string_view> {
    if (auto [whole, tile_type_match]
        = ctre::search<R"(\.([1-3])\.[cC][sS][vV])">(csv_path);
        whole) {
      std::cout << tile_type_match.to_string()
                << '\n';// using to_string here to force ctre to execute
                        // different code. should fail unless it's fixed.
      return tile_type_match.to_view();
    }
    return {};
  }();
  if (!tile_type_view.has_value()) {
    puts("Path doesn't match pattern! Should end with .[1-3].csv");
    return EXIT_FAILURE;
  }
  if (!std::filesystem::exists(csv_path)) {
    puts("Path doesn't exist");
    return EXIT_FAILURE;
  }
  auto is
    = std::fstream(std::string{ csv_path }, std::ios::in | std::ios::binary);
  [[maybe_unused]] std::string headers{};
  std::getline(is, headers);
  puts(headers.c_str());
  using variant_tiles = std::variant<open_viii::graphics::background::Tile1,
                                     open_viii::graphics::background::Tile2,
                                     open_viii::graphics::background::Tile3,
                                     std::monostate>;
  using variant_maps  = std::variant<open_viii::graphics::background::Map<
                                      open_viii::graphics::background::Tile1>,
                                    open_viii::graphics::background::Map<
                                      open_viii::graphics::background::Tile2>,
                                    open_viii::graphics::background::Map<
                                      open_viii::graphics::background::Tile3>>;
  const auto get_map  = [&tile_type_view](auto... data) -> variant_maps {
    switch ((*tile_type_view)[0]) {
    default:
    case '1':
      return open_viii::graphics::background::Map<
        open_viii::graphics::background::Tile1>(data...);
      break;
    case '2':
      return open_viii::graphics::background::Map<
        open_viii::graphics::background::Tile2>(data...);
      break;
    case '3':
      return open_viii::graphics::background::Map<
        open_viii::graphics::background::Tile3>(data...);
      break;
    }
  };
  const auto get_tile = [&tile_type_view](auto... data) -> variant_tiles {
    switch ((*tile_type_view)[0]) {
    case '1':
      return open_viii::graphics::background::Tile1(data...);
      break;
    case '2':
      return open_viii::graphics::background::Tile2(data...);
      break;
    case '3':
      return open_viii::graphics::background::Tile3(data...);
      break;
    }
    return std::monostate{};
  };

  const auto map = get_map([&get_tile, &is]() -> variant_tiles {
    // read line and update data.
    // if(vtile.index() == 4) return vtile;
    std::string line{};
    std::getline(is, line);
    if (std::empty(line))
      return std::monostate();
    //    parse_csv_row(
    //      line,
    //      [x = size_t{ 0 }](
    //        const std::pair<lex_type, std::string_view> &value) mutable {
    //        std::cout << x << "\t";
    //        switch (value.first) {
    //        case lex_type::hex:
    //          std::cout << "hex\t";
    //          break;
    //        case lex_type::string:
    //          std::cout << "string\t";
    //          break;
    //        case lex_type::number:
    //          std::cout << "number\t";
    //          break;
    //        case lex_type::unknown:
    //          ++x;
    //          return;
    //          break;
    //        }
    //        std::cout << value.second << '\n';
    //        ++x;
    //      });
    puts(line.c_str());
    static constexpr auto csv_pattern = ctll::fixed_string{
      R"vv((\d+),"?0x([0-9A-F]+)"?,(\d+),(\d+),"?([A-Za-z ]+)"?,(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(-?\d+),(-?\d+),(\d+))vv"
      //(?:(?<number>-?\d+),|"0x(?<hex>[0-9A-F]+)",|"(?<string>[A-Za-z ]+)",)
      //-?\d+(?=,)|(?!"0x)[0-9A-F]+(?=",)|(?!")[^"]+(?=",)
      //"0x([0-9A-Fa-f]+)",|"([^"]+)",|-?(\d+),
    };
    const auto results = ctre::search<csv_pattern>(line);

    if (!results) {
      puts("line didn't match pattern");
      exit(EXIT_FAILURE);
    }
    const auto raw_bytes    = out_hex_to_vector(results.get<2>());
    auto       variant_tile = get_tile(raw_bytes);
    std::visit(
      [&](auto &tile) {
        if constexpr (!open_viii::is_monostate<decltype(tile)>) {
          [[maybe_unused]] const auto
            &[re_whole_line,
              re_index,  // index where it would be stored.
              re_raw_hex,// original tile in hex
              re_draw,
              re_depth,
              re_blend_mode,
              re_blend,
              re_layer_id,
              re_texture_id,
              re_palette_id,
              re_animation_id,
              re_animation_state,
              re_source_x,
              re_source_y,
              re_x,
              re_y,
              re_z]
            = results;
#define common_code(arg)                                                       \
  if (const auto num = get_number<decltype(tile.arg())>(re_##arg);             \
      num.has_value())                                                         \
  tile = tile.with_##arg(*num)
#define concept_common_code(arg)                                               \
  if constexpr (open_viii::graphics::background::has_with_##arg<               \
                  decltype(tile)>)                                             \
  common_code(arg)
          common_code(z);
          common_code(y);
          common_code(x);
          common_code(source_y);
          common_code(source_x);
          common_code(palette_id);
          common_code(texture_id);
          common_code(draw);
          common_code(blend);
          concept_common_code(animation_state);
          concept_common_code(animation_id);
          concept_common_code(layer_id);
#undef concept_common_code
#undef common_code
          if constexpr (open_viii::graphics::background::has_with_blend_mode<
                          decltype(tile)>) {
            tile = tile.with_blend_mode([](const std::string_view sv) {
              if (open_viii::tools::i_equals(sv, "Half Add"))
                return open_viii::graphics::background::BlendModeT::half_add;
              if (open_viii::tools::i_equals(sv, "Add"))
                return open_viii::graphics::background::BlendModeT::add;
              if (open_viii::tools::i_equals(sv, "Subtract"))
                return open_viii::graphics::background::BlendModeT::subtract;
              if (open_viii::tools::i_equals(sv, "Quarter Add"))
                return open_viii::graphics::background::BlendModeT::quarter_add;
              return open_viii::graphics::background::BlendModeT::none;
            }(re_blend_mode.to_view()));
          }

          tile = tile.with_depth(
            [](const std::string_view sv) -> open_viii::graphics::BPPT {
              using namespace open_viii::graphics::literals;
              if (const auto num = get_number<int>(sv); num.has_value()) {
                switch (*num) {
                case 4:
                  break;
                case 8:
                  return 8_bpp;
                case 16:
                  return 16_bpp;
                case 24:
                  return 24_bpp;
                }
              }
              return 4_bpp;
            }(re_depth.to_view()));
          std::cout << tile << '\n';
        }
      },
      variant_tile);

    return variant_tile;
  });
  save_map(map, csv_path);
  return EXIT_SUCCESS;
}
void
  save_map(const std::variant<open_viii::graphics::background::Map<
                                open_viii::graphics::background::Tile1>,
                              open_viii::graphics::background::Map<
                                open_viii::graphics::background::Tile2>,
                              open_viii::graphics::background::Map<
                                open_viii::graphics::background::Tile3>> &map,
           const std::basic_string_view<char> &csv_path)
{
  std::visit(
    [&csv_path](const auto &m) {
      m.save_map(csv_path);
    },
    map);
}
