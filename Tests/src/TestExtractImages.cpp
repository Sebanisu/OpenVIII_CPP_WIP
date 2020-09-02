//
// Created by pcvii on 8/29/2020.
//
#include "OpenVIII/Paths/Paths.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Graphics/lzs.h"
#include "OpenVIII/Graphics/tim.h"
#include "OpenVIII/Graphics/tdw.h"
#include "OpenVIII/Graphics/tex.h"
#include "OpenVIII/Graphics/sp1.h"
#include "OpenVIII/Graphics/sp2.h"
#include "OpenVIII/MenuGroup/MenuGroupFile.h"

int main()
{
  for (auto &path : open_viii::Paths::get()) {
    open_viii::Tools::replace_slashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = open_viii::archive::Archives<open_viii::LangT::en>(path);
//    {
//      const auto &menu = archives.Get<open_viii::archive::ArchiveTypeT::Menu>();
//      const auto menuGroup = open_viii::MenuGroup::MenuGroupFile{ menu };
//      const auto fullpath = menu.GetFullPath(menuGroup.FILENAME);
//      menuGroup.TestTim(fullpath);
//    }
    archives.execute_on(
      { ".tex", ".lzs", ".tim", ".tdw", ".sp1", ".sp2" },
      [](std::vector<char> buffer, [[maybe_unused]] std::string_view p) {
        if (open_viii::Tools::i_ends_with(p, ".lzs")) {
          auto t = open_viii::graphics::Lzs(buffer);
          std::cout << p << '\n' << t << '\n';
          t.save(p);
        } else if (open_viii::Tools::i_ends_with(p, ".tim")) {
          auto t = open_viii::graphics::Tim(buffer);
          std::cout << p << '\n' << t << '\n';
          if(t.width()==0 || t.height() == 0)
          {
            open_viii::Tools::write_buffer(buffer,p);
          }
          else {
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
          // t.Save(p);
        } else if (open_viii::Tools::i_ends_with(p, ".sp2")) {
          auto t = open_viii::graphics::Sp2(buffer);
          std::cout << p << '\n' << t << '\n';
          // t.Save(p);
        }
      });
  }
}
