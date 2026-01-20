// minimal_diag.cpp
// Compile:  g++ -std=c++20 minimal_diag.cpp -o ff8diag

#include "open_viii/paths/Paths.hpp"
#include <iostream>

int
  main()
{
  std::cout << "FF8_PATH = "
            << (std::getenv("FF8_PATH") ? std::getenv("FF8_PATH") : "(not set)")
            << "\n\n";

  const auto &paths = open_viii::Paths::get();

  if (paths.empty()) {
    std::cout << "No paths found.\n";
    return 0;
  }

  std::cout << "Found " << paths.size() << " path(s):\n\n";

  for (const auto &p : paths) {
    std::cout << p.string() << "\n";
  }

  std::cout << "\n";
  return 0;
}