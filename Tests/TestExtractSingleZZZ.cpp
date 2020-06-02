
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
  const std::initializer_list<std::string_view> Extensions = { OpenVIII::Archive::FI::Ext, OpenVIII::Archive::FL::Ext, OpenVIII::Archive::FS::Ext };
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
        {
          const auto pathString = dataItem.GetPathString();
          if (OpenVIII::Tools::iEndsWithAny(pathString, Extensions) != 0U) {
            std::cout << "  nested: {" << pathString << "}\n";
          } else if (OpenVIII::Tools::iFind(pathString, needle)) {
            std::cout << "  {" << pathString << "}\n";
          }
        }
      }
    }
  }
  return 0;
}