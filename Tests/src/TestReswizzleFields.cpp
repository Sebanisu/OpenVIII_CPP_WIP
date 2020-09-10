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
    //    [[maybe_unused]]const auto ppms =
    //     open_viii::Tools::i_path_extension_ends_with_any(std::filesystem::current_path(),{"test"},{".ppm"});

    open_viii::Tools::execute_on_directories(
      std::filesystem::current_path(), { "test" }, [&field](const std::filesystem::path &directory_path) {
        const std::string fi_filename = directory_path.filename().string() + std::string(open_viii::archive::FI::EXT);
        const std::string fl_filename = directory_path.filename().string() + std::string(open_viii::archive::FL::EXT);
        const std::string fs_filename = directory_path.filename().string() + std::string(open_viii::archive::FS::EXT);
        auto archive = field.get_fiflfs({ fi_filename, fl_filename, fs_filename });

        if (!static_cast<bool>(archive)) {
          return;// no archive for this directory.
        }
        const std::string map_filename = directory_path.filename().string() + std::string(".map");
        const std::string mim_filename = directory_path.filename().string() + std::string(".mim");
        std::cout << map_filename << '\n';
        std::cout << archive << '\n';
        auto mims = archive.get_vector_of_indexs_and_files({ mim_filename });
        if (std::ranges::empty(mims)) {
          return;// no mim file.
        }
        open_viii::FI_Like auto mim_fi = archive.get_entry_by_index(mims[0].first);
        open_viii::graphics::background::MimType mim_type = open_viii::graphics::background::Mim::get_texture_type(
          mim_fi.uncompressed_size(), directory_path.filename().string());
        const auto reswizzle = [&directory_path](auto map) {
          open_viii::Tools::execute_on_directory(
            directory_path, {}, { ".ppm" }, [&map](const std::filesystem::path &file_path) {
              // if(!file_path.has_stem()) {return;}
              auto basename = file_path.stem().string();
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

              auto pupu_id = open_viii::graphics::background::Pupu(hex);
              std::cout << basename << '\t' << suffix << '\t' << hex << '\t' << pupu_id << '\t' << prefix;
              std::cout << '\n';
            });
        };
        const auto map_buffer = archive.get_entry_data(map_filename);
        if (mim_type.type() == 1) {
          reswizzle(open_viii::graphics::background::Map<1>(map_buffer));
        } else if (mim_type.type() == 2) {
          reswizzle(open_viii::graphics::background::Map<2>(map_buffer));
        } else if (mim_type.type() == 3) {
          reswizzle(open_viii::graphics::background::Map<3>(map_buffer));
        }
      });
  }
}
