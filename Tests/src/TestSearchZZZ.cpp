
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
#include "TestSearchZZZ.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{
  static constexpr std::array extensions = { open_viii::archive::FI::EXT,
                                             open_viii::archive::fl::EXT,
                                             open_viii::archive::FS::EXT };
  std::string                 needle{};
  std::cout << "Enter string to search for: ";
  std::getline(std::cin, needle);
  tl::string::replace_slashes(needle);
  if (needle.empty()) {
    return 1;
  }
  // test ZZZ extract with FIFLFS
  open_viii::Paths::for_each_path([&needle](const std::filesystem::path &path) {
    for (const auto &zzz_file_pair :
         open_viii::archive::ZZZ::get_files_from_path(path)) {
      std::cout << "Searching: " << path << "\n";
      for (const open_viii::archive::FileData &data_item :
           zzz_file_pair.second.data()) {
        {
          const auto path_string = data_item.get_path_string();
          if (open_viii::tools::i_ends_with_any(path_string, extensions)
              != 0U) {
            std::cout << "  nested: {" << path_string << "}\n";
          } else if (open_viii::tools::i_find(path_string, needle)) {
            std::cout << "  {" << path_string << "}\n";
          }
        }
      }
    }
  });
  return 0;
}