
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
#include "TestExtractSingleZZZ.h"
#include "paths.h"
int main()
{
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  OpenVIII::Tools::replaceSlashes(needle);
  if (needle.empty()) {
    return 1;
  }
  // test ZZZ extract with FIFLFS
  for (auto path : Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);

    if (!std::filesystem::exists(path)) {
      continue;
    }
    for (const auto &zzzFilePair : OpenVIII::Archive::ZZZ::GetFilesFromPath(path)) {
      std::cout << "Searching: " << path << "\n";
      for (const auto &dataItem : zzzFilePair.second.Data()) {
        if(OpenVIII::Tools::iFind(dataItem.GetPathString(),needle))
        {
          std::cout<< "  {" << dataItem.GetPathString() << "}\n";
        }

      }
    }
  }
  return 0;
}