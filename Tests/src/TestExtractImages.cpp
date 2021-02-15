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
#include "TestExtractImages.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/graphics/Lzs.hpp"
#include "open_viii/graphics/Sp1.hpp"
#include "open_viii/graphics/Sp2.hpp"
#include "open_viii/graphics/Tdw.hpp"
#include "open_viii/graphics/Tex.hpp"
#include "open_viii/graphics/background/Map.hpp"
#include "open_viii/menu_group/MenuGroupFile.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    const auto archives =
      open_viii::archive::Archives<open_viii::LangT::en>(path);
    [[maybe_unused]] static constexpr auto dump_image =
      [](std::vector<char> &&buffer, const std::string &p) {
        if (open_viii::tools::i_ends_with(p, ".lzs")) {
          auto t = open_viii::graphics::Lzs(buffer);
          std::cout << p << '\n' << t << '\n';
          t.save(p);
        } else if (open_viii::tools::i_ends_with(p, ".tim")) {
          auto t = open_viii::graphics::Tim(buffer);
          std::cout << p << '\n' << t << '\n';
          if (t.width() == 0 || t.height() == 0) {
            open_viii::tools::write_buffer(buffer, p);
          } else {
            t.save(p);
          }
        } else if (open_viii::tools::i_ends_with(p, ".tex")) {
          auto t = open_viii::graphics::Tex(buffer);
          std::cout << p << '\n' << t << '\n';
          t.save(p);
        } else if (open_viii::tools::i_ends_with(p, ".tdw")) {
          auto t = open_viii::graphics::Tdw(buffer);
          std::cout << p << '\n' << t << '\n';
          t.save(p);
        } else if (open_viii::tools::i_ends_with(p, ".sp1")) {
          auto t = open_viii::graphics::Sp1(buffer);
          std::cout << p << '\n' << t << '\n';
        } else if (open_viii::tools::i_ends_with(p, ".sp2")) {
          auto t = open_viii::graphics::Sp2(buffer);
          std::cout << p << '\n' << t << '\n';
        } else if (open_viii::tools::i_ends_with(
                     p, open_viii::graphics::background::Mim::EXT)) {
          auto t = open_viii::graphics::background::Mim(std::move(buffer), p);
          std::cout << p << '\n' << t << '\n';
          t.save(p);
        }
      };
    archives.execute_on({ ".tex",
                          ".lzs",
                          ".tim",
                          ".tdw",
                          ".sp1",
                          ".sp2",
                          open_viii::graphics::background::Mim::EXT },
                        dump_image);
    // dump images from menu group.
    [[maybe_unused]] const auto &menu =
      archives.get<open_viii::archive::ArchiveTypeT::menu>();
    std::cout << menu << std::endl;
    auto mngrpfile = open_viii::menu_group::MenuGroupFile{ menu };
    mngrpfile.test_tim(
      menu.get_full_path(open_viii::menu_group::MenuGroupFile::FILENAME));
  });
}
