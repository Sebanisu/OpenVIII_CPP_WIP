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
        // t.Save(p);
      } else if (open_viii::Tools::i_ends_with(p, ".sp2")) {
        auto t = open_viii::graphics::Sp2(buffer);
        std::cout << p << '\n' << t << '\n';
        // t.Save(p);
      } else if (open_viii::Tools::i_ends_with(p, ".map")) {
        auto t = open_viii::graphics::background::Map<1>(buffer);
        std::cout << p << '\n' << t << '\n';
        // t.Save(p);
        // std::cout << sizeof(open_viii::graphics::background::PaletteID) << '\n';
      } else if (open_viii::Tools::i_ends_with(p, ".mim")) {
        auto t = open_viii::graphics::background::Mim(std::move(buffer), p);
        std::cout << p << '\n' << t << '\n';
        t.save(p);
      }
    };

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
    //    continue;
    // archives.execute_on({ /*".tex", ".lzs", ".tim", ".tdw", ".sp1", ".sp2", ".map", ".mim"*/ "ending" },dump_image);
  }
}
