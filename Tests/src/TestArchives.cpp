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
#include "TestArchives.hpp"
#include "OpenVIII/Archive/Archives.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{
  for (auto &path : open_viii::Paths::get()) {
    open_viii::Tools::replace_slashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = open_viii::archive::Archives<open_viii::LangT::en>(path);

    auto results = archives.search({});
    for (const auto &result : results) {
      std::cout << result.first << ":\n";
      [[maybe_unused]] const auto archive = archives.get(result.first);
      for (const auto &item : result.second) { std::cout << "  " << item.first << ", " << item.second << '\n'; }
    }
    continue;
    [[maybe_unused]] const auto &battle = archives.get<open_viii::archive::ArchiveTypeT::battle>();
    std::cout << battle << std::endl;
    [[maybe_unused]] const auto &field = archives.get<open_viii::archive::ArchiveTypeT::field>();
    std::cout << field << std::endl;
    [[maybe_unused]] const auto &nestedField = archives.get_nested({});
    for (const auto &nested : nestedField) { std::cout << nested << std::endl; }
    [[maybe_unused]] const auto &magic = archives.get<open_viii::archive::ArchiveTypeT::magic>();
    std::cout << magic << std::endl;
    [[maybe_unused]] const auto &main = archives.get<open_viii::archive::ArchiveTypeT::main>();
    std::cout << main << std::endl;
    [[maybe_unused]] const auto &menu = archives.get<open_viii::archive::ArchiveTypeT::menu>();
    std::cout << menu << std::endl;
    [[maybe_unused]] const auto &world = archives.get<open_viii::archive::ArchiveTypeT::world>();
    std::cout << world << std::endl;

    [[maybe_unused]] const auto &zzzMain = archives.get<open_viii::archive::ArchiveTypeT::zzz_main>();
    std::cout << zzzMain << std::endl;
    [[maybe_unused]] const auto &zzzOther = archives.get<open_viii::archive::ArchiveTypeT::zzz_other>();
    std::cout << zzzOther << std::endl;
  }
  return 0;
}