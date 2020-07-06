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
#include "OpenVIII/Paths/Paths.h"
int main()
{
  // test FIFLFS
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  OpenVIII::Tools::replaceSlashes(needle);
  if (needle.empty()) {
    return 1;
  }
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    const auto files = OpenVIII::Archive::FIFLFS<true>::GetFilesFromPath(path);
    for (const auto &entry : files) {

      const auto &fiflfs = entry.second;
      const auto &fl = fiflfs.FL();
      std::cout << "Searching " << fiflfs << '\n';
      {
        const auto &[id, innerPath] = OpenVIII::Archive::FL::GetEntry(fl.path, { needle }, fl.offset);
        if (!innerPath.empty()) {
          std::cout << "single match: {" << id << ", " << innerPath << "}\n";
        }
      }
      {
        const auto searchEntries = OpenVIII::Archive::FL::GetAllEntries(
          fl.path, fl.offset, 0U, 0U, { std::string_view(needle), OpenVIII::Archive::FL::Ext });
        if (!searchEntries.empty()) {
          std::cout << "filtered sorted searchEntries: \n";
          for (const auto &searchEntry : searchEntries) {
            const auto &[id, innerPath] = searchEntry;
            if (!innerPath.empty()) {
              auto p = std::filesystem::path(innerPath);
              {
                auto extNum = OpenVIII::Archive::FIFLFS<true>::CheckExtension(p);
                if (extNum == 1) {
                  const auto &parent_fi = fiflfs.FI();
                  const auto &parent_fs = fiflfs.FS();
                  const auto fi = [&parent_fi, &id]() {
                    if (std::empty(parent_fi.data)) {
                      return OpenVIII::Archive::FI(parent_fi.path, id, parent_fi.offset);
                    }
                    return OpenVIII::Archive::FI(parent_fi.data, id, parent_fi.offset);
                  }();
                  auto buffer = [&parent_fs, &fi]() {
                    if (std::empty(parent_fs.data)) {
                      return OpenVIII::Archive::FS::GetEntry<std::string>(parent_fs.path, fi, parent_fs.offset);
                    }
                    return OpenVIII::Archive::FS::GetEntry<std::string>(parent_fs.data, fi, parent_fs.offset);
                  }();
                  OpenVIII::Archive::FL::CleanBuffer(buffer);

                  std::cout << "  Searching " << innerPath << '\n';
                  for (const auto &nestedSearchEntry :
                    OpenVIII::Archive::FL::GetAllEntriesData(innerPath, buffer, 0U, 0U, 0U, { needle })) {
                    const auto &[nestedId, nestedInnerPath] = nestedSearchEntry;
                    std::cout << "    {" << nestedId << ", " << nestedInnerPath << "}\n";
                  }
                }
                if (extNum != 0) {
                  continue;
                }
              }
              std::cout << "  {" << id << ", " << innerPath << "}\n";
            }
          }
        }
      }
    }
  }
  // std::for_each(std::begin(files), std::end(files), &OpenVIII::Archive::FIFLFS::testPair);
  return 0;
}
