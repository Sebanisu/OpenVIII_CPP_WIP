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
#include "TestExtractALLSearch.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/paths/Paths.hpp"
#include <string>
int
  main()
{
  // test FIFLFS
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  tl::string::replace_slashes(needle);
  if (needle.empty()) {
    return 1;
  }
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path(
    [&needle](const std::filesystem::path &path) -> open_viii::Paths::Ops {
      std::cout << path << std::endl;
      static constexpr auto coo      = open_viii::LangT::en;
      const auto            archives = open_viii::archive::Archives(
        path,
        open_viii::LangCommon::to_string<coo>());
      if (!static_cast<bool>(archives)) {
        std::cerr << "Failed to load path: " << path.string() << '\n';
        return open_viii::Paths::Ops::Continue;
      }
      [[maybe_unused]] static constexpr auto dump
        = [](const std::vector<char> &in_buffer, const std::string &in_path) {
            open_viii::tools::write_buffer(in_buffer, in_path);
          };
      archives.execute_on({ needle }, dump);
      return open_viii::Paths::Ops::Continue;
    });
  const auto end  = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
  return 0;
}
