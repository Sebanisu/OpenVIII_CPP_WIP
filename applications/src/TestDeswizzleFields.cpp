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
int
  main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    static constexpr auto coo      = open_viii::LangT::en;
    const auto            archives = open_viii::archive::Archives(
      path, open_viii::LangCommon::to_string<coo>());
    if (!static_cast<bool>(archives)) {
      std::cerr << "Failed to load path: " << path.string() << '\n';
      return;
    }
    {
      const auto &field =
        archives.get<open_viii::archive::ArchiveTypeT::field>();
      field.execute_with_nested(
        {}, [](const open_viii::archive::FIFLFS<false> &e) {
          const std::string &basename = e.get_base_name();
          const std::string  mim_name =
            basename + open_viii::graphics::background::Mim::EXT.data();
          const std::string map_name =
            basename + open_viii::graphics::background::Map<>::EXT.data();
          auto mim =
            open_viii::graphics::background::Mim{ e.get_entry_data(mim_name),
                                                  basename };
          const auto process =
            [&mim, &e, &mim_name, &map_name, &basename](auto map) {
              std::cout << "  " << basename << '\n';
              map.save_csv(e.get_full_path(map_name));
              open_viii::graphics::background::Deswizzle(
                mim, map, e.get_full_path(mim_name))
                .save();
            };
          if (mim.mim_type().type() == 1) {
            process(open_viii::graphics::background::Map<
                    open_viii::graphics::background::Tile1>{
              e.get_entry_data(map_name) });
          } else if (mim.mim_type().type() == 2) {
            process(open_viii::graphics::background::Map<
                    open_viii::graphics::background::Tile2>{
              e.get_entry_data(map_name) });
          } else if (mim.mim_type().type() == 3) {
            process(open_viii::graphics::background::Map<
                    open_viii::graphics::background::Tile3>{
              e.get_entry_data(map_name) });
          }
        });
    }
  });
  const auto end  = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms"
            << '\n';
}
