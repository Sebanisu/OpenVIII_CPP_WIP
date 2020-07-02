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
#include "TestMenuGroup.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Paths/Paths.h"
#include "OpenVIII/MenuGroup/mngrphd.h"
#include "OpenVIII/MenuGroup/tkmnmes.h"
int main()
{
  for (auto path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    const auto archives = OpenVIII::Archive::Archives(path);
    [[maybe_unused]] const auto &menu = archives.Get<OpenVIII::Archive::ArchiveTypeT::Menu>();
    std::cout << menu << std::endl;
    auto mngrphd = OpenVIII::MenuGroup::MenuGroupHeader{ menu };
    auto mngrpBuffer = menu.GetEntryData("mngrp.bin");
    std::cout << "mngrphd.bin " << mngrphd.Sections().size() << " sections\n";
    size_t i =0;
    for(const auto & item : mngrphd.Sections())
    {
      const auto sectionBuffer{item.GetSectionBuffer(mngrpBuffer)};
      i++;
      if(!std::empty(sectionBuffer)) {
        std::cout << i << ": {" << item.FileOffset() << ", " << sectionBuffer.size() << "}\n";
        if(i<=3)
        {
          OpenVIII::MenuGroup::tkmnmes tkmnmes_{sectionBuffer};
          std::cout << "{"<<tkmnmes_.Sections().size()<<", "<<tkmnmes_.SubSections().size() <<"},\n";
          size_t id{};
          for(const auto &subSectionGroup : tkmnmes_.SubSections())
          {
            std::cout<< "  {"<<id<<": "<< subSectionGroup.size() <<'}'<<'\n';
            const auto offset{tkmnmes_.Sections().at(id++)};
            size_t stringnumber{};
            for(const auto &subSection : subSectionGroup)
            {
              stringnumber++;
              std::cout <<"    " << stringnumber << ": {"<<subSection.Offset()<<"} " << subSection.DecodedString(sectionBuffer,offset) << '\n';
            }
          }
          std::cout << '\n';
        }
      }
    }
//    [[maybe_unused]] const auto &buffer = mngrphd.Buffer();
//    std::cout << "kernel.bin " << buffer.size() << " bytes; " << mngrphd.SectionCount() << " section count\n";
//    std::cout << static_cast<int>(OpenVIII::Kernel::SectionTypesT::Count) << std::endl;
//    mngrphd.static_for<static_cast<int>(OpenVIII::Kernel::SectionTypesT::First),
//      static_cast<int>(OpenVIII::Kernel::SectionTypesT::Count)>([](auto string, auto span, auto data) {
//           std::cout << "  " << string << " - " << std::size(span) << " bytes\n";
//
//           return data;
//    });
//    mngrphd.static_for<static_cast<int>(OpenVIII::Kernel::SectionTypesT::First),
//      static_cast<int>(OpenVIII::Kernel::SectionTypesT::Count)>([](auto string, auto span, auto data) {
//           if constexpr (!std::is_null_pointer_v<decltype(
//           data)> && !std::is_null_pointer_v<decltype(string)> && !std::is_null_pointer_v<decltype(span)>) {
//             std::cout << string << " ( " << std::size(span) << "bytes) has " << data.Count() << " entries\n";
//             for (size_t i = 0; i < data.Count(); i++) {
//               auto entry = data.GetEntry(i);
//               std::cout << i << ": ";
//               entry.Out(std::cout, data.TextSpan());
//               std::cout << '\n';
//             }
//           }
//    });
  }
  return 0;
}
