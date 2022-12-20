//
// Created by pcvii on 5/17/2021.
//
#include "fiflfs_extract.hpp"
int
  main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
  const auto check = [](std::string_view in, bool create = false) {
    std::error_code ec{};
    const bool      found = std::filesystem::exists(in, ec);
    if (ec) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << ec.value() << ": " << ec.message() << ec.value()
                << " - path: " << in << std::endl;
      ec.clear();
    }
    if (found) {
      return in;
    }
    if (create) {
      bool created = std::filesystem::create_directories(in, ec);
      if (ec) {
        std::cerr << __FILE__ << ":" << __LINE__ << " - " << ec.value() << ": "
                  << ec.message() << " - " << in << std::endl;
        ec.clear();
      }
      if (created) {
        std::cout << "created directory " << in << "\n";
        return in;
      }
    }
    std::cerr << in << " is not a valid path..." << std::endl;
    return std::string_view{};
  };
  if (argc >= 3) {
    const auto src = check(argv[1]);
    const auto dst = check(argv[2], true);
    if (std::empty(src) || std::empty(dst)) {
      std::cerr << "fiflfs_extract source destination\n";
      return 1;
    }
    fiflfs_extract(src, dst);
    return 0;
  }
  const auto get_path
    = [](std::string_view msg, bool create = false) -> std::string {
    std::string temp;
    while (true) {
      std::cout << msg << std::flush;
      std::getline(std::cin, temp);
      std::error_code ec{};
      const bool      found = std::filesystem::exists(temp, ec);
      if (ec) {
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << ec.value() << ": " << ec.message() << ec.value()
                  << " - path: " << temp << std::endl;
        ec.clear();
      }
      if (found) {
        break;
      }
      if (create) {
        bool created = std::filesystem::create_directories(temp, ec);
        if (ec) {
          std::cerr << __FILE__ << ":" << __LINE__ << " - " << ec.value() << ": "
                    << ec.message() << " - " << temp << std::endl;
          ec.clear();
        }
        if (created) {
          std::cout << "created directory " << temp << "\n";
          break;
        }
      }
      std::cout << temp << " is not a valid path..." << std::endl;
    }
    return temp;
  };
  const std::string src = get_path("Source fi/fl/fs file:");
  const std::string dst = get_path("Destination path:", true);
  fiflfs_extract(src, dst);
  return 0;
}