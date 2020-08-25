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
#include "OpenVIII/SectionData.h"
#include "ComplexStringSection.h"
#include "OpenVIII/Graphics/tim.h"
namespace OpenVIII::MenuGroup {
struct MenuGroupFile
{
private:
  MenuGroupHeader menuGroupHeader_{};
  std::vector<char> dataBuffer_{};
  // std::string_view ToStringView() { return { dataBuffer_.data(), dataBuffer_.size() }; }
  template<MenuGroupSectionT sectionT> [[nodiscard]] auto GetSectionInternal() const
  {
    if constexpr (static_cast<size_t>(sectionT) < MenuGroupHeader::size()) {
      return menuGroupHeader_.Sections().at(static_cast<size_t>(sectionT));
    } else {
      return nullptr;
    }
  }
  static constexpr std::array tkmnmesValueArray = {
    MenuGroupSectionT::tkmnmes1,
    MenuGroupSectionT::tkmnmes2,
    MenuGroupSectionT::tkmnmes3,
  };
  static constexpr std::array refineValueArray = {
    MenuGroupSectionT::refine0,
    MenuGroupSectionT::refine1,
    MenuGroupSectionT::refine2,
    MenuGroupSectionT::refine3,
    MenuGroupSectionT::refine4,
  };

  static constexpr std::array complexValueArray = {
    MenuGroupSectionT::complex01,
    MenuGroupSectionT::complex02,
    MenuGroupSectionT::complex03,
    MenuGroupSectionT::complex04,
    MenuGroupSectionT::complex05,
    MenuGroupSectionT::complex06,
  };

  static constexpr std::array timValueArray = {
    MenuGroupSectionT::tim0,
    MenuGroupSectionT::tim1,
    MenuGroupSectionT::tim2,
    MenuGroupSectionT::tim3,
    MenuGroupSectionT::tim4,
    MenuGroupSectionT::tim5,
    MenuGroupSectionT::tim6,
    MenuGroupSectionT::tim7,
    MenuGroupSectionT::tim8,
    MenuGroupSectionT::tim9,
    MenuGroupSectionT::tim10,
    MenuGroupSectionT::tim11,
    MenuGroupSectionT::tim12,
    MenuGroupSectionT::tim13,
    MenuGroupSectionT::tim14,
    MenuGroupSectionT::tim15,
    MenuGroupSectionT::tim16,
    MenuGroupSectionT::tim17,
    MenuGroupSectionT::tim18,
    MenuGroupSectionT::tim19,
    MenuGroupSectionT::tim20,
    MenuGroupSectionT::tim21,
    MenuGroupSectionT::tim22,
    MenuGroupSectionT::tim23,
    MenuGroupSectionT::tim24,
    MenuGroupSectionT::tim25,
    MenuGroupSectionT::tim26,
    MenuGroupSectionT::tim27,
    MenuGroupSectionT::tim28,
    MenuGroupSectionT::tim29,
    MenuGroupSectionT::tim30,
    MenuGroupSectionT::tim31,
    MenuGroupSectionT::tim32,
    MenuGroupSectionT::tim33,
    MenuGroupSectionT::tim34,
    MenuGroupSectionT::tim35,
    MenuGroupSectionT::tim36,
    MenuGroupSectionT::tim37,
  };
  static constexpr std::array mesValueArray = {
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
  auto getRefine(const sectionBufferT &sectionBuffer) const
  {
    return OpenVIII::BulkSectionData<refineT, 1U>(
      sectionBuffer, GetSectionInternal<textSectionT>().GetSectionBuffer(dataBuffer_));
  }

  template<size_t i, size_t count, typename T> constexpr void static_for_tim(const T t) const
  {
    if constexpr (i < count && i < mesValueArray.size()) {
      constexpr auto val{ timValueArray[i] };
      t(val, GetSection<val>());
      static_for_tim<i + 1U, count>(t);
    }
  }
  template<size_t i, size_t count, typename T> constexpr void static_for_mes(const T t) const
  {
    if constexpr (i < count && i < mesValueArray.size()) {
      constexpr auto val{ mesValueArray[i] };
      t(val, GetSection<val>());
      static_for_mes<i + 1U, count>(t);
    }
  }


  template<size_t i, size_t count, typename T> constexpr void static_for_refine(const T t) const
  {
    if constexpr (i < count && i < refineValueArray.size()) {
      constexpr auto val{ refineValueArray[i] };
      t(val, GetSection<val>());
      static_for_refine<i + 1U, count>(t);
    }
  }

  template<size_t i, size_t count, typename T> constexpr void static_for_tkmnmes(const T t) const
  {
    if constexpr (i < count && i < tkmnmesValueArray.size()) {
      constexpr auto val{ tkmnmesValueArray[i] };
      t(val, GetSection<val>());
      static_for_tkmnmes<i + 1U, count>(t);
    }
  }

public:
  explicit MenuGroupFile(const OpenVIII::Archive::FIFLFS<false> &menuArchive)
    : menuGroupHeader_(menuArchive), dataBuffer_(menuArchive.GetEntryData("mngrp.bin"))
  {}

  template<std::size_t sectionTnum> [[nodiscard]] auto GetSectionBuffer() const
  {
    constexpr auto sectionT = static_cast<MenuGroupSectionT>(sectionTnum);
    [[maybe_unused]] const auto section{ GetSectionInternal<sectionT>() };
    if constexpr (std::is_null_pointer_v<decltype(section)>) {
      return nullptr;
    } else {
      return section.GetSectionBuffer(dataBuffer_);
    }
  }
  template<MenuGroupSectionT sectionT> [[nodiscard]] auto GetSection() const
  {
    if constexpr (Tools::any_of(sectionT, complexValueArray) || sectionT == MenuGroupSectionT::complexMap) {

      const auto map{ GetSectionInternal<MenuGroupSectionT::complexMap>().GetSectionBuffer(dataBuffer_) };
      const std::array data = { GetSectionInternal<MenuGroupSectionT::complex01>().GetSectionBuffer(dataBuffer_),
        GetSectionInternal<MenuGroupSectionT::complex02>().GetSectionBuffer(dataBuffer_),
        GetSectionInternal<MenuGroupSectionT::complex03>().GetSectionBuffer(dataBuffer_),
        GetSectionInternal<MenuGroupSectionT::complex04>().GetSectionBuffer(dataBuffer_),
        GetSectionInternal<MenuGroupSectionT::complex05>().GetSectionBuffer(dataBuffer_),
        GetSectionInternal<MenuGroupSectionT::complex06>().GetSectionBuffer(dataBuffer_) };
      return ComplexStringSection(map, data);
    } else {
      [[maybe_unused]] auto section{ GetSectionInternal<sectionT>() };
      if constexpr (std::is_null_pointer_v<decltype(section)>) {
        return nullptr;
      } else {
        // return mngrphd.Sections().at(id).GetSectionBuffer(mngrpBuffer);
        [[maybe_unused]] const auto sectionBuffer{ section.GetSectionBuffer(dataBuffer_) };
        if constexpr (sectionT == MenuGroupSectionT::tkmnmes1 || sectionT == MenuGroupSectionT::tkmnmes2
                      || sectionT == MenuGroupSectionT::tkmnmes3) {
          return SectionData<MenuMessages>(MenuMessages{ sectionBuffer }, sectionBuffer);
        } else if constexpr (Tools::any_of(sectionT, timValueArray)) {
          return Graphics::tim(sectionBuffer);
        } else if constexpr (Tools::any_of(sectionT, mesValueArray)) {
          return SectionData<MenuMessagesSection>(MenuMessagesSection{ sectionBuffer }, sectionBuffer);
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
  }

  void TestTim() const
  {
    static_for_tim<0U, timValueArray.size()>([&, this](const auto &sectionID, [[maybe_unused]] const auto &tim) {
      std::cout << ':' << static_cast<size_t>(sectionID) << ":  {" << tim.size() << " bytes},\n";
      std::cout << tim << '\n';
      // size_t id = 0;
      //           for (const auto &subSection : tim) {
      //             id++;
      //             if (subSection.Offset() == 0) {
      //               continue;
      //             }
      //
      ////             std::cout << "    " << id - 1 << ": {" << subSection.Offset() << "} "
      ////                       << subSection.template DecodedString<langVal>(tim.TextSpan(), 0, true) << '\n';
      //           }
    });
  }
  template<LangT langVal> void TestMes() const
  {
    static_for_mes<0U, mesValueArray.size()>([&, this](const auto &sectionID, [[maybe_unused]] const auto &mes) {
      std::cout << ':' << static_cast<size_t>(sectionID) << ":  {" << mes.size() << "},\n";
      size_t id = 0;
      for (const auto &subSection : mes) {
        id++;
        if (subSection.Offset() == 0) {
          continue;
        }

        std::cout << "    " << id - 1 << ": {" << subSection.Offset() << "} "
                  << subSection.template DecodedString<langVal>(mes.TextSpan(), 0, true) << '\n';
      }
    });
  }
  template<LangT langVal> void TestTkMnMes() const
  {
    constexpr auto start = 0U;
    static_for_tkmnmes<start, mesValueArray.size() - start>(
      [&, this](const auto &sectionID, [[maybe_unused]] const auto &tkmnmesPair) {
        std::cout << ':' << static_cast<size_t>(sectionID) << ":\n  {" << tkmnmesPair->Sections().size() << ", "
                  << tkmnmesPair->SubSections().size() << "},\n";
        for (size_t id = 0; id < tkmnmesPair->Sections().size() && id < tkmnmesPair->SubSections().size(); id++) {
          [[maybe_unused]] const auto offset = tkmnmesPair->Sections().at(id);
          const auto subSectionGroup = tkmnmesPair->SubSections().at(id);
          [[maybe_unused]] size_t stringNumber{};
          for (const auto &subSection : subSectionGroup) {
            if (subSection.Offset() == 0) {
              continue;
            }
            std::cout << "    " << stringNumber++ << ": {" << subSection.Offset() << "} "
                      << subSection.template DecodedString<langVal>(tkmnmesPair.TextSpan(), offset, true) << '\n';
          }
        }
      });
  }

  template<LangT langVal> void TestComplex() const
  {
    const auto complex = GetSection<MenuGroupSectionT::complexMap>();
    for (size_t i{}; i < complex.size(); i++) {
      [[maybe_unused]] const auto data = complex.at(i);

      [[maybe_unused]] const auto entry{ complex.at(data) };
      std::cout << data << ' ';
      entry.out<langVal>(std::cout);
      std::cout << '\n';
    }
  }
  template<LangT langVal> void TestRefine() const
  {
    constexpr auto start = 0U;
    static_for_refine<start, refineValueArray.size() - start>(
      [&, this](const auto &sectionID, [[maybe_unused]] const auto &refineBulkSectionData) {
        std::cout << ':' << static_cast<size_t>(sectionID) << ":\n  {" << refineBulkSectionData.size() << "},\n";
        //             refineBulkSectionData[0].
        //             static_for([&, this]( [[maybe_unused]]const size_t & id, [[maybe_unused]]const auto &
        //             sectionData){
        //
        //                                                     //sectionData.template
        //                                                     out<langVal>(std::cout,refineBulkSectionData.TextSpan());
        //               });

        for (size_t id = 0U; id < 1U; id++) {
          refineBulkSectionData.at(id).template out<langVal>(std::cout, refineBulkSectionData.TextSpan());
        }

        //             for (size_t id = 0; id < tkmnmesPair->Sections().size() && id <
        //             tkmnmesPair->SubSections().size(); id++) {
        //               [[maybe_unused]] const auto offset = tkmnmesPair->Sections().at(id);
        //               const auto subSectionGroup = tkmnmesPair->SubSections().at(id);
        //               [[maybe_unused]] size_t stringNumber{};
        //               for (const auto &subSection : subSectionGroup) {
        //                 if (subSection.Offset() == 0) {
        //                   continue;
        //                 }
        //                 std::cout << "    " << stringNumber++ << ": {" << subSection.Offset() << "} "
        //                           << subSection.template DecodedString<langVal>(tkmnmesPair.TextSpan(), offset, true)
        //                           << '\n';
        //               }
        //}
      });
  }
};
}// namespace OpenVIII::MenuGroup
#endif// VIIIARCHIVE_MENUGROUPFILE_H
