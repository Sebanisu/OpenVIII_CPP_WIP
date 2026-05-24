#include "Paths.hpp"
#include <array>
#include <cstdlib>// getenv
#include <ctre.hpp>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

// std::vector<std::filesystem::path> Paths::_paths;

std::filesystem::path              open_viii::Paths::_home;
std::filesystem::path              open_viii::Paths::_dataHome;
std::filesystem::path              open_viii::Paths::_configHome;
std::filesystem::path              open_viii::Paths::_stateHome;
std::filesystem::path              open_viii::Paths::_cacheHome;
std::filesystem::path              open_viii::Paths::_runtimeDir;
std::filesystem::path              open_viii::Paths::_binHome;

std::vector<std::filesystem::path> open_viii::Paths::_dataDirs;
std::vector<std::filesystem::path> open_viii::Paths::_configDirs;

std::once_flag                     open_viii::Paths::initFlag;

/**
 * @brief Checks whether a filesystem path exists and is a directory.
 *
 * Uses the non-throwing std::filesystem overloads with std::error_code
 * to safely validate a path without generating exceptions.
 *
 * @param p Filesystem path to validate.
 * @return true if the path exists and is a directory.
 */
bool
  valid_dir(const std::filesystem::path &p)
{
  std::error_code ec;
  return std::filesystem::exists(p, ec) && !ec
      && std::filesystem::is_directory(p, ec) && !ec;
}

/**
 * @brief Resolves an XDG-style filesystem path from an environment variable.
 *
 * Returns the value of the specified environment variable when it exists
 * and is non-empty. Otherwise, returns the provided fallback path.
 *
 * @param envName Name of the environment variable to query.
 * @param fallback Fallback path used when the environment variable is unset.
 * @return Resolved filesystem path.
 */
static std::filesystem::path
  xdgPath(const char *envName, const std::filesystem::path &fallback)
{
  if (const char *value = std::getenv(envName); value && *value) {
    return std::filesystem::path{ value };
  }

  return fallback;
}

#ifndef _WIN32
/**
 * @brief Resolves a colon-separated list of filesystem paths from an
 * environment variable.
 *
 * Parses the specified environment variable using ':' as the delimiter and
 * converts each entry into a std::filesystem::path. If the variable is unset
 * or empty, the provided fallback list is returned.
 *
 * @param envName Name of the environment variable to query.
 * @param fallback Default list of paths used when the variable is unset.
 * @return Vector of resolved filesystem paths.
 */
static std::vector<std::filesystem::path>
  xdgPathList(
    const char                                  *envName,
    std::initializer_list<std::filesystem::path> fallback)
{
  if (const char *value = std::getenv(envName); value && *value) {
    std::vector<std::filesystem::path> paths;

    std::string_view                   dirs{ value };
    size_t                             start = 0;

    while (start < dirs.size()) {
      const size_t end = dirs.find(':', start);

      paths.emplace_back(dirs.substr(start, end - start));

      if (end == std::string_view::npos)
        break;

      start = end + 1;
    }

    return paths;
  }

  return { fallback };
}
#endif

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
void
  open_viii::Paths::initialize()
{
#ifdef _WIN32
  _home       = xdgPath("USERPROFILE", "C:\\Users\\Default");
  _dataHome   = xdgPath("XDG_DATA_HOME", _home / "AppData" / "Local");
  _configHome = xdgPath("XDG_CONFIG_HOME", _home / "AppData" / "Roaming");
  _stateHome  = _configHome;
  _cacheHome  = xdgPath("XDG_CACHE_HOME", _dataHome / "Cache");
  _runtimeDir = xdgPath("XDG_RUNTIME_DIR", _dataHome / "Runtime");
  _binHome    = xdgPath("XDG_BIN_HOME", _home / "AppData" / "Local" / "bin");

  _dataDirs   = { _dataHome, "C:\\ProgramData" };
  _configDirs = { _configHome, "C:\\ProgramData" };

#else
  _home       = xdgPath("HOME", "/home/user");
  _dataHome   = xdgPath("XDG_DATA_HOME", _home / ".local" / "share");
  _configHome = xdgPath("XDG_CONFIG_HOME", _home / ".config");
  _stateHome  = xdgPath("XDG_STATE_HOME", _home / ".local" / "state");
  _cacheHome  = xdgPath("XDG_CACHE_HOME", _home / ".cache");
  _runtimeDir = xdgPath(
    "XDG_RUNTIME_DIR",
    std::filesystem::temp_directory_path() / "run");
  _binHome = xdgPath("XDG_BIN_HOME", _home / ".local" / "bin");

  _dataDirs
    = xdgPathList("XDG_DATA_DIRS", { "/usr/local/share", "/usr/share" });

  _configDirs = xdgPathList("XDG_CONFIG_DIRS", { "/etc/xdg" });
#endif
}

/**
 * @brief Ensures one-time initialization of static path state.
 *
 * Wraps std::call_once to guarantee that Paths::initialize()
 * executes exactly once in a thread-safe manner before accessing
 * cached path data.
 *
 * This macro is intended for internal accessor usage only.
 */
#define INIT() std::call_once(initFlag, initialize)

/**
 * @brief Returns the user's home directory path.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the resolved home directory path.
 */
const std::filesystem::path &
  open_viii::Paths::home()
{
  INIT();
  return _home;
}

/**
 * @brief Returns the user-specific data directory path.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the resolved data directory path.
 */
const std::filesystem::path &
  open_viii::Paths::dataHome()
{
  INIT();
  return _dataHome;
}

/**
 * @brief Returns the user-specific configuration directory path.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the resolved configuration directory path.
 */
const std::filesystem::path &
  open_viii::Paths::configHome()
{
  INIT();
  return _configHome;
}

/**
 * @brief Returns the user-specific state directory path.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the resolved state directory path.
 */
const std::filesystem::path &
  open_viii::Paths::stateHome()
{
  INIT();
  return _stateHome;
}

/**
 * @brief Returns the user-specific cache directory path.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the resolved cache directory path.
 */
const std::filesystem::path &
  open_viii::Paths::cacheHome()
{
  INIT();
  return _cacheHome;
}

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
const std::filesystem::path &
  open_viii::Paths::runtimeDir()
{
  INIT();
  return _runtimeDir;
}

/**
 * @brief Returns the user-specific binary directory path.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the resolved binary directory path.
 */
const std::filesystem::path &
  open_viii::Paths::binHome()
{
  INIT();
  return _binHome;
}

/**
 * @brief Returns the list of system-wide data directories.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the vector of resolved data directories.
 */
const std::vector<std::filesystem::path> &
  open_viii::Paths::dataDirs()
{
  INIT();
  return _dataDirs;
}

/**
 * @brief Returns the list of system-wide configuration directories.
 *
 * Lazily initializes the internal path cache on first access.
 *
 * @return Reference to the vector of resolved configuration directories.
 */
const std::vector<std::filesystem::path> &
  open_viii::Paths::configDirs()
{
  INIT();
  return _configDirs;
}

/**
 * @brief Searches a Steam library for installed Final Fantasy VIII directories.
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
std::vector<std::filesystem::path>
  open_viii::Paths::ff8_paths_in_steam_library(
    const std::filesystem::path &steam_path)
{
  const std::array<std::string_view, 2> app_ids = { "39150", "1026680" };
  std::vector<std::filesystem::path>    paths;

  std::filesystem::path                 steamapps = steam_path / "steamapps";
  if (!valid_dir(steamapps))
    return paths;

  // Parse libraryfolders.vdf for additional libraries
  std::vector<std::filesystem::path> libraries;
  libraries.push_back(steamapps);

  std::filesystem::path vdf_path = steamapps / "libraryfolders.vdf";
  if (std::filesystem::exists(vdf_path)) {
    std::ifstream vdf_file(vdf_path);
    std::string   line;
    while (std::getline(vdf_file, line)) {
      if (auto match = ctre::search<R"ccc("path"\s+"([^"]+)")ccc">(line)) {
        std::string tmp = match.get<1>().to_string();

        size_t      pos = 0;
        while ((pos = tmp.find("\\\\", pos)) != std::string::npos) {
          tmp.replace(pos, 2, "\\");
          ++pos;
        }

        std::filesystem::path lib_path
          = std::filesystem::path{ tmp } / "steamapps";
        if (valid_dir(lib_path))
          libraries.push_back(lib_path);
      }
    }
  }

  // Scan each library for FF8 manifests
  for (const auto &app_id : app_ids) {
    for (const auto &lib : libraries) {
      std::filesystem::path manifest_path
        = lib / (std::string("appmanifest_") + app_id.data() + ".acf");
      if (!std::filesystem::exists(manifest_path))
        continue;

      std::ifstream manifest_file(manifest_path);
      std::string   line;
      while (std::getline(manifest_file, line)) {
        if (
          auto match
          = ctre::search<R"ccc("installdir"\s+"([^"]+)")ccc">(line)) {
          std::filesystem::path ff8_path
            = lib / "common" / match.get<1>().to_string();
          if (valid_dir(ff8_path))
            paths.push_back(std::move(ff8_path));
        }
      }
    }
  }

  // Deduplicate & sort
  std::ranges::sort(paths);
  auto [first, last] = std::ranges::unique(paths);
  paths.erase(first, last);

  return paths;
}

/**
 * @brief Searches common Linux Steam installation locations for FF8 installs.
 *
 * Iterates through a predefined set of Steam installation paths relative
 * to the user's HOME directory and aggregates all detected FF8 installations.
 *
 * @return Vector of detected FF8 installation paths.
 */
std::vector<std::filesystem::path>
  open_viii::Paths::get_linux_ff8_paths()
{
  std::vector<std::filesystem::path> paths{};
  // 1. Check FF8_PATH env var
  // 2. Check Steam

  static const std::filesystem::path home = []() -> std::filesystem::path {
    if (const char *env_home_path = std::getenv("HOME")) {
      return env_home_path;
    }
    return {};
  }();
  if (home.empty()) {
    return paths;
  }
  static const auto SteamRelPaths = std::to_array<std::filesystem::path>(
    { ".local/share/Steam",
      ".steam/steam",
      ".steam/root",
      ".var/app/com.valvesoftware.Steam/.local/share/Steam",
      ".var/app/com.valvesoftware.Steam/.steam",
      "snap/steam/common/.local/share/Steam",
      "snap/steam/common/.steam/steam",
      ".steam" });

  for (const auto &rel : SteamRelPaths) {
    const auto full = home / rel;
    std::ranges::move(
      ff8_paths_in_steam_library(full),
      std::back_inserter(paths));
  }
  return paths;
}

/**
 * @brief Searches Windows registry and Steam libraries for FF8 installs.
 *
 * Detects Final Fantasy VIII installations from:
 * - Original FF8 registry keys
 * - WOW6432Node registry keys
 * - Steam library manifests for FF8 Classic and Remastered
 *
 * Invalid or non-directory entries are removed before results are sorted
 * and deduplicated.
 *
 * @return Vector of detected FF8 installation paths.
 */
std::vector<std::filesystem::path>
  open_viii::Paths::get_windows_ff8_paths()
{
  std::vector<std::filesystem::path> paths;

#ifndef _WIN32
  return paths;// Empty vector on non-Windows
#else

  // 2. Check registry for FF8 2000
  auto queryRegPath = [&](
                        const HKEY  key,
                        const char *subKey,
                        const char *valueName) -> std::filesystem::path {
    HKEY hKey;
    if (RegOpenKeyExA(key, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
      std::array<char, 512U> buffer     = {};
      DWORD                  bufferSize = static_cast<DWORD>(buffer.size());
      if (
        RegQueryValueExA(
          hKey,
          valueName,
          nullptr,
          nullptr,
          reinterpret_cast<LPBYTE>(buffer.data()),
          &bufferSize)
        == ERROR_SUCCESS) {
        RegCloseKey(hKey);
        return std::string(buffer.data(), bufferSize - 1);
      }
      RegCloseKey(hKey);
    }
    return {};
  };

  // Original path
  if (
    auto path = queryRegPath(
      HKEY_LOCAL_MACHINE,
      "SOFTWARE\\Square Soft, Inc\\FINAL FANTASY VIII\\1.00",
      "AppPath");
    !path.empty()) {
    paths.emplace_back(std::move(path));
  }

  // WOW6432Node path
  if (
    auto path = queryRegPath(
      HKEY_LOCAL_MACHINE,
      "SOFTWARE\\WOW6432Node\\Square Soft, Inc\\FINAL FANTASY VIII\\1.00",
      "AppPath");
    !path.empty()) {
    paths.emplace_back(std::move(path));
  }

  // 3. Check Steam libraries for FF8 classic (39150) and remaster (1026680)
  std::filesystem::path steam_path = queryRegPath(
    HKEY_LOCAL_MACHINE,
    "SOFTWARE\\WOW6432Node\\Valve\\Steam",
    "InstallPath");

  if (!steam_path.empty()) {
    std::vector<std::filesystem::path> libraries;
    std::filesystem::path              main_steam_path
      = std::filesystem::path(steam_path) / "steamapps";
    libraries.push_back(main_steam_path);

    // Parse libraryfolders.vdf for additional libraries
    std::filesystem::path vdf_path = main_steam_path / "libraryfolders.vdf";
    if (std::filesystem::exists(vdf_path)) {
      std::ifstream vdf_file(vdf_path);
      std::string   line;
      while (std::getline(vdf_file, line)) {
        if (auto match = ctre::search<R"ccc("path"\s+"([^"]+)")ccc">(line)) {
          std::string tmp = match.get<1>().to_string();
          size_t      pos = 0;
          while ((pos = tmp.find("\\\\", pos)) != std::string::npos) {
            tmp.replace(pos, 2, "\\");
            ++pos;
          }
          std::filesystem::path lib_path
            = std::filesystem::path(tmp) / "steamapps";
          libraries.push_back(lib_path);
        }
      }
    }

    // Check for FF8 in each library
    std::vector<std::string> app_ids = { "39150", "1026680" };
    for (const auto &app_id : app_ids) {
      for (const auto &lib : libraries) {
        std::filesystem::path manifest_path
          = lib / ("appmanifest_" + app_id + ".acf");
        if (std::filesystem::exists(manifest_path)) {
          std::ifstream manifest_file(manifest_path);
          std::string   line;
          while (std::getline(manifest_file, line)) {
            if (
              auto match
              = ctre::search<R"ccc("installdir"\s+"([^"]+)")ccc">(line)) {
              std::filesystem::path path
                = lib / "common" / match.get<1>().to_string();
              paths.push_back(path);
            }
          }
        }
      }
    }
  }
  std::error_code error_code{};
  const auto [first, last] = std::ranges::remove_if(
    paths,
    [&error_code](const std::filesystem::path &path) {
      const bool found = std::filesystem::exists(path, error_code);

      if (error_code) {
        spdlog::trace(
          "std::filesystem::exists failed for '{}': {} ({})",
          path.string(),
          error_code.message(),
          error_code.value());
        error_code.clear();
      }

      const bool is_dir = std::filesystem::is_directory(path, error_code);

      if (error_code) {
        spdlog::trace(
          "std::filesystem::is_directory failed for '{}': {} ({})",
          path.string(),
          error_code.message(),
          error_code.value());
        error_code.clear();
      }
      return !found || !is_dir;
    });
  paths.erase(first, last);
  std::ranges::sort(paths);
  auto &&[rem_begin, rem_end] = std::ranges::unique(paths);
  paths.erase(rem_begin, rem_end);
  return paths;
#endif
}

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
std::vector<std::filesystem::path> &
  open_viii::Paths::get()
{
  using namespace std::literals::string_literals;
  namespace fs                                  = std::filesystem;
  static std::vector<std::filesystem::path> ret = []() {
    std::vector<fs::path> paths{
      fs::path(
        R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)"),
      fs::path(
        R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII Remastered)"),

      fs::path(
        R"(D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII Remastered)"),
      fs::path(
        R"(/mnt/d/SteamLibrary/steamapps/common/FINAL FANTASY VIII Remastered)"),

      fs::path(
        R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII)"),
      fs::path(
        R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII)"),

      fs::path(R"(D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII)"),
      fs::path(R"(/mnt/d/SteamLibrary/steamapps/common/FINAL FANTASY VIII)"),

      fs::path(R"(/mnt/k/ff82000)"),
      fs::path(R"(K:\ff82000)"),

      fs::path(R"(/mnt/d/games/ff82000)"),
      fs::path(R"(D:\games\ff82000)"),

      // CD
      fs::path(R"(/mnt/e/)"),
      fs::path(R"(E:\)"),

      // folders with tim files in it
      fs::path(R"(/mnt/d/tim)"),
      fs::path(R"(D:\tim)")
    };

    // normalize slashes to native
    for (auto &p : paths) {
      p.make_preferred();// converts slashes to native format
    }

    std::error_code error_code{};
    if (
      static const auto path_file
      = std::filesystem::current_path(error_code) / "paths.conf";
      error_code) {
      spdlog::trace(
        "std::filesystem::current_path failed: {} ({})",
        error_code.message(),
        error_code.value());
      error_code.clear();
    }
    else {
      static bool read_file = true;
      if (read_file) {
        auto file_stream
          = std::ifstream(path_file, std::ios::in | std::ios::binary);
        if (file_stream.is_open()) {
          read_file = false;
          std::string line{};
          while (std::getline(file_stream, line)) {
            paths.emplace_back(tl::string::replace_slashes(std::move(line)));
          }
        }
      }
    }
    const auto [first, last] = std::ranges::remove_if(
      paths,
      [&error_code](const std::filesystem::path &path) {
        const bool found = std::filesystem::exists(path, error_code);

        if (error_code) {
          spdlog::trace(
            "std::filesystem::exists failed for '{}': {} ({})",
            path.string(),
            error_code.message(),
            error_code.value());
          error_code.clear();
        }
        const bool is_dir = std::filesystem::is_directory(path, error_code);

        if (error_code) {
          spdlog::trace(
            "std::filesystem::is_directory failed for '{}': {} ({})",
            path.string(),
            error_code.message(),
            error_code.value());
          error_code.clear();
        }
        return !found || !is_dir;
      });
    paths.erase(first, last);

    if (const char *env_path = std::getenv("FF8_PATH")) {
      paths.push_back(env_path);
    }
#ifdef _WIN32
    auto tmp_paths = get_windows_ff8_paths();
#else
    auto tmp_paths = get_linux_ff8_paths();
#endif
    paths.insert(
      paths.end(),
      std::make_move_iterator(tmp_paths.begin()),
      std::make_move_iterator(tmp_paths.end()));
    // normalize paths to native separators
    std::ranges::transform(paths, paths.begin(), [](std::filesystem::path p) {
      p = std::filesystem::canonical(p);
      p.make_preferred();
      return p;
    });
    std::ranges::sort(paths);
    auto &&[rem_begin, rem_end] = std::ranges::unique(paths);
    paths.erase(rem_begin, rem_end);
    return paths;
  }();

  return ret;
}

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
void
  open_viii::Paths::for_each_path(
    std::move_only_function<open_viii::Paths::Ops(std::filesystem::path)>
      lambda)
{
  for (const std::filesystem::path &fs_path : get()) {
    std::error_code error_code{};
    const bool      found = std::filesystem::exists(fs_path, error_code);

    if (error_code) {
      spdlog::trace(
        "std::filesystem::exists failed for '{}': {} ({})",
        fs_path.string(),
        error_code.message(),
        error_code.value());
      error_code.clear();
    }

    const bool is_dir = std::filesystem::is_directory(fs_path, error_code);

    if (error_code) {
      spdlog::trace(
        "std::filesystem::is_directory failed for '{}': {} ({})",
        fs_path.string(),
        error_code.message(),
        error_code.value());
      error_code.clear();
    }

    if (found && is_dir) {
      if (lambda(fs_path) == open_viii::Paths::Ops::Stop) {
        break;
      }
    }
  }
}