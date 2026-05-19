// minimal_diag.cpp
// Compile: g++ -std=c++20 minimal_diag.cpp -o ff8diag -lfmt -lspdlog

#include "open_viii/paths/Paths.hpp"

#include <fmt/core.h>
#include <spdlog/spdlog.h>

#include <cstdlib>
#include <filesystem>

int
  main()
{
  spdlog::set_level(spdlog::level::info);

  // FF8_PATH environment variable
  const char *ff8_path = std::getenv("FF8_PATH");
  if (!ff8_path) {
    spdlog::warn("FF8_PATH is not set");
  }
  else {
    fmt::print("FF8_PATH = {}\n\n", ff8_path);
  }

  // FF8 search paths
  const auto &paths = open_viii::Paths::get();

  if (paths.empty()) {
    spdlog::error("No FF8 paths found");
  }
  else {
    fmt::print("Found {} FF8 path(s):\n", paths.size());
    for (const auto &p : paths) {
      fmt::print("  {}\n", p.string());
    }
    fmt::print("\n");
  }

  // XDG / Windows directories
  fmt::print("Home:        {}\n", open_viii::Paths::home().string());
  fmt::print("Data Home:   {}\n", open_viii::Paths::dataHome().string());
  fmt::print("Config Home: {}\n", open_viii::Paths::configHome().string());
  fmt::print("State Home:  {}\n", open_viii::Paths::stateHome().string());
  fmt::print("Cache Home:  {}\n", open_viii::Paths::cacheHome().string());
  fmt::print("Runtime Dir: {}\n", open_viii::Paths::runtimeDir().string());
  fmt::print("Bin Home:    {}\n", open_viii::Paths::binHome().string());

  // Data / Config search directories
  const auto &dataDirs = open_viii::Paths::dataDirs();
  fmt::print("\nData Dirs:\n");
  for (const auto &d : dataDirs) {
    fmt::print("  {}\n", d.string());
  }

  const auto &configDirs = open_viii::Paths::configDirs();
  fmt::print("\nConfig Dirs:\n");
  for (const auto &d : configDirs) {
    fmt::print("  {}\n", d.string());
  }

  // Optional: Logs directory
  std::filesystem::path logsDir
    = open_viii::Paths::stateHome() / "MyApp" / "Logs";
  fmt::print("\nLogs Dir:   {}\n", logsDir.string());

  fmt::print("\n");
  return 0;
}