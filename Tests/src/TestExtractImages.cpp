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
#include "TestExtractImages.h"
#include "OpenVIII/Paths/Paths.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Graphics/Lzs.hpp"
#include "OpenVIII/Graphics/Tim.hpp"
#include "OpenVIII/Graphics/Tdw.hpp"
#include "OpenVIII/Graphics/Tex.hpp"
#include "OpenVIII/Graphics/Sp1.hpp"
#include "OpenVIII/Graphics/Sp2.hpp"
#include "OpenVIII/MenuGroup/MenuGroupFile.h"
#include "OpenVIII/Graphics/background/Mim.hpp"
#include "OpenVIII/Graphics/background/Map.hpp"

int main()
{
  for (auto &path : open_viii::Paths::get()) {
    open_viii::Tools::replace_slashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = open_viii::archive::Archives<open_viii::LangT::en>(path);
    [[maybe_unused]] static constexpr auto dump_image = [](std::vector<char> &&in_buffer, std::string &&in_path) {
      std::vector<char> buffer{ std::move(in_buffer) };
      std::string p{ std::move(in_path) };
      if (open_viii::Tools::i_ends_with(p, ".lzs")) {
        auto t = open_viii::graphics::Lzs(buffer);
        std::cout << p << '\n' << t << '\n';
        t.save(p);
      } else if (open_viii::Tools::i_ends_with(p, ".tim")) {
        auto t = open_viii::graphics::Tim(buffer);
        std::cout << p << '\n' << t << '\n';
        if (t.width() == 0 || t.height() == 0) {
          open_viii::Tools::write_buffer(buffer, p);
        } else {
          t.save(p);
        }
      } else if (open_viii::Tools::i_ends_with(p, ".tex")) {
        auto t = open_viii::graphics::Tex(buffer);
        std::cout << p << '\n' << t << '\n';
        t.save(p);
      } else if (open_viii::Tools::i_ends_with(p, ".tdw")) {
        auto t = open_viii::graphics::Tdw(buffer);
        std::cout << p << '\n' << t << '\n';
        t.save(p);
      } else if (open_viii::Tools::i_ends_with(p, ".sp1")) {
        auto t = open_viii::graphics::Sp1(buffer);
        std::cout << p << '\n' << t << '\n';
      } else if (open_viii::Tools::i_ends_with(p, ".sp2")) {
        auto t = open_viii::graphics::Sp2(buffer);
        std::cout << p << '\n' << t << '\n';
      } else if (open_viii::Tools::i_ends_with(p, ".mim")) {
        auto t = open_viii::graphics::background::Mim(std::move(buffer), p);
        std::cout << p << '\n' << t << '\n';
        t.save(p);
      }
    };
    archives.execute_on({ ".tex", ".lzs", ".tim", ".tdw", ".sp1", ".sp2", ".mim" }, dump_image);
    // dump images from menu group.
    [[maybe_unused]] const auto &menu = archives.get<open_viii::archive::ArchiveTypeT::menu>();
    std::cout << menu << std::endl;
    auto mngrpfile = open_viii::menu_group::MenuGroupFile{ menu };
    mngrpfile.test_tim(menu.get_full_path(open_viii::menu_group::MenuGroupFile::FILENAME));
  }
}
