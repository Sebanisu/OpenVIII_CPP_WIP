//
// Created by pcvii on 8/29/2020.
//
#include "TestDeswizzleFields.h"
#include "OpenVIII/Paths/Paths.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Graphics/lzs.h"
#include "OpenVIII/Graphics/tim.h"
#include "OpenVIII/Graphics/tdw.h"
#include "OpenVIII/Graphics/tex.h"
#include "OpenVIII/Graphics/sp1.h"
#include "OpenVIII/Graphics/sp2.h"
#include "OpenVIII/MenuGroup/MenuGroupFile.h"
#include "OpenVIII/Graphics/background/Mim.h"
#include "OpenVIII/Graphics/background/Map.h"

int main()
{
  for (auto &path : open_viii::Paths::get()) {
    open_viii::Tools::replace_slashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = open_viii::archive::Archives<open_viii::LangT::en>(path);
    {
      const auto &field = archives.get<open_viii::archive::ArchiveTypeT::field>();
      field.execute_with_nested({}, [](const open_viii::archive::FIFLFS<false> &e) {
        auto mim = open_viii::graphics::background::Mim{ e.get_entry_data(".mim"), e.get_base_name() };
        // mim.save(e.get_full_path(".mim"));
        const auto process = [&mim, &e](const auto &map) {
          //((open_viii::graphics::background::Map<1>)map).max_x()
          std::cout << "  " << e.get_base_name() << '\n';
          const auto &[min_x, max_x] = map.minmax_x();
          const auto &[min_y, max_y] = map.minmax_y();
          std::cout << "  Type: " << static_cast<uint16_t>(mim.mim_type().type()) << '\n';
          std::cout << "min x: " << min_x->x() << '\n';
          std::cout << "min y: " << min_y->y() << '\n';
          std::cout << "max x: " << max_x->x() << '\n';
          std::cout << "max y: " << max_y->y() << '\n';
          std::cout << "canvas: " << map.canvas() << '\n';
          map.shift_to_origin();

          std::cout << "min x: " << min_x->x() << '\n';
          std::cout << "min y: " << min_y->y() << '\n';
          std::cout << "max x: " << max_x->x() << '\n';
          std::cout << "max y: " << max_y->y() << '\n';
          std::cout << "canvas: " << map.canvas() << '\n';
          map.save_csv(mim, e.get_full_path(".map"));
          map.save(mim, e.get_full_path(".mim"));

          // map.save_v1(mim, e.get_full_path(".mim"));
        };
        if (mim.mim_type().type() == 1) {
          process(open_viii::graphics::background::Map<1>{ e.get_entry_data(".map") });
        } else if (mim.mim_type().type() == 2) {
          process(open_viii::graphics::background::Map<2>{ e.get_entry_data(".map") });
        } else if (mim.mim_type().type() == 3) {
          process(open_viii::graphics::background::Map<3>{ e.get_entry_data(".map") });
        }
      });
    }
  }
}
