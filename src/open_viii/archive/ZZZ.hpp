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
#ifndef VIIIARCHIVE_ZZZ_HPP
#define VIIIARCHIVE_ZZZ_HPP
#include "FI.hpp"
#include "FIFLFS.hpp"
#include "FileData.hpp"
#include "open_viii/tools/Tools.hpp"
namespace open_viii::archive {

template<typename T>
concept execute_binary_function
  = std::invocable<T, std::vector<char>, std::string>;
template<typename T>
concept execute_unary_function = std::invocable<T, FileData>;
template<typename T>
concept execute_zzz = execute_binary_function<T> || execute_unary_function<T>;
/**
 * ZZZ file archive from FF8 remaster.
 * @see https://github.com/myst6re/qt-zzz
 */
struct [[maybe_unused]] ZZZ
{
private:
  std::filesystem::path               m_path{};
  /**
   * uint32_t count_{}; fallowed by array of file data
   */
  std::vector<std::istream::off_type> m_offsets{};
  std::vector<std::uint32_t>          m_string_sizes{};
  void
    load_data_from_file()
  {
    m_offsets.push_back(
      static_cast<std::istream::off_type>(sizeof(uint32_t) * 2));
    if (
      m_path.has_extension()
      && tools::i_equals(m_path.extension().string(), EXT)
      && std::filesystem::exists(m_path)) {
      tools::read_from_file(
        [this](std::istream &fp) {
          auto count{ tools::read_val<uint32_t>(fp) };
          if (count == 0) {
            return;
          }
          m_offsets.reserve(count + 1);
          m_string_sizes.reserve(count);

          for (; count != 0; --count) {
            // get string length
            std::array<char, sizeof(std::uint32_t)> tmp{};
            fp.read(
              std::ranges::data(tmp),
              static_cast<std::istream::off_type>(sizeof(uint32_t)));
            auto string_size = std::bit_cast<std::uint32_t>(tmp);
            m_string_sizes.push_back(string_size);

            // seek to next entry
            fp.seekg(
              static_cast<std::istream::off_type>(
                sizeof(std::uint64_t) + sizeof(std::uint32_t) + string_size),
              std::ios::cur);
            m_offsets.push_back(
              fp.tellg()
              + static_cast<std::istream::off_type>(sizeof(std::uint32_t)));
          }
          m_offsets.back()
            -= static_cast<std::istream::off_type>(sizeof(std::uint32_t));
        },
        m_path);
    }
  }

public:
  constexpr static auto EXT = std::string_view(".zzz");
  [[maybe_unused]] [[nodiscard]] const auto &
    path() const noexcept
  {
    return m_path;
  }
  constexpr ZZZ() = default;
  explicit ZZZ(std::filesystem::path path) : m_path(std::move(path))
  {
    load_data_from_file();
  }
  [[nodiscard]] static std::vector<
    std::pair<std::string, open_viii::archive::ZZZ>>
    get_files_from_path(const std::filesystem::path &path)
  {
    std::vector<std::pair<std::string, open_viii::archive::ZZZ>> tmp{};
    tmp.reserve(2);// main and other
    tools::execute_on_directory(
      path,
      {},
      { EXT },
      [&tmp, i = 0](const std::filesystem::path &file_entry) mutable {
        // todo check for language codes to choose correct files
        // this is done for the archives.
        auto &pair = tmp.emplace_back(
          std::piecewise_construct,
          std::forward_as_tuple(tools::get_base_name(file_entry)),
          std::forward_as_tuple(file_entry));
        if (std::empty(pair.first)) {
          pair.first = "__" + std::to_string(i++);
        }
      });
    tmp.shrink_to_fit();
    return tmp;
  }

  using default_filter_lambda = decltype([](auto &&) {
    return true;
  });
  template<execute_zzz FunctionT, typename FilterT = default_filter_lambda>
  void
    execute_on(
      const std::initializer_list<std::string_view> &filename,
      FunctionT                                    &&function,
      FilterT                                      &&filter_lambda = {}) const
  {
    tools::read_from_file(
      [&](std::istream &fp) {
        auto data
          = std::views::iota(
              std::uint32_t{},
              static_cast<std::uint32_t>(std::ranges::size(m_string_sizes)))
          | std::views::transform([this, &fp](std::uint32_t i) {
              std::string name{};
              name.resize(m_string_sizes[i]);
              std::uint64_t offset{};
              std::uint32_t size{};
              {
                fp.seekg(m_offsets[i], std::ios::beg);
                fp.read(
                  std::ranges::data(name),
                  static_cast<std::istream::off_type>(m_string_sizes[i]));
                tl::string::replace_slashes(name);
              }

              {
                std::array<char, sizeof(std::uint64_t)> tmp{};
                fp.read(
                  std::ranges::data(tmp),
                  static_cast<std::istream::off_type>(sizeof(std::uint64_t)));
                offset = std::bit_cast<std::uint64_t>(tmp);
              }
              {
                std::array<char, sizeof(std::uint32_t)> tmp{};
                fp.read(
                  std::ranges::data(tmp),
                  static_cast<std::istream::off_type>(sizeof(std::uint32_t)));
                size = std::bit_cast<std::uint32_t>(tmp);
              }
              return FileData(name, offset, size);
            });
        std::ranges::for_each(
          data,
          [&function, &filename, this, &filter_lambda](
            const open_viii::archive::FileData &dataItem) {
            auto pathString = dataItem.get_path_string();
            if (open_viii::tools::i_find_any(pathString, filename)) {
              if (filter_lambda(pathString)) {
                if constexpr (execute_unary_function<FunctionT>) {
                  function(dataItem);
                }
                else if constexpr (execute_binary_function<FunctionT>) {
                  function(
                    FS::get_entry(m_path, dataItem),
                    std::string(pathString));
                }
              }
            }
          });
      },
      m_path);
  }
  using default_lambda = decltype([](auto &&, auto &&) {});
  template<
    typename lambdaT = default_lambda,
    typename FilterT = default_filter_lambda>
  requires((std::invocable<lambdaT, FIFLFS<false>> || std::invocable<lambdaT, std::vector<char>, std::string>)) void execute_with_nested(
    [[maybe_unused]] const std::initializer_list<std::string_view> & = {},
    [[maybe_unused]] lambdaT                                      && = {},
    [[maybe_unused]] const std::initializer_list<std::string_view> & = {},
    FilterT                                                       && = {}) const
  {
    // only nested archives are handled in the other functions. maybe delete
    // this.
  }
  explicit operator bool() const
  {
    return !std::ranges::empty(m_string_sizes);
  }
  std::size_t
    size() const
  {
    return std::ranges::size(m_string_sizes);
  }
};
inline std::ostream &
  operator<<(std::ostream &os, const ZZZ &data)
{
  return os << '{' << data.path().stem().string() << " zzz {"
            << std::ranges::size(data) << " File Entries from : " << data.path()
            << "}}";
}
inline std::ostream &
  operator<<(std::ostream &os, const std::optional<ZZZ> &data)
{
  if (data.has_value()) {
    return os << data.value();
  }
  return os;
}
}// namespace open_viii::archive
#endif// VIIIARCHIVE_ZZZ_HPP