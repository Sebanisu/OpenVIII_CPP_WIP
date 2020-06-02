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
#include "../Archives.h"
#include "paths.h"
int main()
{
  for (auto path : Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives(path);
    [[maybe_unused]] const auto & battle = archives.Get<OpenVIII::Archive::ArchiveType::Battle>();
    std::cout << battle << std::endl;
    [[maybe_unused]] const auto & field = archives.Get<OpenVIII::Archive::ArchiveType::Field>();
    std::cout << field << std::endl;
    [[maybe_unused]] const auto & magic = archives.Get<OpenVIII::Archive::ArchiveType::Magic>();
    std::cout << magic << std::endl;
    [[maybe_unused]] const auto & main = archives.Get<OpenVIII::Archive::ArchiveType::Main>();
    std::cout << main << std::endl;
    [[maybe_unused]] const auto & menu = archives.Get<OpenVIII::Archive::ArchiveType::Menu>();
    std::cout << menu << std::endl;
    [[maybe_unused]] const auto & world = archives.Get<OpenVIII::Archive::ArchiveType::World>();
    std::cout << world << std::endl;

    [[maybe_unused]] const auto & zzzMain = archives.Get<OpenVIII::Archive::ArchiveType::ZZZMain>();
    std::cout << zzzMain << std::endl;
    [[maybe_unused]] const auto & zzzOther = archives.Get<OpenVIII::Archive::ArchiveType::ZZZOther>();
    std::cout << zzzOther << std::endl;
  }
  return 0;
}