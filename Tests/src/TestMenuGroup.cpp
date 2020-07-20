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
#include "OpenVIII/MenuGroup/MenuGroupHeader.h"
#include "OpenVIII/MenuGroup/MenuMessages.h"
#include "OpenVIII/MenuGroup/RefineSection000.h"
#include "OpenVIII/MenuGroup/RefineSection001.h"
#include "OpenVIII/MenuGroup/RefineSection002.h"
#include "OpenVIII/MenuGroup/RefineSection003.h"
#include "OpenVIII/MenuGroup/RefineSection004.h"
[[maybe_unused]] constexpr static std::array tkmnmesSections = { 0, 1, 2 };
[[maybe_unused]] constexpr static std::array MenuStringsSections = { 87,
  88,
  89,
  90,
  95,
  96,
  97,
  98,
  99,
  100,
  101,
  102,
  103,
  104,
  105,
  106,
  107,
  108,
  109,
  110,
  111,
  112,
  113,
  114,
  115,
  116,
  117,
  118,
  119,
  120,
  121,
  122,
  123,
  124,
  125,
  126,
  160,
  161,
  162,
  163,
  164,
  165,
  166,
  167,
  204 };
[[maybe_unused]] constexpr static std::array refineSections = {//188,189,190,191,192
  std::make_pair(188U,196U),
  std::make_pair(189U,197U),
  std::make_pair(190U,198U),
  std::make_pair(191U,199U),
  std::make_pair(192U,200U)
};
int main()
{
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    constexpr auto coo = std::string_view("jp");
    const auto archives = OpenVIII::Archive::Archives(path, coo);
    [[maybe_unused]] const auto &menu = archives.Get<OpenVIII::Archive::ArchiveTypeT::Menu>();
    std::cout << menu << std::endl;
    auto mngrphd = OpenVIII::MenuGroup::MenuGroupHeader{ menu };
    auto mngrpBuffer = menu.GetEntryData("mngrp.bin");
    std::cout << "mngrphd.bin " << mngrphd.Sections().size() << " sections\n";
    size_t i = 0;
    size_t notSkipped = 0;
    for (const auto &item : mngrphd.Sections()) {
      const auto sectionBuffer{ item.GetSectionBuffer(mngrpBuffer) };
      i++;
      if (!std::empty(sectionBuffer)) {
        notSkipped++;
        std::cout << i - 1 << ":" << notSkipped - 1 << ": {" << item.FileOffset() << ", " << sectionBuffer.size()
                  << "}\n";
        if (i <= 3) {
          OpenVIII::MenuGroup::MenuMessages tkmnmes_{ sectionBuffer };
          std::cout << "{" << tkmnmes_.Sections().size() << ", " << tkmnmes_.SubSections().size() << "},\n";
          size_t id{};
          for (const auto &subSectionGroup : tkmnmes_.SubSections()) {
            const auto offset{ tkmnmes_.Sections().at(id++) };
            if (offset == 0) {
              continue;
            }
            std::cout << "  {" << id << ": " << subSectionGroup.size() << ", " << offset << '}' << '\n';
            size_t stringnumber{};
            for (const auto &subSection : subSectionGroup) {
              stringnumber++;
              if (subSection.Offset() > 0) {
                std::cout << "    " << stringnumber << ": {" << subSection.Offset() << "} "
                          << subSection.DecodedString(sectionBuffer, offset, false, coo) << '\n';
              }
            }
          }
          std::cout << '\n';
        } else if ((notSkipped >= 39 && notSkipped <= 74) || (notSkipped >= 82 && notSkipped <= 89)
                   || (notSkipped == 117)) {
          OpenVIII::MenuGroup::MenuMessagesSection tkmnmes_{ sectionBuffer };
          std::cout << "  {" << tkmnmes_.size() << "},\n";
          size_t id{};
          for (const auto &subSection : tkmnmes_) {
            id++;
            if (subSection.Offset() == 0) {
              continue;
            }
            std::cout << "    " << id << ": {" << subSection.Offset() << "} "
                      << subSection.DecodedString(sectionBuffer, 0, true, coo) << '\n';
          }
          //          std::cout << '\n';
        }
        else if( (notSkipped >= 107) &&  (notSkipped <= 111))
        {
          switch (notSkipped)
          {
          case 107:
          {
            OpenVIII::MenuGroup::RefineSection000 refine{};
            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
            std::cout << refine;
            break;
          }
          case 108:
          {
            OpenVIII::MenuGroup::RefineSection001 refine{};
            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
            std::cout << refine;
            break;
          }
          case 109:
          {
            OpenVIII::MenuGroup::RefineSection002 refine{};
            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
            std::cout << refine;
            break;
          }
          case 110:
          {
            OpenVIII::MenuGroup::RefineSection003 refine{};
            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
            std::cout << refine;
            break;
          }
          case 111:
          {
            OpenVIII::MenuGroup::RefineSection004 refine{};
            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
            std::cout << refine;
            break;
          }
          }
          //std::cout<< notSkipped << "\n";
        }
//        if(sectionBuffer.size() >= sizeof(OpenVIII::MenuGroup::RefineSection000))
//        {
//          //exit(0);
//        }
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
