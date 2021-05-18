//
// Created by pcvii on 5/17/2021.
//

#include "fiflfs_create.hpp"

int
  main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  const auto check = [](std::string_view in, bool create = false) {
    if (std::filesystem::exists(in)) {
      return in;
    }
    if (create) {
      std::cout << "created directory\n";
      std::filesystem::create_directories(in);
      return in;
    }
    std::cerr << in << " is not a valid path..." << std::endl;
    return std::string_view{};
  };
  if (argc >= 4) {
    const auto src      = check(argv[1]);
    const auto dst      = check(argv[2], true);
    const auto dst_name = std::string_view(argv[3]);
    if (std::empty(src) || std::empty(dst)) {
      std::cerr << "fiflfs_create source destination archive\n";
      return 1;
    }
    fiflfs_create(src, dst, dst_name);
    return 0;
  }
  const auto get_path = [](std::string_view msg,
                           bool             create = false) -> std::string {
    std::string temp;
    while (true) {
      std::cout << msg << std::flush;
      std::getline(std::cin, temp);
      if (std::filesystem::exists(temp)) {
        break;
      }
      if (create) {
        std::cout << "created directory\n";
        std::filesystem::create_directories(temp);
        break;
      }
      std::cout << temp << " is not a valid path..." << std::endl;
    }
    return temp;
  };
  const auto get_name = [](std::string_view msg) -> std::string {
    std::string temp;
    std::cout << msg << std::flush;
    std::getline(std::cin, temp);
    return temp;
  };
  const std::string src      = get_path("Source directory:");
  const std::string dst      = get_path("Destination path:", true);
  const std::string dst_name = get_name("Archive name:");
  fiflfs_create(src, dst, dst_name);
  return 0;
}