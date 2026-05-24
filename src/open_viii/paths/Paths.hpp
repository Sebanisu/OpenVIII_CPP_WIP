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
#include <mutex>
#include <spdlog/spdlog.h>
#include <system_error>
#include <utility>
namespace open_viii {
struct Paths
{
public:
  /**
   * @brief Control operations returned by path iteration callbacks.
   *
   * Used by for_each_path() to determine whether iteration should
   * continue or terminate early.
   */
  enum struct Ops : std::uint8_t
  {
    /**
     * @brief Stop iterating over remaining paths.
     */
    Stop,

    /**
     * @brief Continue iterating over remaining paths.
     */
    Continue
  };

  /**
   * @brief Searches a Steam library for installed Final Fantasy VIII
   * directories.
   *
   * Scans the specified Steam library root for FF8 Classic and FF8 Remastered
   * application manifests. Additional Steam library folders are discovered
   * through parsing libraryfolders.vdf.
   *
   * Valid installation directories are collected, deduplicated, and sorted.
   *
   * @param steam_path Root Steam installation path.
   * @return Vector of detected FF8 installation paths.
   */
  static std::vector<std::filesystem::path>
    ff8_paths_in_steam_library(const std::filesystem::path &steam_path);

  /**
   * @brief Searches common Linux Steam installation locations for FF8 installs.
   *
   * Iterates through a predefined set of Steam installation paths relative
   * to the user's HOME directory and aggregates all detected FF8 installations.
   *
   * @return Vector of detected FF8 installation paths.
   */
  static std::vector<std::filesystem::path>
    get_windows_ff8_paths();

  /**
   * @brief Searches common Linux Steam installation locations for FF8 installs.
   *
   * Iterates through a predefined set of Steam installation paths relative
   * to the user's HOME directory and aggregates all detected FF8 installations.
   *
   * @return Vector of detected FF8 installation paths.
   */
  static std::vector<std::filesystem::path>
    get_linux_ff8_paths();

  /**
   * @brief Retrieves all known and discovered Final Fantasy VIII paths.
   *
   * Builds and caches a persistent list of FF8 installation paths from:
   * - Hardcoded default locations
   * - Optional paths.conf entries
   * - FF8_PATH environment variable
   * - Platform-specific Steam and registry discovery
   *
   * Paths are normalized, canonicalized, deduplicated, and validated before
   * being returned.
   *
   * The returned vector is initialized once on first access and reused for
   * the lifetime of the application.
   *
   * @return Reference to the cached vector of valid FF8 installation paths.
   */
  static std::vector<std::filesystem::path> &
    get();

  /**
   * @brief Iterates over all valid FF8 installation paths.
   *
   * Invokes the provided callback for each valid directory returned by get().
   * Iteration stops early when the callback returns Ops::Stop.
   *
   * Filesystem errors are reported to stderr and ignored.
   *
   * @param lambda Callback invoked for each valid filesystem path.
   */
  static void
    for_each_path(std::move_only_function<Ops(std::filesystem::path)> lambda);

  /**
   * @brief Returns the user's home directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the resolved home directory path.
   */
  static const std::filesystem::path &
    home();

  /**
   * @brief Returns the user-specific data directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the resolved data directory path.
   */
  static const std::filesystem::path &
    dataHome();

  /**
   * @brief Returns the user-specific configuration directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the resolved configuration directory path.
   */
  static const std::filesystem::path &
    configHome();

  /**
   * @brief Returns the user-specific state directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the resolved state directory path.
   */
  static const std::filesystem::path &
    stateHome();

  /**
   * @brief Returns the user-specific cache directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the resolved cache directory path.
   */
  static const std::filesystem::path &
    cacheHome();

  /**
   * @brief Returns the runtime directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * On Unix-like systems, this resolves XDG_RUNTIME_DIR when available.
   * If unset, a fallback runtime path under the system temporary directory
   * is used.
   *
   * @return Reference to the resolved runtime directory path.
   */
  static const std::filesystem::path &
    runtimeDir();

  /**
   * @brief Returns the user-specific binary directory path.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the resolved binary directory path.
   */
  static const std::filesystem::path &
    binHome();

  /**
   * @brief Returns the list of system-wide data directories.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the vector of resolved data directories.
   */
  static const std::vector<std::filesystem::path> &
    dataDirs();

  /**
   * @brief Returns the list of system-wide configuration directories.
   *
   * Lazily initializes the internal path cache on first access.
   *
   * @return Reference to the vector of resolved configuration directories.
   */
  static const std::vector<std::filesystem::path> &
    configDirs();

private:
  // FF8 paths storage
  // static std::vector<std::filesystem::path> _paths;

  // Home & XDG/Windows paths
  static std::filesystem::path              _home;
  static std::filesystem::path              _dataHome;
  static std::filesystem::path              _configHome;
  static std::filesystem::path              _stateHome;
  static std::filesystem::path              _cacheHome;
  static std::filesystem::path              _runtimeDir;
  static std::filesystem::path              _binHome;

  static std::vector<std::filesystem::path> _dataDirs;
  static std::vector<std::filesystem::path> _configDirs;

  static std::once_flag                     initFlag;

  /**
   * @brief Initializes platform-specific XDG and application directory paths.
   *
   * Detects and stores standard filesystem locations for user data,
   * configuration, cache, runtime, and binary directories. Paths are resolved
   * from environment variables when available, otherwise platform-specific
   * defaults are used.
   *
   * On Windows, locations are derived from USERPROFILE and AppData folders.
   * On Unix-like systems, XDG Base Directory specifications are followed.
   */
  static void
    initialize();
};

}// namespace open_viii
#endif// VIIIARCHIVE_TESTPATHS_H
