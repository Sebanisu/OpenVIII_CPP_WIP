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
#include "open_viii/paths/Paths.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/graphics/background/Mim.hpp"
#include "open_viii/graphics/background/Map.hpp"
#include "open_viii/graphics/background/Deswizzle.hpp"
#include <chrono>

int main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path){
    std::cout << path << std::endl;
    const auto archives = open_viii::archive::Archives<open_viii::LangT::en>(path);
    {
      const auto &field = archives.get<open_viii::archive::ArchiveTypeT::field>();
      field.execute_with_nested({}, [](const open_viii::archive::FIFLFS<false> &e) {
        const std::string &basename = e.get_base_name();
        const std::string mim_name = basename + ".mim";
        const std::string map_name = basename + ".map";
        auto mim = open_viii::graphics::background::Mim{ e.get_entry_data(mim_name), basename };
        // mim.save(e.get_full_path(".mim"));
        const auto process = [&mim, &e, &mim_name, &map_name, &basename](const auto &map) {
          //((open_viii::graphics::background::Map<1>)map).max_x()
          std::cout << "  " << basename << '\n';
          //          const auto &[min_x, max_x] = map.minmax_x();
          //          const auto &[min_y, max_y] = map.minmax_y();
          //          std::cout << "  Type: " << static_cast<uint16_t>(mim.mim_type().type()) << '\n';
          //          std::cout << "min x: " << min_x->x() << '\n';
          //          std::cout << "min y: " << min_y->y() << '\n';
          //          std::cout << "max x: " << max_x->x() << '\n';
          //          std::cout << "max y: " << max_y->y() << '\n';
          //          std::cout << "canvas: " << map.canvas() << '\n';
          //
          //          std::cout << "min x: " << min_x->x() << '\n';
          //          std::cout << "min y: " << min_y->y() << '\n';
          //          std::cout << "max x: " << max_x->x() << '\n';
          //          std::cout << "max y: " << max_y->y() << '\n';
          //          std::cout << "canvas: " << map.canvas() << '\n';

          map.save_csv(e.get_full_path(map_name));
          open_viii::graphics::background::Deswizzle(mim, map, e.get_full_path(mim_name));
          // map.save_v1(mim, e.get_full_path(".mim"));
        };
        if (mim.mim_type().type() == 1) {
          process(
            open_viii::graphics::background::Map<open_viii::graphics::background::Tile1>{ e.get_entry_data(map_name) });
        } else if (mim.mim_type().type() == 2) {
          process(
            open_viii::graphics::background::Map<open_viii::graphics::background::Tile2>{ e.get_entry_data(map_name) });
        } else if (mim.mim_type().type() == 3) {
          process(
            open_viii::graphics::background::Map<open_viii::graphics::background::Tile3>{ e.get_entry_data(map_name) });
        }
      });
    }
  });

  const auto end = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << '\n';
}
