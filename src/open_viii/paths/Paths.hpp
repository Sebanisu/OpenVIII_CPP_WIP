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
#include <algorithm>
#include <array>
#include <filesystem>
#include <ranges>
#include <string>
namespace open_viii {
struct Paths
{
public:
  static auto &get()
  {
    using namespace std::literals::string_literals;
    static std::array paths = {
      // R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII/Data/lang-en)"s,
      // R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII\Data\lang-en)"s,
      R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII)"s,
      R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII)"s,
      R"(/mnt/k/ff82000)"s,
      R"(K:\ff82000)"s,
      R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)"s,
      R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII Remastered)"s
    };
    return paths;
  }
  template<std::invocable<std::filesystem::path> lambdaT> static auto for_each_path(const lambdaT &lambda)
  {
    std::ranges::for_each(get(), [&lambda](std::string &path) {
      open_viii::Tools::replace_slashes(path);
      const auto fs_path = std::filesystem::path(path);
      if (std::filesystem::exists(fs_path)) {
        lambda(fs_path);
      }
    });
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_TESTPATHS_H
