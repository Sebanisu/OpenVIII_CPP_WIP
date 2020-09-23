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
#include "TestExtractSingleFIFLFS.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{
  // test FIFLFS
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  open_viii::Tools::replace_slashes(needle);
  if (needle.empty()) {
    return 1;
  }
  open_viii::Paths::for_each_path([&needle](const std::filesystem::path &path) {
    const auto files = open_viii::archive::FIFLFS<true>::get_files_from_path(path);
    for (const auto &entry : files) {

      const auto &fiflfs = entry.second;
      const auto &fl = fiflfs.fl();
      std::cout << "Searching " << fiflfs << '\n';
      {
        const auto &[id, innerPath] = open_viii::archive::FL::get_entry(fl.path(), { needle }, fl.offset());
        if (!innerPath.empty()) {
          std::cout << "single match: {" << id << ", " << innerPath << "}\n";
        }
      }
      {
        const auto searchEntries = open_viii::archive::FL::get_all_entries(
          fl.path(), fl.offset(), 0U, 0U, { std::string_view(needle), open_viii::archive::FL::EXT });
        if (!searchEntries.empty()) {
          std::cout << "filtered sorted searchEntries: \n";
          for (const auto &searchEntry : searchEntries) {
            const auto &[id, innerPath] = searchEntry;
            if (!innerPath.empty()) {
              auto p = std::filesystem::path(innerPath);
              {
                auto extNum = open_viii::archive::FIFLFS<true>::check_extension(p);
                if (extNum == 1) {
                  const auto &parent_fi = fiflfs.fi();
                  const auto &parent_fs = fiflfs.fs();
                  const open_viii::FI_Like auto fi = [&parent_fi, &id]() {
                    if (std::ranges::empty(parent_fi.data())) {
                      return open_viii::archive::FI(parent_fi.path(), id, parent_fi.offset());
                    }
                    return open_viii::archive::FI(parent_fi.data(), id, parent_fi.offset());
                  }();
                  auto buffer = [&parent_fs, &fi]() {
                    if (std::ranges::empty(parent_fs.data())) {
                      return open_viii::archive::FS::get_entry<std::string>(parent_fs.path(), fi, parent_fs.offset());
                    }
                    return open_viii::archive::FS::get_entry<std::string>(parent_fs.data(), fi, parent_fs.offset());
                  }();
                  open_viii::archive::FL::clean_buffer(buffer);

                  std::cout << "  Searching " << innerPath << '\n';
                  for (const auto &nestedSearchEntry :
                    open_viii::archive::FL::get_all_entries_data(innerPath, buffer, 0U, 0U, 0U, { needle })) {
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
  });
  return 0;
}
