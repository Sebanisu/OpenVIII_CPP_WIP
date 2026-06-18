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
#include "TestDeswizzleFields.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/graphics/background/Deswizzle.hpp"
#include "open_viii/paths/Paths.hpp"
#include <chrono>
#include <set>
#include <string>
int
  main()
{
  const auto            start = std::chrono::steady_clock::now();

  std::set<std::string> extracted{};
  open_viii::Paths::for_each_path(
    [&](const std::filesystem::path &path) -> open_viii::Paths::Ops {
      std::cout << path << std::endl;
      static constexpr auto coo      = open_viii::LangT::en;
      const auto            archives = open_viii::archive::Archives(
        path,
        open_viii::LangCommon::to_string<coo>());
      if (!static_cast<bool>(archives)) {
        std::cerr << "Failed to load path: " << path.string() << '\n';
        return open_viii::Paths::Ops::Continue;
      }
      {
        const auto &field
          = archives.get<open_viii::archive::ArchiveTypeT::field>();
        field.execute_with_nested(
          {},
          [&](const open_viii::archive::FIFLFS<false> &e) {
            const auto basename = e.get_base_name();

            const auto process  = [&](const auto lang) {
              const auto lang_suffix = open_viii::LangCommon::to_string(lang);

              const auto localized_mim_name = fmt::format(
                "{}_{}{}",
                basename,
                lang_suffix,
                open_viii::graphics::background::Mim::EXT);

              const auto localized_map_name = fmt::format(
                "{}_{}{}",
                basename,
                lang_suffix,
                open_viii::graphics::background::Map::EXT);

              const auto generic_mim_name
                = basename + open_viii::graphics::background::Mim::EXT.data();

              const auto generic_map_name
                = basename + open_viii::graphics::background::Map::EXT.data();

              const auto mim_name = e.contains(localized_mim_name)
                                    ? localized_mim_name
                                    : generic_mim_name;

              if (!extracted.insert(e.get_full_path(mim_name)).second) {
                return;
              }

              const auto map_name = e.contains(localized_map_name)
                                    ? localized_map_name
                                    : generic_map_name;

              if (!extracted.insert(e.get_full_path(map_name)).second) {
                return;
              }

              const auto mim = open_viii::graphics::background::Mim{
                e.get_entry_data(mim_name),
                basename
              };

              const auto map = open_viii::graphics::background::Map{
                mim.mim_type(),
                e.get_entry_data(map_name)
              };

              const auto lang_str = open_viii::LangCommon::to_string(lang);

              if (lang_str.empty()) {
                fmt::print("  {}\n", basename);
              }
              else {
                fmt::print("  {} [{}]\n", basename, lang_str);
              }

              const auto deswizzle = open_viii::graphics::background::Deswizzle(
                mim,
                map,
                e.get_full_path(mim_name));

              deswizzle.save(lang);

              map.save_csv(e.get_full_path(map_name), deswizzle.pupus());
            };

            if (e.is_remaster_from_fl_paths()) {
              for (const auto lang : open_viii::LangCommon::to_array()) {
                process(lang);
              }
            }
            else {
              process(open_viii::LangT::generic);
            }
          });
      }
      return open_viii::Paths::Ops::Continue;
    });
  const auto end  = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
}
