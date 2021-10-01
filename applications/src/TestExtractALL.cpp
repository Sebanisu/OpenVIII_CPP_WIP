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
#include "TestExtractALL.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/paths/Paths.hpp"
#include <set>
int
  main()
{
  const auto start = std::chrono::steady_clock::now();
  auto InsensitiveCompare
    = [](const std::string &a, const std::string &b) -> bool {
    for (std::size_t i = 0; i != (std::min)(std::size(a), std::size(b));
         ++i) {
      // std::cout << ::toupper(a[i]) << '\t' << ::toupper(b[i]) << '\n';
      if (::toupper(a[i]) < ::toupper(b[i])) {
        return true;
      }
      else if (::toupper(a[i]) > ::toupper(b[i])) {
        return false;
      }
    }
    if (std::size(a) < std::size(b))
      return true;
    return false;
  };
  auto tracker = std::set<std::string, decltype(InsensitiveCompare)>{};
  const auto coos  = open_viii::LangCommon::to_string_array();
  for (const auto &coo : coos)
    open_viii::Paths::for_each_path([&coo, &tracker](const std::filesystem::path &path) {
      std::cout << path << std::endl;
      const auto archives = open_viii::archive::Archives(path, coo);
      if (!static_cast<bool>(archives)) {
        std::cerr << "Failed to load path: " << path.string() << '\n';
        return;
      }
      [[maybe_unused]] static constexpr auto dump
        = [](const std::vector<char> &in_buffer, const std::string &in_path) {
            open_viii::tools::write_buffer(in_buffer, in_path);
          };

      auto filter  = [&tracker](const std::string &in_path) {
        if (!tracker.contains(in_path)) {
          tracker.insert(in_path);
          std::cout << in_path << std::endl;
          return true;
        }
        return false;
      };
      archives.execute_on({}, dump, filter);
      // archives.get<open_viii::archive::ArchiveTypeT::field>().execute_with_nested({},dump,{});
    });
  const auto end  = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
}
