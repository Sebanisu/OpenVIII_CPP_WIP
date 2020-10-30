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
#include "TestMenuGroup.hpp"
#include "open_viii/BulkSectionData.hpp"
#include "open_viii/archive/Archives.hpp"
#include "open_viii/menu_group/MenuGroupFile.hpp"
#include "open_viii/menu_group/MenuGroupHeader.hpp"
#include "open_viii/menu_group/MenuMessages.hpp"
#include "open_viii/menu_group/refine/RefineSection000.hpp"
#include "open_viii/menu_group/refine/RefineSection001.hpp"
#include "open_viii/menu_group/refine/RefineSection002.hpp"
#include "open_viii/menu_group/refine/RefineSection003.hpp"
#include "open_viii/menu_group/refine/RefineSection004.hpp"
#include "open_viii/paths/Paths.hpp"


int main()
{
  open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
    std::cout << path << std::endl;
    constexpr auto coo = open_viii::LangT::en;
    const auto archives = open_viii::archive::Archives<coo>(path);
    [[maybe_unused]] const auto &menu = archives.get<open_viii::archive::ArchiveTypeT::menu>();
    std::cout << menu << std::endl;
    auto mngrpfile = open_viii::menu_group::MenuGroupFile{ menu };
    //    auto buffer = mngrpfile.GetSectionBuffer<3>();
    //    std::ofstream ofs{};
    //    ofs.open("4.bin",std::ios::binary|std::ios::out);
    //    if(ofs.is_open())
    //    {
    //      ofs.write(buffer.data(),static_cast<long>(buffer.size()));
    //      ofs.close();
    //      std::cout << std::filesystem::current_path().c_str() << "4.bin";
    //    }

    mngrpfile.test_complex<coo>();
    mngrpfile.test_tk_mn_mes<coo>();
    mngrpfile.test_mes<coo>();
    mngrpfile.test_refine<coo>();
    // mngrpfile.test_tim(menu.get_full_path(open_viii::menu_group::MenuGroupFile::FILENAME));

    // auto mngrphd = open_viii::menu_group::menu_groupHeader{ menu };
    // auto mngrpBuffer = menu.get_entry_data("mngrp.bin");
    // std::cout << "mngrphd.bin " << mngrphd.Sections().size() << " sections\n";
    // size_t i = 0;
    // size_t notSkipped = 0;
    // const auto GetSection = [&](std::size_t id) { return mngrphd.Sections().at(id).GetSectionBuffer(mngrpBuffer);
    //}; for (const auto &item : mngrphd.Sections()) {
    //  const auto sectionBuffer{ item.GetSectionBuffer(mngrpBuffer) };
    //  i++;
    //  if (!std::empty(sectionBuffer)) {
    //    notSkipped++;
    //    std::cout << i - 1 << ":" << notSkipped - 1 << ": {" << item.FileOffset() << ", " << sectionBuffer.size()
    //              << "}\n";
    //    if (i <= 3) {
    //      open_viii::menu_group::MenuMessages tkmnmes_{ sectionBuffer };
    //      std::cout << "{" << tkmnmes_.Sections().size() << ", " << tkmnmes_.SubSections().size() << "},\n";
    //      size_t id{};
    //      for (const auto &subSectionGroup : tkmnmes_.SubSections()) {
    //        const auto offset{ tkmnmes_.Sections().at(id++) };
    //        if (offset == 0) {
    //          continue;
    //        }
    //        std::cout << "  {" << id - 1 << ": " << subSectionGroup.size() << ", " << offset << '}' << '\n';
    //        size_t stringnumber{};
    //        for (const auto &subSection : subSectionGroup) {
    //          stringnumber++;
    //          if (subSection.Offset() > 0) {
    //            std::cout << "    " << stringnumber << ": {" << subSection.Offset() << "} "
    //                      << subSection.decoded_string<coo>(sectionBuffer, offset, false) << '\n';
    //          }
    //        }
    //      }
    //      std::cout << '\n';
    //    } else if ((notSkipped >= 39 && notSkipped <= 74) || (notSkipped >= 82 && notSkipped <= 89)
    //               || (notSkipped == 117)) {
    //      open_viii::menu_group::MenuMessagesSection tkmnmes_{ sectionBuffer };
    //      std::cout << "  {" << tkmnmes_.size() << "},\n";
    //      size_t id{};
    //      for (const auto &subSection : tkmnmes_) {
    //        id++;
    //        if (subSection.Offset() == 0) {
    //          continue;
    //        }
    //        std::cout << "    " << id << ": {" << subSection.Offset() << "} "
    //                  << subSection.decoded_string<coo>(sectionBuffer, 0, true) << '\n';
    //      }
    //      //          std::cout << '\n';
    //    } else if ((notSkipped >= 107) && (notSkipped <= 111)) {
    //      switch (i) {
    //      case 189:// 107
    //      {
    //        const auto refine =
    //          open_viii::BulkSectionData<open_viii::menu_group::RefineSection000>(sectionBuffer.data(),
    //          GetSection(196));
    //        refine.at(0).out<coo>(std::cout, refine.TextSpan());
    //        //            open_viii::menu_group::RefineSection000 refine{};
    //        //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
    //        //            refine.out<coo>(std::cout,GetSection(196));
    //        break;
    //      }
    //      case 190:// 108
    //      {
    //        const auto refine =
    //          open_viii::BulkSectionData<open_viii::menu_group::RefineSection001>(sectionBuffer.data(),
    //          GetSection(197));
    //        refine.at(0).out<coo>(std::cout, refine.TextSpan());
    //        //            open_viii::menu_group::RefineSection001 refine{};
    //        //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
    //        //            refine.out<coo>(std::cout,GetSection(197));
    //        break;
    //      }
    //      case 191:// 109
    //      {
    //        const auto refine =
    //          open_viii::BulkSectionData<open_viii::menu_group::RefineSection002>(sectionBuffer.data(),
    //          GetSection(198));
    //        refine.at(0).out<coo>(std::cout, refine.TextSpan());
    //        //            open_viii::menu_group::RefineSection002 refine{};
    //        //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
    //        //            refine.out<coo>(std::cout,GetSection(198));
    //        break;
    //      }
    //      case 192:// 110
    //      {
    //        const auto refine =
    //          open_viii::BulkSectionData<open_viii::menu_group::RefineSection003>(sectionBuffer.data(),
    //          GetSection(199));
    //        refine.at(0).out<coo>(std::cout, refine.TextSpan());
    //        //            open_viii::menu_group::RefineSection003 refine{};
    //        //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
    //        //            refine.out<coo>(std::cout,GetSection(199));
    //        break;
    //      }
    //      case 193:// 111
    //      {
    //        const auto refine =
    //          open_viii::BulkSectionData<open_viii::menu_group::RefineSection004>(sectionBuffer.data(),
    //          GetSection(200));
    //        refine.at(0).out<coo>(std::cout, refine.TextSpan());
    //        //            open_viii::menu_group::RefineSection004 refine{};
    //        //            std::memcpy(&refine,sectionBuffer.data(),sizeof(refine));
    //        //            refine.out<coo>(std::cout,GetSection(200));
    //        break;
    //      }
    //      }
    //      // std::cout<< notSkipped << "\n";
    //    }
    //    //        if(sectionBuffer.size() >= sizeof(open_viii::menu_group::RefineSection000))
    //    //        {
    //    //          //exit(0);
    //    //        }

    //    if(sectionBuffer.at(0) == 0x10 && sectionBuffer.at(1) == 0)
    //    {
    //      std::cout<< "TIM?" << "\n";
    //    }
    //          }
    //       }
    //    [[maybe_unused]] const auto &buffer = mngrphd.Buffer();
    //    std::cout << "kernel.bin " << buffer.size() << " bytes; " << mngrphd.SectionCount() << " section count\n";
    //    std::cout << static_cast<int>(open_viii::kernel::SectionTypesT::Count) << std::endl;
    //    mngrphd.static_for<static_cast<int>(open_viii::kernel::SectionTypesT::First),
    //      static_cast<int>(open_viii::kernel::SectionTypesT::Count)>([](auto string, auto span, auto data) {
    //           std::cout << "  " << string << " - " << std::ranges::size(span) << " bytes\n";
    //
    //           return data;
    //    });
    //    mngrphd.static_for<static_cast<int>(open_viii::kernel::SectionTypesT::First),
    //      static_cast<int>(open_viii::kernel::SectionTypesT::Count)>([](auto string, auto span, auto data) {
    //           if constexpr (!std::is_null_pointer_v<decltype(
    //           data)> && !std::is_null_pointer_v<decltype(string)> && !std::is_null_pointer_v<decltype(span)>) {
    //             std::cout << string << " ( " << std::ranges::size(span) << "bytes) has " << data.Count() << "entries\n";
    //             for (size_t i = 0; i < data.Count(); i++) {
    //               auto entry = data.at(i);
    //               std::cout << i << ": ";
    //               entry.Out(std::cout, data.TextSpan());
    //               std::cout << '\n';
    //             }
    //           }
    //    });
  });
  return 0;
}
