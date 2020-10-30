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
#include "TestKernel.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/kernel/Header.hpp"
#include "open_viii/paths/Paths.hpp"
int main()
{

  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    const auto coo = open_viii::LangT::en;
    const auto archives = open_viii::archive::Archives<coo>(path);
    [[maybe_unused]] const auto &main = archives.get<open_viii::archive::ArchiveTypeT::main>();
    std::cout << main << std::endl;
    auto kernel = open_viii::kernel::Header<coo>{ main };
    [[maybe_unused]] const auto &buffer = kernel.buffer();
    std::cout << "kernel.bin " << buffer.size() << " bytes; " << kernel.section_count() << " section count\n";
    std::cout << static_cast<int>(open_viii::kernel::SectionTypesT::count) << std::endl;
    // const auto &sectionOffsets = kernel.SectionOffsets();
    //    std::for_each(
    //      sectionOffsets.begin(), sectionOffsets.end(),[](const auto &value) { std::cout << value << std::endl; });
    //    kernel.static_for<static_cast<int>(open_viii::kernel::SectionTypesT::first),
    //      static_cast<int>(open_viii::kernel::SectionTypesT::count)>([](const std::string_view &string,const
    //      std::span<const char> & span, const auto & data) { std::cout << "  " << string << " - " << std::ranges::size(span)
    //      << " bytes\n";
    //
    //      return data;
    //    });
    kernel.static_for([]<typename kernel_section_t>(const std::string_view &string,
      const std::span<const char> &span,
      const kernel_section_t &data) requires(!std::is_null_pointer_v<kernel_section_t>) {
      std::cout << string << " ( " << std::ranges::size(span) << "bytes) has " << std::ranges::size(data)
                << " entries\n";
      for (size_t i = 0; i < data.size(); i++) {
        auto entry = data.at(i);
        std::cout << i << ": ";
        entry.out(std::cout, data.text_span());
        std::cout << '\n';
      }
    });
  });
  return 0;
}