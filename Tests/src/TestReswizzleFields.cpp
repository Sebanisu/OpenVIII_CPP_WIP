//
// Created by pcvii on 8/29/2020.
//
#include "TestReswizzleFields.h"
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
    [[maybe_unused]] const auto &field = archives.get<open_viii::archive::ArchiveTypeT::field>();
    //[[maybe_unused]]const auto ppms =
    // open_viii::Tools::i_path_extension_ends_with_any(std::filesystem::current_path(),{"test"},{".ppm"});
    open_viii::Tools::execute_on_i_path_extension_ends_with_any(
      std::filesystem::current_path(), { "test" }, { ".ppm" }, [](const std::filesystem::path &p) {
        // if(!p.has_stem()) {return;}
        auto basename = p.stem().string();
        static constexpr auto minsize{ 24 };
        if (std::ranges::size(basename) < minsize) {
          return;
        }
        auto suffix = std::string_view(basename).substr(std::ranges::size(basename) - 7);
        if (suffix != "_mimmap") {
          return;
        }
        auto hex = std::string_view(basename).substr(std::ranges::size(basename) - 23, 16);
        auto prefix = std::string_view(basename).substr(0, std::ranges::size(basename) - 24);

        auto pupu_id = std::strtoull(std::ranges::data(hex), 0, 16);
        std::cout << basename << '\t' << suffix << '\t' << hex << '\t' << pupu_id << '\t' << prefix;
        std::cout << '\n';
        // test7_413FC003FF800000_mimmap.ppm

        const auto [p_depth, p_layer_id, p_blend_mode, p_animation_id, p_animation_state, p_z] =
          open_viii::graphics::background::Map<>::extract_pupu(pupu_id);
        std::cout << '\t' << p_depth << '\t' << p_layer_id << '\t' << p_blend_mode << '\t' << p_animation_id << '\t'
                  << p_animation_state << '\t' << p_z << '\n';
      });
  }
}
