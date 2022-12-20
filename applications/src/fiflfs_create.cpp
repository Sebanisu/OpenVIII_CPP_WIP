//
// Created by pcvii on 5/17/2021.
//

#include "fiflfs_create.hpp"

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
          std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                    << ec.value() << ": " << ec.message() << ec.value()
                    << " - path: " << temp << std::endl;
          ec.clear();
          if (created) {
            std::cout << "created directory " << temp << "\n";
          }
        }
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