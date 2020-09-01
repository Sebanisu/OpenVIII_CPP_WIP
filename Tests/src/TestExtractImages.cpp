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
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives<OpenVIII::LangT::EN>(path);
    {
      const auto &menu = archives.Get<OpenVIII::Archive::ArchiveTypeT::Menu>();
      const auto menuGroup = OpenVIII::MenuGroup::MenuGroupFile{ menu };
      const auto fullpath = menu.GetFullPath(menuGroup.FILENAME);
      menuGroup.TestTim(fullpath);
    }
    archives.ExecuteOn({ /*".tex", ".lzs", ".tim", ".tdw", ".sp1", ".sp2"*/ ".lzs" },
      [](std::vector<char> buffer, [[maybe_unused]] std::string_view p) {
        if (OpenVIII::Tools::iEndsWith(p, ".lzs")) {
          auto t = OpenVIII::Graphics::lzs(buffer);
          std::cout << p << '\n' << t << '\n';
          t.Save(p);
        } else if (OpenVIII::Tools::iEndsWith(p, ".tim")) {
          auto t = OpenVIII::Graphics::tim(buffer);
          std::cout << p << '\n' << t << '\n';
          t.Save(p);
        } else if (OpenVIII::Tools::iEndsWith(p, ".tex")) {
          auto t = OpenVIII::Graphics::tex(buffer);
          std::cout << p << '\n' << t << '\n';
          t.Save(p);
        } else if (OpenVIII::Tools::iEndsWith(p, ".tdw")) {
          auto t = OpenVIII::Graphics::tdw(buffer);
          std::cout << p << '\n' << t << '\n';
          t.Save(p);
        } else if (OpenVIII::Tools::iEndsWith(p, ".sp1")) {
          auto t = OpenVIII::Graphics::sp1(buffer);
          std::cout << p << '\n' << t << '\n';
          // t.Save(p);
        } else if (OpenVIII::Tools::iEndsWith(p, ".sp2")) {
          auto t = OpenVIII::Graphics::sp2(buffer);
          std::cout << p << '\n' << t << '\n';
          // t.Save(p);
        }
      });
  }
}
