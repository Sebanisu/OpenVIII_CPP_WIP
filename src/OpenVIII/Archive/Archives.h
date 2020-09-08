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
#ifndef VIIIARCHIVE_ARCHIVES_H
#define VIIIARCHIVE_ARCHIVES_H
#include "FIFLFS.h"
#include "ZZZ.h"
#include <type_traits>
#include <string_view>
#include "OpenVIII/Strings/LangCommon.h"
#include <variant>
namespace open_viii::archive {
// There are 6 main FIFLFS archives and 2 main zzz archives for ff8 and ff8 remaster.
enum class ArchiveTypeT : std::uint8_t {
  battle,
  field,
  magic,
  main,
  menu,
  world,
  zzz_main,
  zzz_other,
  count,
  first = static_cast<std::int8_t>(battle),
  last = static_cast<std::int8_t>(count) - 1,
};


template<std::signed_integral T> static constexpr bool test_valid_archive_type_t(T archive_type_t)
{
  return (static_cast<std::intmax_t>(archive_type_t) >= static_cast<std::intmax_t>(ArchiveTypeT::first)
          || static_cast<std::intmax_t>(archive_type_t) <= static_cast<std::intmax_t>(ArchiveTypeT::last));
}
template<typename T>
requires(std::unsigned_integral<T> || std::is_same_v<T, ArchiveTypeT>) static constexpr bool test_valid_archive_type_t(
  T archive_type_t)
{
  return (static_cast<std::size_t>(archive_type_t) >= static_cast<std::size_t>(ArchiveTypeT::first)
          || static_cast<std::size_t>(archive_type_t) <= static_cast<std::size_t>(ArchiveTypeT::last));
}
//
// template<ArchiveTypeT archiveTypeT>
// concept ValidArchiveTypeT = (static_cast<std::intmax_t>(archiveTypeT) >=
// static_cast<std::intmax_t>(ArchiveTypeT::first)
//                             || static_cast<std::intmax_t>(archiveTypeT)
//                                  <= static_cast<std::intmax_t>(ArchiveTypeT::last));
template<LangT langVal> struct Archives
{
private:
  std::filesystem::path m_path{};
  std::string m_lang{ LangCommon::to_string<langVal>() };
  FIFLFS<false> m_battle{};
  FIFLFS<true> m_field{};
  FIFLFS<false> m_magic{};
  FIFLFS<false> m_main{};
  FIFLFS<false> m_menu{};
  FIFLFS<false> m_world{};
  std::optional<ZZZ> m_zzz_main{};
  std::optional<ZZZ> m_zzz_other{};
  /**
   * Runs lambda on archive First to Last
   * @tparam First default: ArchiveTypeT::first
   * @tparam Last default: ArchiveTypeT::last
   * @tparam Lambda void (*)(ArchiveTypeT, std::string_view)
   * @param f of type Lambda.
   */
  template<std::intmax_t First = static_cast<std::intmax_t>(ArchiveTypeT::first),
    std::intmax_t Last = static_cast<std::intmax_t>(ArchiveTypeT::last),
    typename lambdaT>
  requires(((test_valid_archive_type_t(First) && test_valid_archive_type_t(Last)) || First == Last + 1)
           && std::invocable<lambdaT,
             const ArchiveTypeT &,
             const std::string &>) void static_for([[maybe_unused]] lambdaT const &lambda)
  {// https://stackoverflow.com/questions/13816850/is-it-possible-to-develop-static-for-loop-in-c
    if constexpr (First <= Last) {
      constexpr auto archiveType_ = std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(First)>{};
      lambda(archiveType_, get_string<archiveType_>());
      static_for<First + 1, Last>(lambda);
    }
  }
  /**
   * Set default language.
   */
  void set_lang() { m_lang = LangCommon::to_string<langVal>(); }
  /**
   * Search for lang.dat from steam 2013 release. TODO find cross platform way to get to remaster config.txt
   * @param path location of ff8
   */
  void set_lang(const std::filesystem::path &path)
  {
    using namespace std::string_literals;
    if (std::empty(m_lang)) {
      m_lang = [this, &path]() {
        {
          // try to read lang.dat from ff8 steam folder
          // lang.dat overrides the explicitly set one.
          const std::filesystem::path &langDatPath = path / "lang.dat";
          if (std::filesystem::exists(langDatPath)) {
            std::ifstream fp = std::ifstream(langDatPath, std::ios::in);
            if (fp.is_open()) {
              fp.seekg(0, std::ios::end);
              const auto length = fp.tellg();
              if (length <= 0) {
                std::string returnValue{};
                returnValue.resize(static_cast<unsigned>(length));// sets to length.
                fp.seekg(0);
                fp.read(returnValue.data(), length);
                fp.close();
                std::cout << "lang.dat detected: ";
                return returnValue;
              }
            }
            fp.close();
          }
        }
        // remaster stores the language value in my documents. I don't see a cross platform way to find this in cpp.
        // will probably need a c# launcher to pass the lang code to this. As .Net has a standard cross platform
        // way to get the documents folder.
        // Documents\My Games\FINAL FANTASY VIII Remastered\Steam\(\d+)\config.txt
        return ""s;// defaulting to english
      }();
    }
    if (std::empty(m_lang)) {
      set_lang();
    }
    std::cout << "lang = " << m_lang << '\n';
  }
  /**
   * Set Path to FF8. Will look for Data and lang- folders.
   * @param path location of FF8.
   */
  void set_path(const std::filesystem::path &path)
  {
    using namespace std::string_literals;
    using namespace std::string_view_literals;
    // assert(!std::empty(m_lang));
    m_path = path;
    const std::filesystem::path &dataPath = m_path / "Data"sv;
    if (std::filesystem::exists(dataPath)) {
      m_path = dataPath;
      {
        static constexpr auto langStart = "lang-"sv;
        std::filesystem::path langFolderPath = m_path / langStart;
        langFolderPath = langFolderPath.string() + m_lang;
        if (!std::empty(m_lang) && std::filesystem::exists(langFolderPath)) {
          m_path = langFolderPath;
        }
      }
    }
  }

  /**
   * TryToAdd Archive of type to archive member variable.
   * @param archiveType_ Type of valid archive.
   * @param path physical path on the hard drive
   * @param nestedPath path located inside the archive. default {}
   * @param offset bytes from front of file where archive is located. 0U = unlimited
   * @param size max size of archive? 0U = unlimited
   * @return
   */
  bool try_add(const ArchiveTypeT &archive_type,
    const std::filesystem::path &path,
    const std::filesystem::path &nested_path = {},
    const size_t &offset = {},
    const size_t &size = {})
  {// this string can be compared to the stem of the filename to determine which archive is try added to.
    // returns nullptr on failure.
    assert(test_valid_archive_type_t(archive_type));
    const auto tryAddToFIFLFS = [&path, &nested_path, &offset, &size](auto &archive) {
      return archive.try_add(path, nested_path, offset, size) != TryAddT::not_part_of_archive;
    };
    const auto tryAddToZZZ = [&path](std::optional<ZZZ> &archive) {
      if (path.has_extension() && Tools::i_equals(path.extension().string(), ZZZ::EXT)) {
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
          {
            const auto pathString = dataItem.get_path_string();
            if (FIFLFS<true>::check_extension(pathString) == 0U) {
              continue;
            }
            static constexpr auto data = "data"sv;
            static constexpr auto langStart = "lang-"sv;
            auto langStartingFilter = std::filesystem::path(data) / langStart;
            auto langStarting = std::filesystem::path(langStartingFilter.string() + m_lang);

            if (Tools::i_starts_with(pathString, langStartingFilter.string())
                && !Tools::i_starts_with(pathString, langStarting.string())) {
              continue;
            }
            auto localPath = std::filesystem::path(pathString);
            static_for<static_cast<intmax_t>(ArchiveTypeT::first), static_cast<intmax_t>(ArchiveTypeT::zzz_main) - 1>(
              [&localPath, &dataItem, &path, this](const ArchiveTypeT &test, const auto &stem) {
                if (!(open_viii::Tools::i_equals(stem, localPath.stem().string()))) {
                  return;
                }
                try_add(test, path, localPath, dataItem.offset(), dataItem.uncompressed_size());
              });
          }
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

public:
  [[nodiscard]] constexpr std::variant<std::monostate, FIFLFS<true>, FIFLFS<false>, std::optional<ZZZ>>
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
  [[maybe_unused]] [[nodiscard]] constexpr std::variant<std::monostate, FIFLFS<true>, FIFLFS<false>, std::optional<ZZZ>>
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
  requires(test_valid_archive_type_t(archiveType_)) constexpr auto get_string() const noexcept
  {// this string can be compared to the stem of the filename to determine which archive is try added to.
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
    } else {
      constexpr auto error = ""sv;
      return error;
    }
  }
  /**
   *
   * @tparam archiveType_
   * @return
   */
  template<ArchiveTypeT archiveType_> requires(test_valid_archive_type_t(archiveType_)) const auto &get() const noexcept
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
    } else {
      return nullptr;
    }
  }
  /**
   * Gets the nested FIFLFS files from Field archive.
   * @param nestedArchive string to filter results. {} will get all nested archives.
   * @return all results.
   */
  auto get_nested(const std::string_view &nested_archive) const { return m_field.get_fiflfs_entries(nested_archive); }
  Archives() = default;
  /**
   * Preloads all archives in the path.
   * @param path that contains FIFLFS files or ZZZ files.
   */
  explicit Archives(const std::filesystem::path &path)
  {
    set_lang(path);
    set_path(path);

    const std::filesystem::directory_options options = std::filesystem::directory_options::skip_permission_denied;
    for (const auto &fileEntry : std::filesystem::directory_iterator(m_path, options))// todo may need sorted.
    {
      const auto &localPath = fileEntry.path();
      if (localPath.has_stem()) {
        static_for([&localPath, this](const ArchiveTypeT &test, const auto &stem) {
          if (!(open_viii::Tools::i_equals(stem, localPath.stem().string()))) {
            return;
          }
          try_add(test, localPath);
        });
      }
    }
  }

  /**
   * Search all the archives. For any of the listed strings.
   * @tparam nested enable or disable searching nested archives from field, default: true.
   * @tparam maxT Max ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::last.
   * @tparam minT Min ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::first.
   * @param filename is an initializer-list of strings to search the archives for. use {} to get all files.
   * @return Returns a vector of pairs of archiveName and results with pairs of index number and embedded filepath.
   */
  template<bool nested = true,
    std::intmax_t maxT = static_cast<std::intmax_t>(ArchiveTypeT::last),
    std::intmax_t minT = static_cast<std::intmax_t>(ArchiveTypeT::first)>
  requires((test_valid_archive_type_t(maxT) || maxT >= minT - 1) && test_valid_archive_type_t(minT))
    [[nodiscard]] std::vector<std::pair<std::string, std::vector<std::pair<unsigned int, std::string>>>> search(
      const std::initializer_list<std::string_view> &filename) const
  {
    if constexpr (maxT >= minT) {
      std::vector<std::pair<std::string, std::vector<std::pair<unsigned int, std::string>>>> vector =
        search<nested, maxT - 1, minT>(filename);
      constexpr auto archiveType_ = std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(maxT)>{};
      auto archive = get<archiveType_>();
      if constexpr (!std::is_null_pointer_v<decltype(archive)>) {
        if constexpr (std::is_same_v<decltype(archive), std::optional<ZZZ>>) {
          if (archive.has_value()) {
            [[maybe_unused]] const auto result = archive->get_all_entries_data(filename);
            if (!std::ranges::empty(result)) {
              vector.emplace_back(std::make_pair(get_string<archiveType_>(), std::move(result)));
            }
          }
        } else if constexpr (std::is_same_v<decltype(archive),
                               FIFLFS<false>> || std::is_same_v<decltype(archive), FIFLFS<true>>) {
          [[maybe_unused]] auto result = archive.get_all_entries_data(filename);
          if (!std::ranges::empty(result)) {
            vector.emplace_back(std::make_pair(get_string<archiveType_>(), std::move(result)));
          }
          if constexpr (nested) {
            auto nestedResult = archive.get_all_nested_entries_data(filename);
            if (!std::ranges::empty(nestedResult)) {
              vector.reserve(std::ranges::size(nestedResult) + std::ranges::size(vector));
              for (auto &item : nestedResult) { vector.emplace_back(std::move(item)); }
            }
          }
        }
      }
      return vector;
    } else {
      return {};
    }
  }


  /**
   * Execute Lambda on all the archives. For any of the listed strings.
   * @tparam nested enable or disable searching nested archives from field, default: true.
   * @tparam maxT Max ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::last.
   * @tparam minT Min ArchiveTypeT value to search in. Converted to std::intmax_t. default: ArchiveTypeT::first.
   * @param filename is an initializer-list of strings to search the archives for. use {} to get all files.
   */
  template<bool nested = true,
    std::intmax_t maxT = static_cast<std::intmax_t>(ArchiveTypeT::last),
    std::intmax_t minT = static_cast<std::intmax_t>(ArchiveTypeT::first),
    typename lambdaT>
  requires((test_valid_archive_type_t(maxT) || maxT >= minT - 1) && test_valid_archive_type_t(minT)
           && std::invocable<lambdaT,
             std::vector<char>,
             std::string>) void execute_on(const std::initializer_list<std::string_view> &filename,
    const lambdaT &lambda) const
  {
    if constexpr (maxT >= minT) {
      execute_on<nested, maxT - 1, minT>(filename, lambda);
      constexpr auto archiveType_ = std::integral_constant<ArchiveTypeT, static_cast<ArchiveTypeT>(maxT)>{};
      std::cout << get_string<archiveType_>() << '\n';
      auto archive = get<archiveType_>();
      if constexpr (!std::is_null_pointer_v<decltype(archive)>) {
        if constexpr (std::is_same_v<decltype(archive), std::optional<ZZZ>>) {
          if (archive.has_value()) {
            archive->execute_on(filename, lambda);
          }
        } else if constexpr (std::is_same_v<decltype(archive),
                               FIFLFS<false>> || std::is_same_v<decltype(archive), FIFLFS<true>>) {
          archive.execute_on(filename, lambda);
          if constexpr (nested) {
            archive.execute_on_nested(filename, lambda);
          }
        }
      }
    }
  }
};// namespace open_viii::Archive
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ARCHIVES_H
