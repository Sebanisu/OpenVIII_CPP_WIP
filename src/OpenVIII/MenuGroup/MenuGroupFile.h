//
// Created by pcvii on 8/6/2020.
//

#ifndef VIIIARCHIVE_MENUGROUPFILE_H
#define VIIIARCHIVE_MENUGROUPFILE_H
#include "MenuGroupHeader.h"
#include "OpenVIII/Archive/Archives.h"
#include "OpenVIII/Tools/Tools.h"
#include "MenuGroupSectionT.h"
#include "MenuMessages.h"
namespace OpenVIII::MenuGroup {
struct MenuGroupFile
{
private:
  MenuGroupHeader menuGroupHeader_{};
  std::vector<char> dataBuffer_{};
  std::string_view ToStringView()
  {
    return {dataBuffer_.data(),dataBuffer_.size()};
  }
  template<MenuGroupSectionT sectionT> auto GetSectionInternal() const
  {
    if constexpr (static_cast<size_t>(sectionT) < MenuGroupHeader::size()) {
      return menuGroupHeader_.Sections().at(static_cast<size_t>(sectionT));
    } else {
      return nullptr;
    }
  }
  static constexpr std::array mesValueArray = {
    MenuGroupSectionT::mes1,
    MenuGroupSectionT::mes2,
    MenuGroupSectionT::mes3,
    MenuGroupSectionT::mes4,
    MenuGroupSectionT::mes5,
    MenuGroupSectionT::mes6,
    MenuGroupSectionT::mes7,
    MenuGroupSectionT::mes8,
    MenuGroupSectionT::mes9,
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
  auto getRefine(const sectionBufferT sectionBuffer)
  {
    return OpenVIII::SectionData<refineT>(
      sectionBuffer, GetSectionInternal<textSectionT>().GetSectionBuffer(dataBuffer_));
  }
  template<size_t i, size_t count, typename T> constexpr void static_for(const T t)
  {
    if constexpr (i < count) {
      constexpr auto val{ mesValueArray[i] };
      t(GetSection<val>());
      static_for<i + 1U, count>(t);
    }
  }

public:
  explicit MenuGroupFile(const OpenVIII::Archive::FIFLFS<false> &menuArchive)
    : menuGroupHeader_(menuArchive), dataBuffer_(menuArchive.GetEntryData("mngrp.bin"))
  {}

  template<MenuGroupSectionT sectionT> [[nodiscard]] auto GetSection() const
  {
    [[maybe_unused]] auto section{ GetSectionInternal<sectionT>() };
    if constexpr (std::is_null_pointer_v<decltype(section)>) {
      return nullptr;
    } else {

      [[maybe_unused]] const auto sectionBuffer{ section.GetSectionBuffer(dataBuffer_) };
      if constexpr (sectionT == MenuGroupSectionT::tkmnmes1 || sectionT == MenuGroupSectionT::tkmnmes2
                    || sectionT == MenuGroupSectionT::tkmnmes3) {
        return MenuMessages(sectionBuffer);
      } else if constexpr (Tools::any_of(sectionT, mesValueArray)) {
        return MenuMessagesSection(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine0) {
        return getRefine<OpenVIII::MenuGroup::RefineSection000, MenuGroupSectionT::refineText0>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine1) {
        return getRefine<OpenVIII::MenuGroup::RefineSection001, MenuGroupSectionT::refineText1>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine2) {
        return getRefine<OpenVIII::MenuGroup::RefineSection002, MenuGroupSectionT::refineText2>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine3) {
        return getRefine<OpenVIII::MenuGroup::RefineSection003, MenuGroupSectionT::refineText3>(sectionBuffer);
      } else if constexpr (sectionT == MenuGroupSectionT::refine4) {
        return getRefine<OpenVIII::MenuGroup::RefineSection004, MenuGroupSectionT::refineText4>(sectionBuffer);
      }
    }
  }
  template<LangT langVal>
  constexpr bool TestMes()
  {
    static_for<0U, mesValueArray.size()>([&, this]([[maybe_unused]] MenuMessagesSection tkmnmes_) {
              std::cout << "  {" << tkmnmes_.size() << "},\n";
        size_t id{};
        for (const auto &subSection : tkmnmes_) {
            id++;
            if (subSection.Offset() == 0) {
                continue;
            }
            std::cout << "    " << id << ": {" << subSection.Offset() << "} "
                      << subSection.DecodedString<langVal>(ToStringView(), 0, true) << '\n';
        }
    });
    return true;
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_MENUGROUPFILE_H
