//
// Created by pcvii on 8/29/2020.
//
#include "OpenVIII/Paths/Paths.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Graphics/lzs.h"
#include "OpenVIII/Graphics/tim.h"
#include "OpenVIII/Graphics/tdw.h"
#include "OpenVIII/Graphics/tex.h"

int main()
{
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives<OpenVIII::LangT::EN>(path);

    archives.ExecuteOn({ ".tex" }, [](std::vector<char> buffer, [[maybe_unused]] std::string_view p) {
      if (OpenVIII::Tools::iEndsWith(p, ".lzs")) {
        auto t = OpenVIII::Graphics::lzs(buffer);
        std::cout << p << '\n' << t << '\n';
      } else if (OpenVIII::Tools::iEndsWith(p, ".tim")) {
        auto t = OpenVIII::Graphics::tim(buffer);
        std::cout << p << '\n' << t << '\n';
      } else if (OpenVIII::Tools::iEndsWith(p, ".tex")) {
        auto t = OpenVIII::Graphics::tex(buffer);
        std::cout << p << '\n' << t << '\n';
      }
    });
  }
}
