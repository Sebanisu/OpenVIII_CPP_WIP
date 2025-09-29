#include "Paths.hpp"
#include <array>
#include <string>
#ifdef _WIN32
#include <ctre.hpp>
#include <windows.h>
#endif

std::vector<std::filesystem::path>
  open_viii::Paths::get_windows_ff8_paths()
{
  std::vector<std::filesystem::path> paths;
#ifndef _WIN32
  return paths;// Empty vector on non-Windows
#else

  // 1. Check FF8_PATH env var
  if (const char *env_path = std::getenv("FF8_PATH")) {
    paths.push_back(env_path);
  }

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
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << error_code.value() << ": " << error_code.message()
                  << error_code.value() << " - path: " << path << std::endl;
        error_code.clear();
      }
      const bool is_dir = std::filesystem::is_directory(path, error_code);
      if (error_code) {
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << error_code.value() << ": " << error_code.message()
                  << " - path: " << path << std::endl;
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
  static std::vector<std::filesystem::path> ret = []() {
    std::vector<std::filesystem::path> paths{
      tl::string::replace_slashes(
        R"(C:\Program Files (x86)\Steam\steamapps\common\FINAL FANTASY VIII Remastered)"s),
      tl::string::replace_slashes(
        R"(/mnt/c/Program Files (x86)/Steam/steamapps/common/FINAL FANTASY VIII Remastered)"s),

      tl::string::replace_slashes(
        R"(D:\SteamLibrary\steamapps\common\FINAL FANTASY VIII Remastered)"s),
      tl::string::replace_slashes(
        R"(/mnt/d/SteamLibrary/steamapps/common/FINAL FANTASY VIII Remastered)"s),

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
    };

    std::error_code   error_code{};
    static const auto path_file
      = std::filesystem::current_path(error_code) / "paths.conf";
    if (error_code) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << error_code.value() << ": " << error_code.message()
                << error_code.value() << " - path: " << path_file << std::endl;
      error_code.clear();
    }
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
    const auto [first, last] = std::ranges::remove_if(
      paths,
      [&error_code](const std::filesystem::path &path) {
        const bool found = std::filesystem::exists(path, error_code);
        if (error_code) {
          std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                    << error_code.value() << ": " << error_code.message()
                    << error_code.value() << " - path: " << path << std::endl;
          error_code.clear();
        }
        const bool is_dir = std::filesystem::is_directory(path, error_code);
        if (error_code) {
          std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                    << error_code.value() << ": " << error_code.message()
                    << " - path: " << path << std::endl;
          error_code.clear();
        }
        return !found || !is_dir;
      });
    paths.erase(first, last);
    paths.insert_range(paths.end(), get_windows_ff8_paths());
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
