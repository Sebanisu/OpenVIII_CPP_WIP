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

#ifndef VIIIARCHIVE_MENUGROUPFILE_H
#define VIIIARCHIVE_MENUGROUPFILE_H
#include "MenuGroupHeader.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Tools/Tools.h"
#include "MenuGroupSectionT.h"
#include "MenuMessages.h"
#include "OpenVIII/SectionData.h"
#include "OpenVIII/BulkSectionData.h"
#include "ComplexStringSection.h"
#include "OpenVIII/MenuGroup/Refine/RefineSection000.h"
#include "OpenVIII/MenuGroup/Refine/RefineSection001.h"
#include "OpenVIII/MenuGroup/Refine/RefineSection002.h"
#include "OpenVIII/MenuGroup/Refine/RefineSection003.h"
#include "OpenVIII/MenuGroup/Refine/RefineSection004.h"
#include "OpenVIII/Graphics/color.h"
#include "OpenVIII/Graphics/tim.h"
#include "OpenVIII/Graphics/ppm.h"
//#include "open_viii/graphics/tim.h"
#include "OpenVIII/Graphics/ppm.h"
#include <sstream>
#include <vector>
namespace open_viii::menu_group {
struct MenuGroupFile
{
private:
  MenuGroupHeader m_menu_group_header{};
  std::vector<char> m_data_buffer{};// maybe should be a view unless we plan to keep this in memory 100%
  template<MenuGroupSectionT sectionT> [[nodiscard]] auto get_section_internal() const
  {
    if constexpr (static_cast<size_t>(sectionT) < MenuGroupHeader::size()) {
      return m_menu_group_header.sections().at(static_cast<size_t>(sectionT));
    } else {
      return nullptr;
    }
  }
  static constexpr std::array TKMNMES_VALUE_ARRAY = {
    MenuGroupSectionT::tkmnmes1,
    MenuGroupSectionT::tkmnmes2,
    MenuGroupSectionT::tkmnmes3,
  };
  static constexpr std::array REFINE_VALUE_ARRAY = {
    MenuGroupSectionT::refine0,
    MenuGroupSectionT::refine1,
    MenuGroupSectionT::refine2,
    MenuGroupSectionT::refine3,
    MenuGroupSectionT::refine4,
  };

  static constexpr std::array COMPLEX_VALUE_ARRAY = {
    MenuGroupSectionT::complex01,
    MenuGroupSectionT::complex02,
    MenuGroupSectionT::complex03,
    MenuGroupSectionT::complex04,
    MenuGroupSectionT::complex05,
    MenuGroupSectionT::complex06,
  };

  static constexpr std::array TIM_VALUE_ARRAY = {
    MenuGroupSectionT::tim00_face1,
    MenuGroupSectionT::tim01_face2,
    MenuGroupSectionT::tim02_magita,
    MenuGroupSectionT::tim03_start,
    MenuGroupSectionT::tim04_mag00_weapons_1st,
    MenuGroupSectionT::tim05_mag07_pets,
    MenuGroupSectionT::tim06_mag00_weapons_1st,
    MenuGroupSectionT::tim07_mag01_weapons_march,
    MenuGroupSectionT::tim08_mag02_weapons_april,
    MenuGroupSectionT::tim09_mag03_weapons_may,
    MenuGroupSectionT::tim10_mag04_weapons_june,
    MenuGroupSectionT::tim11_mag05_weapons_july,
    MenuGroupSectionT::tim12_mag06_weapons_august,
    MenuGroupSectionT::tim13_mag08_occult_1_2,
    MenuGroupSectionT::tim14_mag09_occult_3_4,
    MenuGroupSectionT::tim15_mc00_cards,
    MenuGroupSectionT::tim16_mc01_cards,
    MenuGroupSectionT::tim17_mc02_cards,
    MenuGroupSectionT::tim18_mc03_cards,
    MenuGroupSectionT::tim19_mc04_cards,
    MenuGroupSectionT::tim20_mc05_cards,
    MenuGroupSectionT::tim21_mc06_cards,
    MenuGroupSectionT::tim22_mc07_cards,
    MenuGroupSectionT::tim23_mc08_cards,
    MenuGroupSectionT::tim24_mc09_cards,
    MenuGroupSectionT::tim25_psx_field_controls,
    MenuGroupSectionT::tim26_psx_world_controls,
    MenuGroupSectionT::tim27_psx_battle_controls,
    MenuGroupSectionT::tim28_mag10_cards,
    MenuGroupSectionT::tim29_mag11_cards,
    MenuGroupSectionT::tim30_mag12_cards,
    MenuGroupSectionT::tim31_mag13_battle,
    MenuGroupSectionT::tim32_mag14_battle,
    MenuGroupSectionT::tim33_mag15_chocobo,
    MenuGroupSectionT::tim34_mag16_chocobo,
    MenuGroupSectionT::tim35_mag17_chocobo,
    MenuGroupSectionT::tim36_mag18_chocobo,
    MenuGroupSectionT::tim37_mag19_chocobo,
  };
  static constexpr std::array MES_VALUE_ARRAY = {
    MenuGroupSectionT::mes01,
    MenuGroupSectionT::mes02,
    MenuGroupSectionT::mes03,
    MenuGroupSectionT::mes04,
    MenuGroupSectionT::mes05,
    MenuGroupSectionT::mes06,
    MenuGroupSectionT::mes07,
    MenuGroupSectionT::mes08,
    MenuGroupSectionT::mes09,
    MenuGroupSectionT::mes10,
    MenuGroupSectionT::mes11,
    MenuGroupSectionT::mes12,
    MenuGroupSectionT::mes13,
    MenuGroupSectionT::mes14,
    MenuGroupSectionT::mes15,
    MenuGroupSectionT::mes16,
    MenuGroupSectionT::mes17,
    MenuGroupSectionT::mes18,
    MenuGroupSectionT::mes19,
    MenuGroupSectionT::mes20,
    MenuGroupSectionT::mes21,
    MenuGroupSectionT::mes22,
    MenuGroupSectionT::mes23,
    MenuGroupSectionT::mes24,
    MenuGroupSectionT::mes25,
    MenuGroupSectionT::mes26,
    MenuGroupSectionT::mes27,
    MenuGroupSectionT::mes28,
    MenuGroupSectionT::mes29,
    MenuGroupSectionT::mes30,
    MenuGroupSectionT::mes31,
    MenuGroupSectionT::mes32,
    MenuGroupSectionT::mes33,
    MenuGroupSectionT::mes34,
    MenuGroupSectionT::mes35,
    MenuGroupSectionT::mes36,
    MenuGroupSectionT::mes37,
    MenuGroupSectionT::mes38,
    MenuGroupSectionT::mes39,
    MenuGroupSectionT::mes40,
    MenuGroupSectionT::mes41,
    MenuGroupSectionT::mes42,
    MenuGroupSectionT::mes43,
    MenuGroupSectionT::mes44,
    MenuGroupSectionT::mes45,
  };
  template<typename refineT, MenuGroupSectionT textSectionT, typename sectionBufferT>
  auto get_refine(const sectionBufferT &section_buffer) const
  {
    return open_viii::BulkSectionData<refineT, 1U>(
      section_buffer, get_section_internal<textSectionT>().get_section_buffer(m_data_buffer));
  }

  template<size_t i, size_t count, typename T> constexpr void static_for_tim(const T t) const
  {
    if constexpr (i < count && i < MES_VALUE_ARRAY.size()) {
      constexpr auto val{ TIM_VALUE_ARRAY[i] };
      t(val, get_section<val>());
      static_for_tim<i + 1U, count>(t);
    }
  }
  template<size_t i, size_t count, typename T> constexpr void static_for_mes(const T t) const
  {
    if constexpr (i < count && i < MES_VALUE_ARRAY.size()) {
      constexpr auto val{ MES_VALUE_ARRAY[i] };
      t(val, get_section<val>());
      static_for_mes<i + 1U, count>(t);
    }
  }


  template<size_t i, size_t count, typename T> constexpr void static_for_refine(const T t) const
  {
    if constexpr (i < count && i < REFINE_VALUE_ARRAY.size()) {
      constexpr auto val{ REFINE_VALUE_ARRAY[i] };
      t(val, get_section<val>());
      static_for_refine<i + 1U, count>(t);
    }
  }

  template<size_t i, size_t count, typename T> constexpr void static_for_tkmnmes(const T t) const
  {
    if constexpr (i < count && i < TKMNMES_VALUE_ARRAY.size()) {
      constexpr auto val{ TKMNMES_VALUE_ARRAY[i] };
      t(val, get_section<val>());
      static_for_tkmnmes<i + 1U, count>(t);
    }
  }

public:
  constexpr static std::string_view FILENAME = "mngrp.bin";
  explicit MenuGroupFile(const open_viii::archive::FIFLFS<false> &menu_archive)
    : m_menu_group_header(menu_archive), m_data_buffer(menu_archive.get_entry_data(FILENAME))
  {}

  template<MenuGroupSectionT sectionT> [[nodiscard]] auto get_section() const
  {
    if constexpr (Tools::any_of(sectionT, COMPLEX_VALUE_ARRAY) || sectionT == MenuGroupSectionT::complex_map) {

      const auto map{ get_section_internal<MenuGroupSectionT::complex_map>().get_section_buffer(m_data_buffer) };
      const std::array data = { get_section_internal<MenuGroupSectionT::complex01>().get_section_buffer(m_data_buffer),
        get_section_internal<MenuGroupSectionT::complex02>().get_section_buffer(m_data_buffer),
        get_section_internal<MenuGroupSectionT::complex03>().get_section_buffer(m_data_buffer),
        get_section_internal<MenuGroupSectionT::complex04>().get_section_buffer(m_data_buffer),
        get_section_internal<MenuGroupSectionT::complex05>().get_section_buffer(m_data_buffer),
        get_section_internal<MenuGroupSectionT::complex06>().get_section_buffer(m_data_buffer) };
      return ComplexStringSection(map, data);
    } else {
      [[maybe_unused]] auto section{ get_section_internal<sectionT>() };
      if constexpr (std::is_null_pointer_v<decltype(section)>) {
        return nullptr;
      } else {
        // return mngrphd.Sections().at(id).get_section_buffer(mngrpBuffer);
        [[maybe_unused]] const auto section_buffer{ section.get_section_buffer(m_data_buffer) };
        if constexpr (sectionT == MenuGroupSectionT::tkmnmes1 || sectionT == MenuGroupSectionT::tkmnmes2
                      || sectionT == MenuGroupSectionT::tkmnmes3) {
          return SectionData<MenuMessages>(MenuMessages{ section_buffer }, section_buffer);
        } else if constexpr (Tools::any_of(sectionT, TIM_VALUE_ARRAY)) {
          return graphics::Tim(section_buffer);
        } else if constexpr (Tools::any_of(sectionT, MES_VALUE_ARRAY)) {
          return SectionData<MenuMessagesSection>(MenuMessagesSection{ section_buffer }, section_buffer);
        } else if constexpr (sectionT == MenuGroupSectionT::refine0) {
          return get_refine<open_viii::menu_group::RefineSection000, MenuGroupSectionT::refine_text0>(section_buffer);
        } else if constexpr (sectionT == MenuGroupSectionT::refine1) {
          return get_refine<open_viii::menu_group::RefineSection001, MenuGroupSectionT::refine_text1>(section_buffer);
        } else if constexpr (sectionT == MenuGroupSectionT::refine2) {
          return get_refine<open_viii::menu_group::RefineSection002, MenuGroupSectionT::refine_text2>(section_buffer);
        } else if constexpr (sectionT == MenuGroupSectionT::refine3) {
          return get_refine<open_viii::menu_group::RefineSection003, MenuGroupSectionT::refine_text3>(section_buffer);
        } else if constexpr (sectionT == MenuGroupSectionT::refine4) {
          return get_refine<open_viii::menu_group::RefineSection004, MenuGroupSectionT::refine_text4>(section_buffer);
        }
      }
    }
  }
  void test_tim(const std::string_view &path_input) const
  {
    const auto temp_path = std::filesystem::path(path_input);
    auto path = temp_path.parent_path() / (temp_path.stem().string() + "_tim");
    static_for_tim<0U, TIM_VALUE_ARRAY.size()>(
      [&, this](const auto &section_id, [[maybe_unused]] const graphics::Tim &tim) {
        std::stringstream so{};// TODO have these save in the folder with the mngrp files.
        std::cout << ':' << static_cast<std::size_t>(section_id) << ":  {" << tim.size() << " bytes},\n";
        std::cout << tim << '\n';
        std::cout << tim.check();
        so << path.string() << static_cast<std::size_t>(section_id);
        const auto colors_dump = [&tim, &so](
                                   const std::vector<graphics::Color32<0, 1, 2, 3>> &colors, std::uint16_t num = 0) {
          std::cout << '\n';
          std::stringstream fn{};
          fn << so.str() << "_" << static_cast<std::size_t>(num) << ".tmp";

          // std::cout << "Saved: " << fn.str() << "\n";
          graphics::Ppm::save(colors, tim.width(), tim.height(), fn.str());
        };
        if (tim.clut_rows() > 0) {
          for (std::uint16_t i = 0; i < tim.clut_rows(); i++) {
            colors_dump(tim.get_colors<graphics::Color32<0, 1, 2, 3>>(i), i);
          }
        } else {
          colors_dump(tim.get_colors<graphics::Color32<0, 1, 2, 3>>());
        }
      });
  }
  template<LangT langVal> void test_mes() const
  {
    static_for_mes<0U, MES_VALUE_ARRAY.size()>([&, this](const auto &sectionID, [[maybe_unused]] const auto &mes) {
      std::cout << ':' << static_cast<size_t>(sectionID) << ":  {" << mes.size() << "},\n";
      size_t id = 0;
      for (const auto &subSection : mes) {
        id++;
        if (subSection.offset() == 0) {
          continue;
        }
        const auto temp = subSection.template decoded_string<langVal>(mes.text_span(), 0, true);
        std::cout << "    " << id - 1 << ": {" << subSection.offset() << "} " << Tools::u8_to_sv(temp) << '\n';
      }
    });
  }
  template<LangT langVal> void test_tk_mn_mes() const
  {
    constexpr auto start = 0U;
    static_for_tkmnmes<start, MES_VALUE_ARRAY.size() - start>(
      [&, this](const auto &sectionID, [[maybe_unused]] const auto &tkmnmesPair) {
        std::cout << ':' << static_cast<size_t>(sectionID) << ":\n  {" << tkmnmesPair->sections().size() << ", "
                  << tkmnmesPair->sub_sections().size() << "},\n";
        for (size_t id = 0; id < tkmnmesPair->sections().size() && id < tkmnmesPair->sub_sections().size(); id++) {
          [[maybe_unused]] const auto offset = tkmnmesPair->sections().at(id);
          const auto subSectionGroup = tkmnmesPair->sub_sections().at(id);
          [[maybe_unused]] size_t stringNumber{};
          for (const auto &subSection : subSectionGroup) {
            if (subSection.offset() == 0) {
              continue;
            }
            const auto temp = subSection.template decoded_string<langVal>(tkmnmesPair.text_span(), offset, true);
            std::cout << "    " << stringNumber++ << ": {" << subSection.offset() << "} " << Tools::u8_to_sv(temp)
                      << '\n';
          }
        }
      });
  }

  template<LangT langVal> void test_complex() const
  {
    const auto complex = get_section<MenuGroupSectionT::complex_map>();
    for (size_t i{}; i < complex.size(); i++) {
      [[maybe_unused]] const auto data = complex.at(i);

      [[maybe_unused]] const auto entry{ complex.at(data) };
      std::cout << data << ' ';
      entry.out<langVal>(std::cout);
      std::cout << '\n';
    }
  }
  template<LangT langVal> void test_refine() const
  {
    constexpr auto start = 0U;
    static_for_refine<start, REFINE_VALUE_ARRAY.size() - start>(
      [&, this](const auto &sectionID, [[maybe_unused]] const auto &refineBulkSectionData) {
        std::cout << ':' << static_cast<size_t>(sectionID) << ":\n  {" << refineBulkSectionData.size() << "},\n";

        for (size_t id = 0U; id < 1U; id++) {
          refineBulkSectionData[id].template out<langVal>(std::cout, refineBulkSectionData.text_span());
        }
      });
  }
};
}// namespace open_viii::menu_group
#endif// VIIIARCHIVE_MENUGROUPFILE_H
