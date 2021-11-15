// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
#ifndef VIIIARCHIVE_TESTPATHS_H
#define VIIIARCHIVE_TESTPATHS_H
#include "open_viii/tools/Tools.hpp"
#include "tl/string.hpp"
#include <algorithm>
#include <filesystem>
#include <ranges>
#include <string>
#include <vector>
namespace open_viii {
struct Paths
{
public:
  static auto &
    get()
  {
    using namespace std::literals::string_literals;
    static std::vector paths = {
      tl::string::replace_slashes(
        R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)"s),
      tl::string::replace_slashes(
        R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII Remastered)"s),

      tl::string::replace_slashes(
        R"(D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII Remastered)"s),
      tl::string::replace_slashes(
        R"(/mnt/d/SteamLibrary/steamapps/common/FINAL FANTASY VIII Remastered)"s),
#if 1
      tl::string::replace_slashes(
        R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII)"s),
      tl::string::replace_slashes(
        R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII)"s),

      tl::string::replace_slashes(
        R"(D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII)"s),
      tl::string::replace_slashes(
        R"(/mnt/d/SteamLibrary/steamapps/common/FINAL FANTASY VIII)"s),

      tl::string::replace_slashes(R"(/mnt/k/ff82000)"s),
      tl::string::replace_slashes(R"(K:\ff82000)"s),

      tl::string::replace_slashes(R"(/mnt/d/games/ff82000)"s),
      tl::string::replace_slashes(R"(D:\games\ff82000)"s),

      // CD
      tl::string::replace_slashes(R"(/mnt/e/)"s),
      tl::string::replace_slashes(R"(e:\)"s),

      // folders with tim files in it
      tl::string::replace_slashes(R"(/mnt/d/tim)"s),
      tl::string::replace_slashes(R"(d:\tim)"s)
#endif
    };
    static const auto path_file
      = std::filesystem::current_path() / "paths.conf";
    static bool read_file = true;
    if (read_file) {
      auto fs = std::ifstream(path_file, std::ios::in | std::ios::binary);
      if (fs.is_open()) {
        read_file = false;
        std::string line{};
        while (std::getline(fs, line)) {
          paths.emplace_back(tl::string::replace_slashes(std::move(line)));
        }
      }
    }
    const auto [first, last]
      = std::ranges::remove_if(paths, [](const std::filesystem::path path) {
          std::error_code ec{};
          const bool      found = std::filesystem::exists(path, ec);
          if (ec) {
            std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                      << ec.value() << ": " << ec.message() << ec.value()
                      << " - path: " << path << std::endl;
            ec.clear();
          }
          const bool is_dir = std::filesystem::is_directory(path, ec);
          if (ec) {
            std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                      << ec.value() << ": " << ec.message()
                      << " - path: " << path << std::endl;
            ec.clear();
          }
          return !found || !is_dir;
        });
    paths.erase(first, last);
    return paths;
  }
  template<std::invocable<std::filesystem::path> lambdaT>
  static auto
    for_each_path(const lambdaT &lambda)
  {
    std::ranges::for_each(get(), [&lambda](const std::string &path) {
      const auto      fs_path = std::filesystem::path(path);
      std::error_code ec{};
      const bool      found = std::filesystem::exists(fs_path, ec);
      if (ec) {
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << ec.value() << ": " << ec.message() << std::endl;
        ec.clear();
      }
      const bool is_dir = std::filesystem::is_directory(fs_path, ec);
      if (ec) {
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << ec.value() << ": " << ec.message() << std::endl;
        ec.clear();
      }
      if (found && is_dir) {
        lambda(fs_path);
      }
    });
  }
};

}// namespace open_viii
#endif// VIIIARCHIVE_TESTPATHS_H
