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
    std::error_code ec{};
    const bool      found = std::filesystem::exists(in_path, ec);
    if (ec) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << ec.value() << ": " << ec.message() << ec.value()
                << " - path: " << in_path << std::endl;
      ec.clear();
    }
    if (found) {
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
  const auto mim     = open_viii::graphics::background::Mim{ mim_path };
  const auto process = [&map_path](open_viii::graphics::background::Map map) {
    std::string out_path = map_path.string();
    switch (map.get_type()) {
    case 0: {
      out_path += ".1";
    } break;
    case 1: {
      out_path += ".2";
    } break;
    case 2: {
      out_path += ".3";
    } break;
    }
    out_path += std::string(".csv");
    map.save_csv(out_path);
  };
  process(open_viii::graphics::background::Map{ mim.mim_type(),
                                                map_path,
                                                false });
}
