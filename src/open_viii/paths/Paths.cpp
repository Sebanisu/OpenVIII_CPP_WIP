#include "Paths.hpp"
#include <array>
#include <cstdlib>// getenv
#include <ctre.hpp>
#include <string>
#ifdef _WIN32
#include <windows.h>
#endif

// std::vector<std::filesystem::path> Paths::_paths;

std::filesystem::path open_viii::Paths::_home;
std::filesystem::path open_viii::Paths::_dataHome;
std::filesystem::path open_viii::Paths::_configHome;
std::filesystem::path open_viii::Paths::_stateHome;
std::filesystem::path open_viii::Paths::_cacheHome;
std::filesystem::path open_viii::Paths::_runtimeDir;
std::filesystem::path open_viii::Paths::_binHome;

std::vector<std::filesystem::path> open_viii::Paths::_dataDirs;
std::vector<std::filesystem::path> open_viii::Paths::_configDirs;

std::once_flag open_viii::Paths::initFlag;

// Helper: verify path exists and is a directory
bool
  valid_dir(const std::filesystem::path &p)
{
  std::error_code ec;
  return std::filesystem::exists(p, ec) && !ec
      && std::filesystem::is_directory(p, ec) && !ec;
}


void open_viii::Paths::initialize()
{
#ifdef _WIN32
    char* homeEnv = std::getenv("USERPROFILE");
    _home = homeEnv ? homeEnv : "C:\\Users\\Default";

    _dataHome   = std::filesystem::path(std::getenv("XDG_DATA_HOME") ? std::getenv("XDG_DATA_HOME") : (_home / "AppData" / "Local"));
    _configHome = std::filesystem::path(std::getenv("XDG_CONFIG_HOME") ? std::getenv("XDG_CONFIG_HOME") : (_home / "AppData" / "Roaming"));
    _stateHome  = _configHome;
    _cacheHome  = std::filesystem::path(std::getenv("XDG_CACHE_HOME") ? std::getenv("XDG_CACHE_HOME") : (_dataHome / "Cache"));
    _runtimeDir = std::filesystem::path(std::getenv("XDG_RUNTIME_DIR") ? std::getenv("XDG_RUNTIME_DIR") : (_dataHome / "Runtime"));
    _binHome    = std::filesystem::path(std::getenv("XDG_BIN_HOME") ? std::getenv("XDG_BIN_HOME") : (_home / "AppData" / "Local" / "bin"));

    _dataDirs = { _dataHome, "C:\\ProgramData" };
    _configDirs = { _configHome, "C:\\ProgramData" };

#else
    char* homeEnv = std::getenv("HOME");
    _home = homeEnv ? homeEnv : "/home/user";

    _dataHome   = std::filesystem::path(std::getenv("XDG_DATA_HOME") ? std::getenv("XDG_DATA_HOME") : (_home / ".local" / "share"));
    _configHome = std::filesystem::path(std::getenv("XDG_CONFIG_HOME") ? std::getenv("XDG_CONFIG_HOME") : (_home / ".config"));
    _stateHome  = std::filesystem::path(std::getenv("XDG_STATE_HOME") ? std::getenv("XDG_STATE_HOME") : (_home / ".local" / "state"));
    _cacheHome  = std::filesystem::path(std::getenv("XDG_CACHE_HOME") ? std::getenv("XDG_CACHE_HOME") : (_home / ".cache"));
    _runtimeDir = std::filesystem::path(std::getenv("XDG_RUNTIME_DIR") ? std::getenv("XDG_RUNTIME_DIR") : (_home / ".run"));
    _binHome    = std::filesystem::path(std::getenv("XDG_BIN_HOME") ? std::getenv("XDG_BIN_HOME") : (_home / ".local" / "bin"));

    const char* dataDirsEnv = std::getenv("XDG_DATA_DIRS");
    if (dataDirsEnv)
    {
        std::string dirs = dataDirsEnv;
        size_t pos = 0;
        while ((pos = dirs.find(':')) != std::string::npos)
        {
            _dataDirs.push_back(dirs.substr(0, pos));
            dirs.erase(0, pos + 1);
        }
        if (!dirs.empty()) _dataDirs.push_back(dirs);
    }
    else
    {
        _dataDirs = { "/usr/local/share", "/usr/share" };
    }

    const char* configDirsEnv = std::getenv("XDG_CONFIG_DIRS");
    if (configDirsEnv)
    {
        std::string dirs = configDirsEnv;
        size_t pos = 0;
        while ((pos = dirs.find(':')) != std::string::npos)
        {
            _configDirs.push_back(dirs.substr(0, pos));
            dirs.erase(0, pos + 1);
        }
        if (!dirs.empty()) _configDirs.push_back(dirs);
    }
    else
    {
        _configDirs = { "/etc/xdg" };
    }
#endif
}

// Accessors with call_once
#define INIT() std::call_once(initFlag, initialize)

const std::filesystem::path& open_viii::Paths::home()      { INIT(); return _home; }
const std::filesystem::path& open_viii::Paths::dataHome()  { INIT(); return _dataHome; }
const std::filesystem::path& open_viii::Paths::configHome(){ INIT(); return _configHome; }
const std::filesystem::path& open_viii::Paths::stateHome() { INIT(); return _stateHome; }
const std::filesystem::path& open_viii::Paths::cacheHome() { INIT(); return _cacheHome; }
const std::filesystem::path& open_viii::Paths::runtimeDir(){ INIT(); return _runtimeDir; }
const std::filesystem::path& open_viii::Paths::binHome()   { INIT(); return _binHome; }

const std::vector<std::filesystem::path>& open_viii::Paths::dataDirs()   { INIT(); return _dataDirs; }
const std::vector<std::filesystem::path>& open_viii::Paths::configDirs() { INIT(); return _configDirs; }

#undef INIT

// Returns FF8 paths under a given Steam library root
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
  if (auto path = queryRegPath(
        HKEY_LOCAL_MACHINE,
        "SOFTWARE\\Square Soft, Inc\\FINAL FANTASY VIII\\1.00",
        "AppPath");
      !path.empty()) {
    paths.emplace_back(std::move(path));
  }

  // WOW6432Node path
  if (auto path = queryRegPath(
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
        // std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
        //           << error_code.value() << ": " << error_code.message()
        //           << error_code.value() << " - path: " << path << std::endl;
        error_code.clear();
      }
      const bool is_dir = std::filesystem::is_directory(path, error_code);
      if (error_code) {
        // std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
        //           << error_code.value() << ": " << error_code.message()
        //           << " - path: " << path << std::endl;
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
    if (static const auto path_file
        = std::filesystem::current_path(error_code) / "paths.conf";
        error_code) {
      // std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
      //           << error_code.value() << ": " << error_code.message()
      //           << error_code.value() << " - path: " << path_file <<
      //           std::endl;
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
          // std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
          //           << error_code.value() << ": " << error_code.message()
          //           << error_code.value() << " - path: " << path <<
          //           std::endl;
          error_code.clear();
        }
        const bool is_dir = std::filesystem::is_directory(path, error_code);
        if (error_code) {
          // std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
          //           << error_code.value() << ": " << error_code.message()
          //           << " - path: " << path << std::endl;
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

void
  open_viii::Paths::for_each_path(
    std::move_only_function<open_viii::Paths::Ops(std::filesystem::path)>
      lambda)
{
  for (const std::filesystem::path &fs_path : get()) {
    std::error_code error_code{};
    const bool      found = std::filesystem::exists(fs_path, error_code);
    if (error_code) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << error_code.value() << ": " << error_code.message()
                << std::endl;
      error_code.clear();
    }

    const bool is_dir = std::filesystem::is_directory(fs_path, error_code);
    if (error_code) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << error_code.value() << ": " << error_code.message()
                << std::endl;
      error_code.clear();
    }

    if (found && is_dir) {
      if (lambda(fs_path) == open_viii::Paths::Ops::Stop) {
        break;
      }
    }
  }
}
