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
#include "TestKernel.h"
#include "../Archives.h"
#include "paths.h"
#include "../Kernel.h"
int main()
{
  for (auto path : Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives(path);
    [[maybe_unused]] const auto &main = archives.Get<OpenVIII::Archive::ArchiveType::Main>();
    std::cout << main << std::endl;
    auto kernel = Kernel{ main };
    [[maybe_unused]] const auto &buffer = kernel.Buffer();
    std::cout << "kernel.bin " << buffer.size() << " bytes; " << kernel.SectionCount() << " section count\n";
    const auto &sectionOffsets = kernel.SectionOffsets();
    std::for_each(
      sectionOffsets.begin(), sectionOffsets.end(),[](const auto &value) { std::cout << value << std::endl; });
  }
  return 0;
}