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
#include "TestArchives.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Paths/Paths.h"
int main()
{
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives<OpenVIII::LangT::EN>(path);
    auto results = archives.Search({ ".tdw", ".tim", ".png" });
    for (const auto &result : results) {
      std::cout << result.first << ":\n";
      [[maybe_unused]] const auto archive = archives.Get(result.first);
      for (const auto &item : result.second) { std::cout << "  " << item.first << ", " << item.second << '\n'; }
    }
    continue;
    [[maybe_unused]] const auto &battle = archives.Get<OpenVIII::Archive::ArchiveTypeT::Battle>();
    std::cout << battle << std::endl;
    [[maybe_unused]] const auto &field = archives.Get<OpenVIII::Archive::ArchiveTypeT::Field>();
    std::cout << field << std::endl;
    [[maybe_unused]] const auto &nestedField = archives.Get<OpenVIII::Archive::ArchiveTypeT::Field>({});
    for (const auto &nested : nestedField) { std::cout << nested << std::endl; }
    [[maybe_unused]] const auto &magic = archives.Get<OpenVIII::Archive::ArchiveTypeT::Magic>();
    std::cout << magic << std::endl;
    [[maybe_unused]] const auto &main = archives.Get<OpenVIII::Archive::ArchiveTypeT::Main>();
    std::cout << main << std::endl;
    [[maybe_unused]] const auto &menu = archives.Get<OpenVIII::Archive::ArchiveTypeT::Menu>();
    std::cout << menu << std::endl;
    [[maybe_unused]] const auto &world = archives.Get<OpenVIII::Archive::ArchiveTypeT::World>();
    std::cout << world << std::endl;

    [[maybe_unused]] const auto &zzzMain = archives.Get<OpenVIII::Archive::ArchiveTypeT::ZZZMain>();
    std::cout << zzzMain << std::endl;
    [[maybe_unused]] const auto &zzzOther = archives.Get<OpenVIII::Archive::ArchiveTypeT::ZZZOther>();
    std::cout << zzzOther << std::endl;
  }
  return 0;
}