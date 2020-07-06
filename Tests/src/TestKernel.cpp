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
#include "TestKernel.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Paths/Paths.h"
#include "OpenVIII/Kernel/Header.h"
int main()
{
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives(path);
    [[maybe_unused]] const auto &main = archives.Get<OpenVIII::Archive::ArchiveTypeT::Main>();
    std::cout << main << std::endl;
    auto kernel = OpenVIII::Kernel::Header{ main };
    [[maybe_unused]] const auto &buffer = kernel.Buffer();
    std::cout << "kernel.bin " << buffer.size() << " bytes; " << kernel.SectionCount() << " section count\n";
    std::cout << static_cast<int>(OpenVIII::Kernel::SectionTypesT::Count) << std::endl;
    //    const auto &sectionOffsets = kernel.SectionOffsets();
    //    std::for_each(
    //      sectionOffsets.begin(), sectionOffsets.end(),[](const auto &value) { std::cout << value << std::endl; });
    kernel.static_for<static_cast<int>(OpenVIII::Kernel::SectionTypesT::First),
      static_cast<int>(OpenVIII::Kernel::SectionTypesT::Count)>([](auto string, auto span, auto data) {
      std::cout << "  " << string << " - " << std::size(span) << " bytes\n";

      return data;
    });
    kernel.static_for<static_cast<int>(OpenVIII::Kernel::SectionTypesT::First),
      static_cast<int>(OpenVIII::Kernel::SectionTypesT::Count)>([](auto string, auto span, auto data) {
      if constexpr (!std::is_null_pointer_v<decltype(
                      data)> && !std::is_null_pointer_v<decltype(string)> && !std::is_null_pointer_v<decltype(span)>) {
        std::cout << string << " ( " << std::size(span) << "bytes) has " << data.Count() << " entries\n";
        for (size_t i = 0; i < data.Count(); i++) {
          auto entry = data.GetEntry(i);
          std::cout << i << ": ";
          entry.Out(std::cout, data.TextSpan());
          std::cout << '\n';
        }
      }
    });
    //    {
    //
    //      auto data = kernel.GetSectionData<OpenVIII::Kernel::SectionTypes::First>();
    //      std::cout << kernel.GetSectionName<OpenVIII::Kernel::SectionTypes::First>() << " has " << data.Count()
    //                << " entries\n";
    //      for (size_t i = 0; i < data.Count(); i++) {
    //        auto entry = data.GetEntry(i);
    //        std::cout<< i<<": ";
    //        auto name = entry.DecodedName(data.TextSpan());
    //        auto description = entry.DecodedDescription(data.TextSpan());
    //        if(!std::empty(name)) {
    //          std::cout << name;
    //        }
    //        if(!std::empty(description)) {
    //          std::cout << ", " << description;
    //        }
    //        std::cout<< '\n';
    //      }
    //    }
  }
  return 0;
}