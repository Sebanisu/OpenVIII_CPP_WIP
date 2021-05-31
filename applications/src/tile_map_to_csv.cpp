//
// Created by pcvii on 5/31/2021.
// Goal to convert tile map to a workable csv file.
// And have a way to convert it back later.
//
#include "tile_map_to_csv.hpp"
#include <variant>
int
  main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  if (argc != 3) {
    return EXIT_FAILURE;
  }
  const auto            mim_path  = std::filesystem::path(argv[1]);
  static constexpr auto mim_ext   = std::string_view(".mim");
  const auto            map_path  = std::filesystem::path(argv[2]);
  static constexpr auto map_ext   = std::string_view(".map");
  const auto            check_ext = [](const std::filesystem::path &in_path,
                            const std::string_view       ext) {
    std::cout << "path: \"" << in_path.string() << "\"\n";
    const auto path_ext = in_path.extension().string();
    if (!std::filesystem::exists(in_path)) {
      std::cout << "does not exist!\n";
      exit(EXIT_FAILURE);
    }
    if (!in_path.has_extension()
        || !open_viii::tools::i_equals(path_ext, ext)) {
      std::cout << "wrong extension, expected: " << ext;
      exit(EXIT_FAILURE);
    }
  };
  check_ext(mim_path, mim_ext);
  check_ext(map_path, map_ext);
  const auto mim = open_viii::graphics::background::Mim{ mim_path };
  const auto process =
    [&map_path](std::variant<open_viii::graphics::background::Map<
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
  } else if (mim.mim_type().type() == 2) {
    process(open_viii::graphics::background::Map<
            open_viii::graphics::background::Tile2>{ map_path, false, false });
  } else if (mim.mim_type().type() == 3) {
    process(open_viii::graphics::background::Map<
            open_viii::graphics::background::Tile3>{ map_path, false, false });
  }

  return EXIT_SUCCESS;
}