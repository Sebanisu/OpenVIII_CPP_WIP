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
#include "OpenVIII/BulkSectionData.h"
#include "OpenVIII/MenuGroup/MenuGroupHeader.h"
#include "OpenVIII/MenuGroup/MenuMessages.h"
#include "OpenVIII/MenuGroup/RefineSection000.h"
#include "OpenVIII/MenuGroup/RefineSection001.h"
#include "OpenVIII/MenuGroup/RefineSection002.h"
#include "OpenVIII/MenuGroup/RefineSection003.h"
#include "OpenVIII/MenuGroup/RefineSection004.h"
#include "OpenVIII/MenuGroup/MenuGroupFile.h"

//[[maybe_unused]] constexpr static std::array tkmnmesSections = { 0, 1, 2 };
//[[maybe_unused]] constexpr static std::array MenuStringsSections = { 87,
//  88,
//  89,
//  90,
//  95,
//  96,
//  97,
//  98,
//  99,
//  100,
//  101,
//  102,
//  103,
//  104,
//  105,
//  106,
//  107,
//  108,
//  109,
//  110,
//  111,
//  112,
//  113,
//  114,
//  115,
//  116,
//  117,
//  118,
//  119,
//  120,
//  121,
//  122,
//  123,
//  124,
//  125,
//  126,
//  160,
//  161,
//  162,
//  163,
//  164,
//  165,
//  166,
//  167,
//  204 };
//[[maybe_unused]] constexpr static std::array refineSections = {//188,189,190,191,192
//  std::make_pair(188U,196U),
//  std::make_pair(189U,197U),
//  std::make_pair(190U,198U),
//  std::make_pair(191U,199U),
//  std::make_pair(192U,200U)
//};
int main()
{
  for (auto &path : OpenVIII::Paths::get()) {
    OpenVIII::Tools::replaceSlashes(path);
    if (!std::filesystem::exists(path)) {
      continue;
    }
    std::cout << path << std::endl;
    constexpr auto coo = OpenVIII::LangT::EN;
    const auto archives = OpenVIII::Archive::Archives<coo>(path);
    [[maybe_unused]] const auto &menu = archives.Get<OpenVIII::Archive::ArchiveTypeT::Menu>();
    std::cout << menu << std::endl;
    auto mngrpfile = OpenVIII::MenuGroup::MenuGroupFile{ menu };
    auto buffer = mngrpfile.GetSectionBuffer<3>();
    std::ofstream ofs{};
    ofs.open("4.bin",std::ios::binary|std::ios::out);
    if(ofs.is_open())
    {
      ofs.write(buffer.data(),static_cast<long>(buffer.size()));
      ofs.close();
      std::cout << std::filesystem::current_path().c_str() << "4.bin";
    }
    return 0;
    mngrpfile.TestComplex<coo>();
    mngrpfile.TestTkMnMes<coo>();
    mngrpfile.TestMes<coo>();
    mngrpfile.TestRefine<coo>();

        auto mngrphd = OpenVIII::MenuGroup::MenuGroupHeader{ menu };
        auto mngrpBuffer = menu.GetEntryData("mngrp.bin");
        std::cout << "mngrphd.bin " << mngrphd.Sections().size() << " sections\n";
        size_t i = 0;
        size_t notSkipped = 0;
        const auto GetSection = [&](std::size_t id) { return mngrphd.Sections().at(id).GetSectionBuffer(mngrpBuffer);
        }; for (const auto &item : mngrphd.Sections()) {
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
                std::cout << "  {" << id - 1 << ": " << subSectionGroup.size() << ", " << offset << '}' << '\n';
                size_t stringnumber{};
                for (const auto &subSection : subSectionGroup) {
                  stringnumber++;
                  if (subSection.Offset() > 0) {
                    std::cout << "    " << stringnumber << ": {" << subSection.Offset() << "} "
                              << subSection.DecodedString<coo>(sectionBuffer, offset, false) << '\n';
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
                          << subSection.DecodedString<coo>(sectionBuffer, 0, true) << '\n';
              }
              //          std::cout << '\n';
            } else if ((notSkipped >= 107) && (notSkipped <= 111)) {
              switch (i) {
              case 189:// 107
              {
                const auto refine =
                  OpenVIII::BulkSectionData<OpenVIII::MenuGroup::RefineSection000>(sectionBuffer.data(),
                  GetSection(196));
                refine.at(0).out<coo>(std::cout, refine.TextSpan());
                //            OpenVIII::MenuGroup::RefineSection000 refine{};
                //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
                //            refine.out<coo>(std::cout,GetSection(196));
                break;
              }
              case 190:// 108
              {
                const auto refine =
                  OpenVIII::BulkSectionData<OpenVIII::MenuGroup::RefineSection001>(sectionBuffer.data(),
                  GetSection(197));
                refine.at(0).out<coo>(std::cout, refine.TextSpan());
                //            OpenVIII::MenuGroup::RefineSection001 refine{};
                //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
                //            refine.out<coo>(std::cout,GetSection(197));
                break;
              }
              case 191:// 109
              {
                const auto refine =
                  OpenVIII::BulkSectionData<OpenVIII::MenuGroup::RefineSection002>(sectionBuffer.data(),
                  GetSection(198));
                refine.at(0).out<coo>(std::cout, refine.TextSpan());
                //            OpenVIII::MenuGroup::RefineSection002 refine{};
                //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
                //            refine.out<coo>(std::cout,GetSection(198));
                break;
              }
              case 192:// 110
              {
                const auto refine =
                  OpenVIII::BulkSectionData<OpenVIII::MenuGroup::RefineSection003>(sectionBuffer.data(),
                  GetSection(199));
                refine.at(0).out<coo>(std::cout, refine.TextSpan());
                //            OpenVIII::MenuGroup::RefineSection003 refine{};
                //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
                //            refine.out<coo>(std::cout,GetSection(199));
                break;
              }
              case 193:// 111
              {
                const auto refine =
                  OpenVIII::BulkSectionData<OpenVIII::MenuGroup::RefineSection004>(sectionBuffer.data(),
                  GetSection(200));
                refine.at(0).out<coo>(std::cout, refine.TextSpan());
                //            OpenVIII::MenuGroup::RefineSection004 refine{};
                //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
                //            refine.out<coo>(std::cout,GetSection(200));
                break;
              }
              }
              // std::cout<< notSkipped << "\n";
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
        //             std::cout << string << " ( " << std::size(span) << "bytes) has " << data.Count() << "entries\n";
        //             for (size_t i = 0; i < data.Count(); i++) {
        //               auto entry = data.at(i);
        //               std::cout << i << ": ";
        //               entry.Out(std::cout, data.TextSpan());
        //               std::cout << '\n';
        //             }
        //           }
        //    });
  }
  return 0;
}
