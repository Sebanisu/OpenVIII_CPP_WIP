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

#include <string>
#include "TestExtractSingleFIFLFS.h"
#include "paths.h"
int main()
{
  // test FIFLFS
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  OpenVIII::Tools::replaceSlashes(needle);
  if (needle.empty()) { return 1; }
  for(auto path : Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) { continue; }
    const auto files = OpenVIII::Archive::FIFLFS::GetFilesFromPath(path);
    for (const auto &entry : files) {
      const auto &fiflfs = entry.second;
      std::cout << "Searching " << fiflfs << '\n';
      const auto &fl = fiflfs.FL();
      const auto &[id, innerPath] = OpenVIII::Archive::FL::GetEntry(fl.path, needle, fl.offset);
      if (!innerPath.empty()) { std::cout << "single match: {" << id << ", " << innerPath << "}\n"; }
      const auto vector = OpenVIII::Archive::FL::GetAllEntries(fl.path, fl.offset, 0U,0U, needle);
      if (!vector.empty()) {
        std::cout << "filtered sorted vector: \n";
        for (const auto &ventry : vector) {
          const auto &[vid, vinnerPath] = ventry;
          if (!vinnerPath.empty()) { std::cout << '{' << vid << ", " << vinnerPath << "}\n"; }
        }
      }
    }
  }
  // std::for_each(std::begin(files), std::end(files), &OpenVIII::Archive::FIFLFS::testPair);
  return 0;
}
