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
#ifndef VIIIARCHIVE_ARCHIVES_HPP
#define VIIIARCHIVE_ARCHIVES_HPP
#include "ArchivesTypesT.hpp"
#include "FIFLFS.hpp"
#include "ZZZ.hpp"
#include "open_viii/strings/LangCommon.hpp"
#include <string_view>
#include <type_traits>
#include <variant>
namespace open_viii::archive {
struct Archives
{
private:
  std::string           m_lang{};
  std::filesystem::path m_path{};
  FIFLFS<false>         m_battle{};
  FIFLFS<true>          m_field{};
  FIFLFS<false>         m_magic{};
  FIFLFS<false>         m_main{};
  FIFLFS<false>         m_menu{};
  FIFLFS<false>         m_world{};
  std::optional<ZZZ>    m_zzz_main{};
  std::optional<ZZZ>    m_zzz_other{};
  /**
   * Search for lang.dat from steam 2013 release.
   * @todo find cross platform way to get to remaster config.txt
   * @param path location of ff8
   */
  [[nodiscard]] std::string set_lang(const std::filesystem::path &path,
                                     std::string_view             lang) const
  {
    using namespace std::string_literals;
    return [&path, &lang]() -> std::string {
      {
        // try to read lang.dat from ff8 steam folder
        // lang.dat overrides the explicitly set one.
        const std::filesystem::path &langDatPath = path / "lang.dat";
        if (std::filesystem::exists(langDatPath)) {
          return tools::read_entire_file<std::basic_string<char>>(langDatPath);
        }
      }
      // remaster stores the language value in my documents. I don't see a
      // cross platform way to find this in cpp. will probably need a c#
      // launcher to pass the lang code to this. As .Net has a standard cross
      // platform way to get the documents folder. Documents\My Games\FINAL
      // FANTASY VIII Remastered\Steam\(\d+)\config.txt
      return std::string(lang);// defaulting to english
    }();
  }
  /**
   * Set Path to FF8. Will look for Data and lang- folders.
   * @param path location of FF8.
   */
  static std::filesystem::path set_path(std::filesystem::path path,
                                        const std::string &   lang)
  {
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    // assert(!std::empty(m_lang));
    // auto path = in_path;
    const std::filesystem::path &dataPath = path / "Data"sv;
    if (std::filesystem::exists(dataPath)) {
      path = dataPath;
      {
        static constexpr auto langStart      = "lang-"sv;
        std::filesystem::path langFolderPath = path / langStart;
        langFolderPath                       = langFolderPath.string() + lang;
        if (std::filesystem::exists(langFolderPath)) {
          path = langFolderPath;
        }
      }
    }
    return path;
  }
  [[nodiscard]] bool check_lang_path(const auto &pathString) const
  {
    // no languages in zzz for:
    // field, magic, world
    // languages in zzz for only:
    // main menu battle
    static constexpr auto data      = "data";
    static constexpr auto langStart = "lang-";
    const auto            langStartingFilter =
      std::filesystem::__cxx11::path(data) / langStart;
    const auto langStarting =
      std::filesystem::path(langStartingFilter.string() + m_lang);
    return tools::i_starts_with(pathString, langStartingFilter.string())
           && !tools::i_starts_with(pathString, langStarting.string());
  }
  /**
   * TryToAdd archive of type to archive member variable.
   * @param archiveType_ Type of valid archive.
   * @param path physical path on the hard drive
   * @param nestedPath path located inside the archive. default {}
   * @param offset bytes from front of file where archive is located. 0U =
   * unlimited
   * @param size max size of archive? 0U = unlimited
   * @return
   */
  template<FI_Like fiT>
  bool try_add(const ArchiveTypeT &         archive_type,
               fiT                          fi,
               const std::filesystem::path &path,
               const std::filesystem::path &nested_path)
  {
    // this string can be compared to the stem of the filename to determine
    // which archive is try added to.
    // returns nullptr on failure.
    assert(test_valid_archive_type_t(archive_type));
    const auto tryAddToFIFLFS = [&path, &nested_path, &fi](auto &archive) {
      TryAddT add = archive.try_add(fi, path, nested_path);
      return add == TryAddT::added_to_archive;
    };
    const auto tryAddToZZZ = [&path](std::optional<ZZZ> &archive) {
      if (path.has_extension()
          && tools::i_equals(path.extension().string(), ZZZ::EXT)) {
        archive.emplace(path);
        return true;
      }
      return false;
    };
    switch (archive_type) {
    case ArchiveTypeT::battle:
      return tryAddToFIFLFS(m_battle);
    case ArchiveTypeT::field:
      return tryAddToFIFLFS(m_field);
    case ArchiveTypeT::magic:
      return tryAddToFIFLFS(m_magic);
    case ArchiveTypeT::main:
      return tryAddToFIFLFS(m_main);
    case ArchiveTypeT::menu:
      return tryAddToFIFLFS(m_menu);
    case ArchiveTypeT::world:
      return tryAddToFIFLFS(m_world);
    case ArchiveTypeT::zzz_main: {
      if (tryAddToZZZ(m_zzz_main)) {
        using namespace std::string_literals;
        using namespace std::string_view_literals;
        for (const auto &dataItem : m_zzz_main->data()) {
          const auto pathString = dataItem.get_path_string();
          if (FIFLFS<true>::check_extension(pathString) == fiflfsT::none) {
            continue;
          }
          if (check_lang_path(pathString)) {
            continue;
          }
          auto localPath = std::filesystem::path(pathString);
          loop<static_cast<intmax_t>(ArchiveTypeT::begin),
               static_cast<intmax_t>(ArchiveTypeT::zzz_main) - 1>(
            [&localPath, &dataItem, &path, this](const ArchiveTypeT     test,
                                                 const std::string_view stem) {
              if (!(open_viii::tools::i_equals(stem,
                                               localPath.stem().string()))) {
                return true;
              }
              try_add(test, dataItem, path, localPath);
              return true;
            });
        }
        return true;
      }
      return false;
    }
    case ArchiveTypeT::zzz_other:
      return tryAddToZZZ(m_zzz_other);
    case ArchiveTypeT::count:
      break;
    }
    return false;
  }
  template<typename lambdaT>
  static constexpr bool valid_static_for_lambda_type =
    (std::is_invocable_r_v<bool, lambdaT, ArchiveTypeT, std::string_view>);
  void populate_archives_from_path()
  {
    tools::execute_on_directory(
      m_path,
      {},
      { FI::EXT, FS::EXT, fl::EXT, ZZZ::EXT },
      [this](const std::filesystem::path &localPath) {
        if (localPath.has_stem()) {
          loop([&localPath, this](const ArchiveTypeT     archiveTypeT,
                                  const std::string_view stem) {
            if (!(tools::i_equals(stem, localPath.stem().string()))) {
              return true;
            }
            try_add(
              archiveTypeT,
              FI(static_cast<uint32_t>(std::filesystem::file_size(localPath)),
                 0U),
              localPath,
              localPath);
            return true;
          });
        }
      });
  }
  template<typename lambdaT>
  static constexpr bool takes_valid_archive_type =
    (std::is_invocable_r_v<bool, lambdaT, decltype(m_battle)>)
    || (std::is_invocable_r_v<bool, lambdaT, decltype(m_field)>)
    || std::is_invocable_r_v<bool, lambdaT, decltype(*m_zzz_main)>;
  template<typename lambdaT>
  [[maybe_unused]] static constexpr bool valid_execute_on_lambda =
    (std::invocable<lambdaT, std::vector<char>, std::string>)
    || (std::invocable<lambdaT, std::string, std::string>);
  auto test_valid_lambda() const
  {
    return [](const auto &archive) -> bool {
      return static_cast<bool>(archive);
    };
  }
  template<bool nested = true, typename lambdaT>
  requires(valid_execute_on_lambda<lambdaT>) auto get_execute_on_lambda(
    const std::initializer_list<std::string_view> &filename,
    const lambdaT &                                lambda) const
  {
    return [&filename, &lambda](const auto &archive) -> bool {
      archive.execute_on(filename, lambda);
      if constexpr (nested && requires(decltype(archive) a, lambdaT l) {
                      a.execute_with_nested({}, l, {});
                    }) {
        archive.execute_with_nested({}, lambda, filename);
      }
      return true;
    };
  }
public:
  /**
   *
   * @tparam archiveType_
   * @return
   */
  template<ArchiveTypeT archiveType_>
  requires(test_valid_archive_type_t(archiveType_)) const
    auto &get() const noexcept
  {
    if constexpr (archiveType_ == ArchiveTypeT::battle) {
      return m_battle;
    } else if constexpr (archiveType_ == ArchiveTypeT::field) {
      return m_field;
    } else if constexpr (archiveType_ == ArchiveTypeT::magic) {
      return m_magic;
    } else if constexpr (archiveType_ == ArchiveTypeT::main) {
      return m_main;
    } else if constexpr (archiveType_ == ArchiveTypeT::menu) {
      return m_menu;
    } else if constexpr (archiveType_ == ArchiveTypeT::world) {
      return m_world;
    } else if constexpr (archiveType_ == ArchiveTypeT::zzz_main) {
      return m_zzz_main;
    } else if constexpr (archiveType_ == ArchiveTypeT::zzz_other) {
      return m_zzz_other;
    }
  }
  [[nodiscard]] explicit operator bool() const noexcept
  {
    return test_set();
  }
  [[nodiscard]] std::
    variant<std::monostate, FIFLFS<true>, FIFLFS<false>, std::optional<ZZZ>>
    get(const std::string_view &name, bool zzz = false) const
  {
    if (!zzz) {
      if (get_string<ArchiveTypeT::battle>() == name) {
        return get<ArchiveTypeT::battle>();
      }
      if (get_string<ArchiveTypeT::field>() == name) {
        return get<ArchiveTypeT::field>();
      }
      if (get_string<ArchiveTypeT::magic>() == name) {
        return get<ArchiveTypeT::magic>();
      }
      if (get_string<ArchiveTypeT::main>() == name) {
        return get<ArchiveTypeT::main>();
      }
      if (get_string<ArchiveTypeT::menu>() == name) {
        return get<ArchiveTypeT::menu>();
      }
      if (get_string<ArchiveTypeT::world>() == name) {
        return get<ArchiveTypeT::world>();
      }
      if (get_string<ArchiveTypeT::field>().starts_with(name)) {
        std::cout << " -- nested -- " << name << '\n';
        return std::monostate{};
      }
    } else {
      if (get_string<ArchiveTypeT::zzz_main>() == name) {
        return get<ArchiveTypeT::zzz_main>();
      }
      if (get_string<ArchiveTypeT::zzz_other>() == name) {
        return get<ArchiveTypeT::zzz_other>();
      }
    }
    return std::monostate{};
  }
  [[maybe_unused]] [[nodiscard]] std::
    variant<std::monostate, FIFLFS<true>, FIFLFS<false>, std::optional<ZZZ>>
    get(const ArchiveTypeT &type) const
  {
    if (ArchiveTypeT::battle == type) {
      return get<ArchiveTypeT::battle>();
    }
    if (ArchiveTypeT::field == type) {
      return get<ArchiveTypeT::field>();
    }
    if (ArchiveTypeT::magic == type) {
      return get<ArchiveTypeT::magic>();
    }
    if (ArchiveTypeT::main == type) {
      return get<ArchiveTypeT::main>();
    }
    if (ArchiveTypeT::menu == type) {
      return get<ArchiveTypeT::menu>();
    }
    if (ArchiveTypeT::world == type) {
      return get<ArchiveTypeT::world>();
    }
    if (ArchiveTypeT::zzz_main == type) {
      return get<ArchiveTypeT::zzz_main>();
    }
    if (ArchiveTypeT::zzz_other == type) {
      return get<ArchiveTypeT::zzz_other>();
    }
    return std::monostate{};
  }
  template<ArchiveTypeT archiveType_>
  requires(test_valid_archive_type_t(archiveType_)) constexpr std::string_view
    get_string() const noexcept
  {// this string can be compared to the stem of the filename to determine which
   // archive is try added to.
    // returns nullptr on failure.
    using namespace std::literals;
    if constexpr (archiveType_ == ArchiveTypeT::battle) {
      constexpr auto battle = "BATTLE"sv;
      return battle;
    } else if constexpr (archiveType_ == ArchiveTypeT::field) {
      constexpr auto field = "FIELD"sv;
      return field;
    } else if constexpr (archiveType_ == ArchiveTypeT::magic) {
      constexpr auto magic = "MAGIC"sv;
      return magic;
    } else if constexpr (archiveType_ == ArchiveTypeT::main) {
      constexpr auto main = "MAIN"sv;
      return main;
    } else if constexpr (archiveType_ == ArchiveTypeT::menu) {
      constexpr auto menu = "MENU"sv;
      return menu;
    } else if constexpr (archiveType_ == ArchiveTypeT::world) {
      constexpr auto world = "WORLD"sv;
      return world;
    } else if constexpr (archiveType_ == ArchiveTypeT::zzz_other) {
      constexpr auto other = "OTHER"sv;
      return other;
    } else if constexpr (archiveType_ == ArchiveTypeT::zzz_main) {
      constexpr auto main = "MAIN"sv;
      return main;
    }
  }
  /**
   * Gets the nested FIFLFS files from Field archive.
   * @param nestedArchive string to filter results. {} will get all nested
   * archives.
   * @return all results.
   */
  auto get_nested(
    const std::initializer_list<std::string_view> &nested_archive) const
  {
    return m_field.get_fiflfs_entries(nested_archive);
  }
  Archives() = delete;
  /**
   * Preloads all archives in the path.
   * @param path that contains FIFLFS files or ZZZ files.
   */
  explicit Archives(const std::filesystem::path &path, std::string_view lang)
    : m_lang(set_lang(path, lang)), m_path(set_path(path, m_lang))
  {
    populate_archives_from_path();
  }
  /**
   * Loop through each of the archives.
   * @tparam minT min archive
   * @tparam maxT max archive
   * @tparam lambdaT type of lambda function that returns bool and takes archive
   * types. The bool returned short circuits the loop.
   * @param lambda lambda of type lambdaT
   * @return true if ran to completion
   */
  template<std::intmax_t minT = static_cast<std::intmax_t>(ArchiveTypeT::begin),
           std::intmax_t maxT = static_cast<std::intmax_t>(ArchiveTypeT::end),
           typename lambdaT>
  requires(
    test_valid_archive_type_t(minT, maxT, true)
    && ((takes_valid_archive_type<lambdaT>)
        || (valid_static_for_lambda_type<lambdaT>))) bool loop(const lambdaT
                                                                 &lambda) const
  {
    bool ret{ true };
    if constexpr (test_valid_archive_type_t(minT, maxT - 1)) {
      ret = loop<minT, maxT - 1>(lambda);
      if (!ret)
        return ret;
    }
    if constexpr (test_valid_archive_type_t(maxT)) {
      constexpr auto archiveType_ =
        std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(maxT)>{};
      if constexpr (valid_static_for_lambda_type<lambdaT>) {
        ret = lambda(archiveType_, get_string<archiveType_>());
        return ret;
      } else if (takes_valid_archive_type<lambdaT>) {
        const auto &archive = get<archiveType_>();
        std::cout << get_string<archiveType_>();
        if constexpr (requires(decltype(archive) a) { a.has_value(); }) {
          if (archive.has_value()) {
            ret = lambda(*archive);
          }
        } else {
          ret = lambda(archive);
        }
      }
    }
    return ret;
  }
  template<ArchiveTypeT... aT, typename lambdaT>
  requires(test_valid_archive_type_t(aT)
           && ...) bool specify(const lambdaT &lambda)
  {
    return (loop<aT, aT>(lambda) && ...);
  }
  bool test_set() const
  {
    return loop(test_valid_lambda());
  }
  template<ArchiveTypeT... aT> requires(sizeof...(aT) > 0) bool test_set()
  {
    return specify<aT...>(test_valid_lambda());
  }
  template<bool nested = true, typename lambdaT>
  requires(valid_execute_on_lambda<lambdaT>) bool execute_on(
    const std::initializer_list<std::string_view> &filename,
    const lambdaT &                                lambda) const
  {
    return loop(get_execute_on_lambda<true>(filename, lambda));
  }
  template<bool nested = true, ArchiveTypeT... aT, typename lambdaT>
  requires((valid_execute_on_lambda<lambdaT>)&&sizeof...(aT)
           > 0) bool execute_on(const std::initializer_list<std::string_view>
                                  &            filename,
                                const lambdaT &lambda) const
  {
    return specify<aT...>(get_execute_on_lambda<true>(filename, lambda));
  }
};
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ARCHIVES_HPP
