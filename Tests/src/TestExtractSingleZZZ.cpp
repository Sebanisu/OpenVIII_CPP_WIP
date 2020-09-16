
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
#include "TestExtractSingleZZZ.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{
  const std::initializer_list<std::string_view> extensions = {
    open_viii::archive::FI::EXT, open_viii::archive::FL::EXT, open_viii::archive::FS::EXT
  };
  std::string needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  open_viii::Tools::replace_slashes(needle);
  if (needle.empty()) {
    return 1;
  }
  // test ZZZ extract with FIFLFS
  for (auto &path : open_viii::Paths::get()) {
    open_viii::Tools::replace_slashes(path);

    if (!std::filesystem::exists(path)) {
      continue;
    }
    for (const auto &zzzFilePair : open_viii::archive::ZZZ::get_files_from_path(path)) {
      std::cout << "Searching: " << path << "\n";
      for (const open_viii::archive::FileData &dataItem : zzzFilePair.second.data()) {
        {
          const auto pathString = dataItem.get_path_string();
          if (open_viii::Tools::i_ends_with_any(pathString, extensions) != 0U) {
            std::cout << "  nested: {" << pathString << "}\n";
          } else if (open_viii::Tools::i_find(pathString, needle)) {
            std::cout << "  {" << pathString << "}\n";
          }
        }
      }
    }
  }
  return 0;
}