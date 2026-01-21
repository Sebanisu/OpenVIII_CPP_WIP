// minimal_diag.cpp
// Compile: g++ -std=c++20 minimal_diag.cpp -o ff8diag

#include "open_viii/paths/Paths.hpp"
#include <iostream>

int main()
{
    // FF8_PATH environment variable
    std::cout << "FF8_PATH = "
              << (std::getenv("FF8_PATH") ? std::getenv("FF8_PATH") : "(not set)")
              << "\n\n";

    // FF8 search paths
    const auto &paths = open_viii::Paths::get();

    if (paths.empty()) {
        std::cout << "No FF8 paths found.\n\n";
    } else {
        std::cout << "Found " << paths.size() << " FF8 path(s):\n";
        for (const auto &p : paths) {
            std::cout << "  " << p.string() << "\n";
        }
        std::cout << "\n";
    }

    // XDG / Windows directories
    std::cout << "Home:        " << open_viii::Paths::home().string() << "\n";
    std::cout << "Data Home:   " << open_viii::Paths::dataHome().string() << "\n";
    std::cout << "Config Home: " << open_viii::Paths::configHome().string() << "\n";
    std::cout << "State Home:  " << open_viii::Paths::stateHome().string() << "\n";
    std::cout << "Cache Home:  " << open_viii::Paths::cacheHome().string() << "\n";
    std::cout << "Runtime Dir: " << open_viii::Paths::runtimeDir().string() << "\n";
    std::cout << "Bin Home:    " << open_viii::Paths::binHome().string() << "\n";

    // Data / Config search directories
    const auto &dataDirs = open_viii::Paths::dataDirs();
    std::cout << "\nData Dirs:\n";
    for (const auto &d : dataDirs) {
        std::cout << "  " << d.string() << "\n";
    }

    const auto &configDirs = open_viii::Paths::configDirs();
    std::cout << "\nConfig Dirs:\n";
    for (const auto &d : configDirs) {
        std::cout << "  " << d.string() << "\n";
    }

    // Optional: Logs directory
    std::filesystem::path logsDir = open_viii::Paths::stateHome() / "MyApp" / "Logs";
    std::cout << "\nLogs Dir:   " << logsDir.string() << "\n";

    std::cout << "\n";
    return 0;
}
