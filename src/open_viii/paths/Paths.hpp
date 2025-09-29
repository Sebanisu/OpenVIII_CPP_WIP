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
#include <filesystem>
#include <functional>
#include <iostream>
#include <system_error>
#include <utility>
namespace open_viii {

struct Paths
{
public:
  enum struct Ops : std::uint8_t
  {
    Stop,
    Continue
  };
  static std::vector<std::filesystem::path>
    get_windows_ff8_paths();
  static std::vector<std::filesystem::path> &
    get();
  static void
    for_each_path(std::move_only_function<Ops(std::filesystem::path)> lambda);
};

}// namespace open_viii
#endif// VIIIARCHIVE_TESTPATHS_H
