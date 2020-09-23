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
#include "TestReswizzleFields.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/graphics/Ppm.hpp"
#include "open_viii/graphics/background/Map.hpp"
#include "open_viii/graphics/background/Mim.hpp"
#include "open_viii/graphics/background/ReswizzleTree.hpp"
#include "open_viii/paths/Paths.hpp"

static void save_and_clear(std::vector<open_viii::graphics::Color24<0, 1, 2>> &out,
  const std::unsigned_integral auto &width,
  const std::unsigned_integral auto &height,
  const std::unsigned_integral auto &tex_id,
  const std::unsigned_integral auto &bpp,
  const std::string &output_prefix)
{
  std::string output_name = output_prefix + "_" + std::to_string(bpp) + "_" + std::to_string(tex_id);
  open_viii::graphics::Ppm::save(out, width, height, output_name);
  std::fill(std::ranges::begin(out), std::ranges::end(out), open_viii::graphics::Color24<0, 1, 2>{});
}
int main()
{
  const auto start = std::chrono::steady_clock::now();
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    const auto archives = open_viii::archive::Archives<open_viii::LangT::en>(path);
    [[maybe_unused]] const auto &field = archives.get<open_viii::archive::ArchiveTypeT::field>();
    {
      std::vector<std::jthread> threads{};
      open_viii::Tools::execute_on_directories(
        std::filesystem::current_path(), {}, [&field, &threads](const std::filesystem::path &directory_path) {
          threads.emplace_back([&field, directory_path]() {
            const auto reswizzle_tree = open_viii::graphics::background::ReswizzleTree{ field, directory_path };
            if (!static_cast<bool>(reswizzle_tree)) {
              return;
            }
            std::cout << directory_path << std::endl;
            reswizzle_tree.reswizzle();
          });

          if (threads.size() > 16) {
            threads.front().join();
            threads.erase(threads.begin());
          }
        });
    }
  });
  const auto end = std::chrono::steady_clock::now();
  const auto diff = end - start;
  std::cout << std::chrono::duration<double, std::milli>(diff).count() << " ms" << '\n';
}
