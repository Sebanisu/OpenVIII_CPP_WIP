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
#include <mutex>
namespace open_viii {
struct Paths
{
public:
    enum struct Ops : std::uint8_t
    {
        Stop,
        Continue
    };

    // FF8 library lookup
    static std::vector<std::filesystem::path> ff8_paths_in_steam_library(const std::filesystem::path &steam_path);
    static std::vector<std::filesystem::path> get_windows_ff8_paths();
    static std::vector<std::filesystem::path> get_linux_ff8_paths();

    static std::vector<std::filesystem::path>& get();
    static void for_each_path(std::move_only_function<Ops(std::filesystem::path)> lambda);

    // Home & XDG/Windows equivalent directories
    static const std::filesystem::path& home();
    static const std::filesystem::path& dataHome();
    static const std::filesystem::path& configHome();
    static const std::filesystem::path& stateHome();
    static const std::filesystem::path& cacheHome();
    static const std::filesystem::path& runtimeDir();
    static const std::filesystem::path& binHome();

    static const std::vector<std::filesystem::path>& dataDirs();
    static const std::vector<std::filesystem::path>& configDirs();

private:
    // FF8 paths storage
    // static std::vector<std::filesystem::path> _paths;

    // Home & XDG/Windows paths
    static std::filesystem::path _home;
    static std::filesystem::path _dataHome;
    static std::filesystem::path _configHome;
    static std::filesystem::path _stateHome;
    static std::filesystem::path _cacheHome;
    static std::filesystem::path _runtimeDir;
    static std::filesystem::path _binHome;

    static std::vector<std::filesystem::path> _dataDirs;
    static std::vector<std::filesystem::path> _configDirs;

    static std::once_flag initFlag;
    static void initialize();
};

}// namespace open_viii
#endif// VIIIARCHIVE_TESTPATHS_H
