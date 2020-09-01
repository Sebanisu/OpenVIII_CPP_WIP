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
#include "TestExtractZZZ.h"
#include "OpenVIII/Paths/Paths.h"
int main()
{
  // test ZZZ extract with FIFLFS
  for (auto &path : open_viii::Paths::get()) {
    open_viii::Tools::replace_slashes(path);

    if (!std::filesystem::exists(path)) {
      continue;
    }
    const auto files = open_viii::archive::ZZZ::get_files_from_path(path);
    std::for_each(files.begin(), files.end(), &open_viii::archive::ZZZ::test_pair);
  }
  return 0;
}