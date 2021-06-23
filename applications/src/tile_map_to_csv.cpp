//
// Created by pcvii on 5/31/2021.
// Goal to convert tile map to a workable csv file.
// And have a way to convert it back later.
//
#include "tile_map_to_csv.hpp"
#include <variant>
void
  map_to_csv(const std::filesystem::path &mim_path,
             const std::filesystem::path &map_path);
int
  main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  static constexpr auto mim_ext   = std::string_view(".mim");
  static constexpr auto map_ext   = std::string_view(".map");
  const auto            check_ext = [](const std::filesystem::path &in_path,
                            const std::string_view       ext) -> bool {
    std::cout << "path: \"" << in_path.string() << "\"\n";
    const auto path_ext = in_path.extension().string();
    if (!std::filesystem::exists(in_path)) {
      std::cout << "does not exist!\n";
      return false;
    }
    if (!in_path.has_extension()
        || !open_viii::tools::i_equals(path_ext, ext)) {
      std::cout << "wrong extension, expected: " << ext;
      return false;
    }
    return true;
  };
  if (argc != 3) {
    std::cout << "`Tile, map to csv tool` by: Sebanisu\n\n"
                 "You may use command line:"
                 "\n\ttile_map_to_cvs path_to_mim path_to_map"
                 "\nPress control+c to exit.\n\n";

    std::string mim_path_string{};
    do {
      std::cout << "Path to " << mim_ext << " file:\n";
      std::cin >> mim_path_string;
    }
    while (!check_ext(mim_path_string, mim_ext));
    std::string map_path_string{};
    do {
      std::cout << "Path to " << map_ext << " file:\n";
      std::cin >> map_path_string;
    }
    while (!check_ext(map_path_string, map_ext));
    map_to_csv(mim_path_string, map_path_string);
    return EXIT_SUCCESS;
  }
  const auto mim_path = std::filesystem::path(argv[1]);
  if (!check_ext(mim_path, mim_ext))
    return EXIT_FAILURE;
  const auto map_path = std::filesystem::path(argv[2]);
  if (!check_ext(map_path, map_ext))
    return EXIT_FAILURE;
  map_to_csv(mim_path, map_path);
  return EXIT_SUCCESS;
}
void
  map_to_csv(const std::filesystem::path &mim_path,
             const std::filesystem::path &map_path)
{
  const auto mim = open_viii::graphics::background::Mim{ mim_path };
  const auto process
    = [&map_path](std::variant<open_viii::graphics::background::Map<
                                 open_viii::graphics::background::Tile1>,
                               open_viii::graphics::background::Map<
                                 open_viii::graphics::background::Tile2>,
                               open_viii::graphics::background::Map<
                                 open_viii::graphics::background::Tile3>> map) {
        switch (map.index()) {
        case 0: {
          const auto out_path = map_path.string() + ".1" + std::string(".csv");
          std::get<0>(map).save_csv(out_path);
        } break;
        case 1: {
          const auto out_path = map_path.string() + ".2" + std::string(".csv");
          std::get<1>(map).save_csv(out_path);
        } break;
        case 2: {
          const auto out_path = map_path.string() + ".3" + std::string(".csv");
          std::get<2>(map).save_csv(out_path);
        } break;
        }
      };
  if (mim.mim_type().type() == 1) {
    process(open_viii::graphics::background::Map<
            open_viii::graphics::background::Tile1>{ map_path, false, false });
  }
  else if (mim.mim_type().type() == 2) {
    process(open_viii::graphics::background::Map<
            open_viii::graphics::background::Tile2>{ map_path, false, false });
  }
  else if (mim.mim_type().type() == 3) {
    process(open_viii::graphics::background::Map<
            open_viii::graphics::background::Tile3>{ map_path, false, false });
  }
}
