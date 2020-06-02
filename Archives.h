//
// Created by pcvii on 5/30/2020.
//
#include "FIFLFS.h"
#include "ZZZ.h"
#include <type_traits>
#include <string_view>
#ifndef VIIIARCHIVE_ARCHIVES_H
#define VIIIARCHIVE_ARCHIVES_H
namespace OpenVIII::Archive {
// There are 6 main FIFLFS archives and 2 main zzz archives for ff8 and ff8 remaster.
enum class ArchiveType {
  Battle,
  Field,
  Magic,
  Main,
  Menu,
  World,
  ZZZMain,
  ZZZOther,
};
struct Archives
{
private:
  std::filesystem::path path_{};
  FIFLFS<false> battle_{};
  FIFLFS<true> field_{};
  FIFLFS<false> magic_{};
  FIFLFS<false> main_{};
  FIFLFS<false> menu_{};
  FIFLFS<false> world_{};
  std::optional<ZZZ> zzzMain_{};
  std::optional<ZZZ> zzzOther_{};
  template<int First, int Last, typename Lambda> void static_for(Lambda const &f)
  {// https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
    if constexpr (First <= Last) {
      constexpr auto archiveType_ = std::integral_constant<ArchiveType, static_cast<ArchiveType>(First)>{};
      f(archiveType_, GetString<archiveType_>());
      static_for<First + 1, Last>(f);
    }
  }

  bool TryAdd(const ArchiveType &archiveType_, const std::filesystem::path &path)
  {// this string can be compared to the stem of the filename to determine which archive is try added to.
    // returns nullptr on failure.

    const auto tryAddToFIFLFS = [&path](auto &archive) { return archive.TryAdd(path) != 0; };
    const auto tryAddToZZZ = [&path](std::optional<ZZZ> &archive) {
           if (path.has_extension() && Tools::iEquals(path.extension().string(), ZZZ::Ext)) {
             archive.emplace(ZZZ(path));
             return true;
           }
           return false;
    };
    switch (archiveType_) {
    case ArchiveType::Battle:
      return tryAddToFIFLFS(battle_);
    case ArchiveType::Field:
      return tryAddToFIFLFS(field_);
    case ArchiveType::Magic:
      return tryAddToFIFLFS(magic_);
    case ArchiveType::Main:
      return tryAddToFIFLFS(main_);
    case ArchiveType::Menu:
      return tryAddToFIFLFS(menu_);
    case ArchiveType::World:
      return tryAddToFIFLFS(world_);
    case ArchiveType::ZZZMain:
      return tryAddToZZZ(zzzMain_);
    case ArchiveType::ZZZOther:
      return tryAddToZZZ(zzzOther_);
    }
    return false;
  }
public:
  template<ArchiveType archiveType_> constexpr static auto GetString()
  {// this string can be compared to the stem of the filename to determine which archive is try added to.
    // returns nullptr on failure.
    using namespace std::literals;
    if constexpr (archiveType_ == ArchiveType::Battle) {
      return "BATTLE"sv;
    } else if constexpr (archiveType_ == ArchiveType::Field) {
      return "FIELD"sv;
    } else if constexpr (archiveType_ == ArchiveType::Magic) {
      return "MAGIC"sv;
    } else if constexpr (archiveType_ == ArchiveType::Main || archiveType_ == ArchiveType::ZZZMain) {
      return "MAIN"sv;
    } else if constexpr (archiveType_ == ArchiveType::Menu) {
      return "MENU"sv;
    } else if constexpr (archiveType_ == ArchiveType::World) {
      return "WORLD"sv;
    } else if constexpr (archiveType_ == ArchiveType::ZZZOther) {
      return "OTHER"sv;
    } else {
      return ""sv;
    }
  }

//  constexpr static auto GetString(ArchiveType archiveType_)
//  {// this string can be compared to the stem of the filename to determine which archive is try added to.
//    // returns nullptr on failure.
//    using namespace std::literals;
//    switch (archiveType_) {
//    case ArchiveType::Battle:
//      return GetString<ArchiveType::Battle>();
//    case ArchiveType::Field:
//      return GetString<ArchiveType::Field>();
//    case ArchiveType::Magic:
//      return GetString<ArchiveType::Magic>();
//    case ArchiveType::Main:
//      return GetString<ArchiveType::Main>();
//    case ArchiveType::Menu:
//      return GetString<ArchiveType::Menu>();
//    case ArchiveType::World:
//      return GetString<ArchiveType::World>();
//    case ArchiveType::ZZZMain:
//      return GetString<ArchiveType::ZZZMain>();
//    case ArchiveType::ZZZOther:
//      return GetString<ArchiveType::ZZZOther>();
//    }
//    return ""sv;
//  }
  template<ArchiveType archiveType_> const auto &Get() const noexcept
  {
    if constexpr (archiveType_ == ArchiveType::Battle) {
      return battle_;
    } else if constexpr (archiveType_ == ArchiveType::Field) {
      return field_;
    } else if constexpr (archiveType_ == ArchiveType::Magic) {
      return magic_;
    } else if constexpr (archiveType_ == ArchiveType::Main) {
      return main_;
    } else if constexpr (archiveType_ == ArchiveType::Menu) {
      return menu_;
    } else if constexpr (archiveType_ == ArchiveType::World) {
      return world_;
    } else if constexpr (archiveType_ == ArchiveType::ZZZMain) {
      return zzzMain_;
    } else if constexpr (archiveType_ == ArchiveType::ZZZOther) {
      return zzzOther_;
    } else {
      return nullptr;
    }
  }
  Archives() = default;
  explicit Archives(const std::filesystem::path &path)
  {
    path_ = path;
    //    static constexpr auto values = std::array<ArchiveType, 8>{ { ArchiveType::Battle,
    //      ArchiveType::Field,
    //      ArchiveType::Magic,
    //      ArchiveType::Main,
    //      ArchiveType::Menu,
    //      ArchiveType::World,
    //      ArchiveType::ZZZMain,
    //      ArchiveType::ZZZOther } };
    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;

    for (const auto &fileEntry : std::filesystem::directory_iterator(path_, options))// todo may need sorted.
    {

      static_for<static_cast<int>(ArchiveType::Field), static_cast<int>(ArchiveType::ZZZOther)>(
        [&fileEntry, this](const ArchiveType &test, const auto &stem) {
          const auto& localPath = fileEntry.path();
          if (!(localPath.has_stem() && OpenVIII::Tools::iEquals(stem, localPath.stem().string()))) {
            return;
          }
          TryAdd(test, fileEntry.path());
        });
      //      for (const auto &i : values) {
      //        if (fileEntry.path().has_stem() && OpenVIII::Tools::iEquals(GetString(i),
      //        fileEntry.path().stem().string())
      //            && TryAdd(i, path_)) {
      //          break;
      //        }
      //      }
    }
  }
};// namespace OpenVIII::Archive
}// namespace OpenVIII::Archive
#endif// VIIIARCHIVE_ARCHIVES_H
