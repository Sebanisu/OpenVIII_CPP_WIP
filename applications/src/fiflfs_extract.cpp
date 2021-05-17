//
// Created by pcvii on 5/17/2021.
//
#include "fiflfs_extract.hpp"
#include <iostream>
#include <string_view>

int
  main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  const auto check = [](std::string_view in, bool create = false) {
    if (std::filesystem::exists(in)) {
      return in;
    }
    if(create)
    {
      std::cout << "created directory\n";
      std::filesystem::create_directories(in);
      return in;
    }
    std::cerr << in << " is not a valid path..." << std::endl;
    return std::string_view{};
  };
  if (argc >= 3) {
    const auto src = check(argv[1]);
    const auto dst = check(argv[2], true);
    if(std::empty(src) || std::empty(dst)) {
      std::cerr << "fiflfs_extract source destination\n";
      return 1;
    }
    extract_fiflfs(src, dst);
    return 0;
  }
  const auto get_path = [](std::string_view msg, bool create = false) -> std::string {
    std::string temp;
    while (true) {
      std::cout << msg << std::flush;
      std::getline(std::cin, temp);
      if (std::filesystem::exists(temp)) {
        break;
      }
      if(create)
      {
        std::cout << "created directory\n";
        std::filesystem::create_directories(temp);
        break;
      }
      std::cout << temp << " is not a valid path..." << std::endl;
    }
    return temp;
  };
  const std::string src = get_path("Source fi/fl/fs file:");
  const std::string dst = get_path("Destination path:", true);
  extract_fiflfs(src, dst);
  return 0;
}