//
// Created by pcvii on 5/31/2021.
//

#include "tile_csv_to_map.hpp"
#include "open_viii/graphics/background/Map.hpp"
#include <charconv>
#include <ctre.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>
template<typename T>
void
  get_number(std::string_view num, T &result, int base = 10)
{
  [[maybe_unused]] auto data =
    std::from_chars(num.data(), num.data() + num.size(), result, base);
}
template<typename T = std::int32_t, typename K>
T
  out_num(K re_result)
{
  T index;
  get_number(re_result.to_view(), index);
  std::cout << +index << '\n';
  return index;
}
template<typename K>
std::vector<char>
  out_hex_to_vector(K re_result)
{
  std::string_view  view = re_result.to_view();
  std::vector<char> vector;
  vector.reserve(view.size() / 2U);
  std::cout << view << '\n';
  for (std::size_t i = 0; i != view.size(); i += 2) {
    std::uint8_t value{};
    auto         sv_num = view.substr(i, 2U);
    get_number(sv_num, value, 16);
    std::cout << sv_num << " >> " << static_cast<int>(value) << std::endl;
    vector.push_back(static_cast<char>(value));
  }
  return vector;
}
int
  main([[maybe_unused]] const int         argc,
       [[maybe_unused]] const char *const argv[])
{
  if (argc != 2) {
    puts("Missing command-line argument\n\tRequires path to csv file.");
    return EXIT_FAILURE;
  }
  puts(argv[1]);
  auto csv_path = std::string_view(argv[1]);

  const auto tile_type_view = [&csv_path]() -> std::optional<std::string_view> {
    if (auto [whole, tile_type_match] =
          ctre::search<R"(\.([1-3])\.[cC][sS][vV])">(csv_path);
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
  auto is =
    std::fstream(std::string{ csv_path }, std::ios::in | std::ios::binary);
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
    puts(line.c_str());
    static constexpr auto csv_pattern = ctll::fixed_string{
      R"vv((\d+),"0x([0-9A-F]+)",(\d+),(\d+),"([A-Za-z ]+)",(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(\d+),(-?\d+),(-?\d+),(\d+))vv"
    };
    const auto results = ctre::search<csv_pattern>(line);

    if (!results) {
      puts("line didn't match pattern");
      exit(EXIT_FAILURE);
    }
    //                [re_whole,
    //                re_index,
    //                re_raw_hex,
    //                re_draw,
    //                re_bpp,
    //                re_blend,
    //                re_blend2,
    //                re_layer,
    //                re_texture_page,
    //                re_palette,
    //                re_animation,
    //                re_frame,
    //                re_source_x,
    //                re_source_y,
    //                re_x,
    //                re_y,
    //                re_z]
    out_num<std::int32_t>(results.get<1>());
    //              << raw_hex.to_view() << '\n'
    //              << draw.to_view() << '\n'
    //              << bpp.to_view() << '\n'
    //              << blend.to_view() << '\n'
    //              << blend2.to_view() << '\n'
    //              << layer.to_view() << '\n'
    //              << texture_page.to_view() << '\n'
    //              << palette.to_view() << '\n'
    //              << animation.to_view() << '\n'
    //              << frame.to_view() << '\n'
    //              << source_x.to_view() << '\n'
    //              << source_y.to_view() << '\n'
    //              << x.to_view() << '\n'
    //              << y.to_view() << '\n'
    //              << z.to_view() << '\n';

    const auto raw_bytes = out_hex_to_vector(results.get<2>());
    auto       vtile     = get_tile(raw_bytes);
    std::visit(
      [&results](auto &tile) {
        if constexpr (!std::is_same_v<std::decay_t<decltype(tile)>,
                                      std::monostate>) {
          [[maybe_unused]] const auto &[re_whole,
                                        re_index,
                                        re_raw_hex,
                                        re_draw,
                                        re_bpp,
                                        re_blend_mode,
                                        re_blend_other,
                                        re_layer,
                                        re_texture_page,
                                        re_palette,
                                        re_animation,
                                        re_frame,
                                        re_source_x,
                                        re_source_y,
                                        re_x,
                                        re_y,
                                        re_z] = results;
          tile = tile.with_draw(re_draw.to_view().at(0) == '1');
          tile =
            tile.with_blend(out_num<decltype(tile.blend())>(re_blend_other));
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
          tile = tile.with_depth(
            [](const std::string_view sv) -> open_viii::graphics::BPPT {
              using namespace open_viii::graphics::literals;
              switch (sv.at(0)) {
              case '4':
              default:
                return 4_bpp;
              case '8':
                return 8_bpp;
                break;
              case '1':
                if (sv.at(1) == '6')
                  return 16_bpp;
                return 4_bpp;
                break;
              case '2':
                if (sv.at(1) == '4')
                  return 24_bpp;
                return 4_bpp;
                break;
              }
            }(re_bpp.to_view()));

          std::cout << tile.output_rectangle() << ' ' << tile.source_rectangle()
                    << ' ' << tile.z() << ' ' << tile.depth() << '\n';
        }
      },
      vtile);

    return vtile;
    // return std::monostate{};
  });
  return EXIT_SUCCESS;
}