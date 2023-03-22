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
#ifndef VIIIARCHIVE_FIFLFS_HPP
#define VIIIARCHIVE_FIFLFS_HPP
#include "FIFLFS_sentinel.hpp"
#include "fiflfsT.hpp"
#include "FIOutIterator.hpp"
#include "Grouping.hpp"
#include "TryAddT.hpp"
#include <sstream>
#include <string>
namespace open_viii::archive {

/**
 * @class FIFLFSArchiveIterator
 * @brief Forward declaration of the FIFLFSArchiveIterator class
 */
class FIFLFSArchiveIterator;

/**
 * @class FIFLFSFileIterator
 * @brief Forward declaration of the FIFLFSFileIterator template class
 */
template<bool is_nested>
class FIFLFSFileIterator;

/**
 * @struct FIFLFS
 * @brief Forward declaration of the FIFLFS template struct
 */
template<bool HasNested = false>
struct FIFLFS;

/**
 * @concept executable_buffer_path
 * @brief A concept requiring a callable that accepts a vector of characters and
 * a string
 */
template<typename lambdaT>
concept executable_buffer_path
  = std::invocable<lambdaT, std::vector<char>, std::string>;

/**
 * @concept executable_buffer_path_fi
 * @brief A concept requiring a callable that accepts a vector of characters, a
 * string, and an FI instance
 */
template<typename lambdaT>
concept executable_buffer_path_fi
  = std::invocable<lambdaT, std::vector<char>, std::string, FI>;

/**
 * @concept executable_fiflfs_sans_nested
 * @brief A concept requiring a callable that accepts an FIFLFS<false> instance
 */
template<typename lambdaT>
concept executable_fiflfs_sans_nested = std::invocable<lambdaT, FIFLFS<false>>;

/**
 * @concept executable_common_sans_nested
 * @brief A concept requiring a callable that meets either the
 * executable_buffer_path or executable_buffer_path_fi concepts
 */
template<typename lambdaT>
concept executable_common_sans_nested
  = executable_buffer_path<lambdaT> || executable_buffer_path_fi<lambdaT>;

/**
 * @concept filter_paths
 * @brief A concept requiring a callable that accepts a string and returns a
 * bool
 */
template<typename lambdaT>
concept filter_paths = std::is_invocable_r_v<bool, lambdaT, std::string>;

/**
 * @concept path_or_range
 * @brief A concept requiring a type that is either convertible to a filesystem
 * path or a contiguous range
 */
template<typename srcT>
concept path_or_range = std::convertible_to<srcT, std::filesystem::path>
                     || std::ranges::contiguous_range<srcT>;

/**
 * @concept executable_on_pair_of_int_string
 * @brief A concept requiring a callable that accepts a pair of unsigned int and
 * string
 */
template<typename lambdaT>
concept executable_on_pair_of_int_string
  = std::invocable<lambdaT, std::pair<unsigned int, std::string>>;
/**
 * @class FIFLFSBase
 * @brief A class to manage grouped file data, file information, and file paths.
 *
 * The class provides methods for loading and managing components of the
 * archive. It also provides various utility functions to extract data from the
 * archive components.
 */
class FIFLFSBase
{
protected:
  //! Grouping of file information data
  Grouping<std::vector<char>> m_fi{};
  //! Grouping of binary file data
  Grouping<std::vector<char>> m_fs{};
  //! Grouping of file paths
  Grouping<std::string>       m_fl{};
  //! File count
  std::size_t                 m_count{};

  /**
   * @brief Calculate and set the file count based on file information size.
   */
  void
    get_count() noexcept
  {
    m_count = FI::get_count(m_fi.size());
  }
  //! Default filter lambda function for file operations
  static constexpr auto default_filter_lambda = [](auto &&) {
    return true;
  };

public:
  // Default constructor
  FIFLFSBase() = default;

  /**
   * @brief Constructor that initializes FIFLFSBase from a source path.
   * @param src Source path of the files to be managed.
   */
  FIFLFSBase(std::filesystem::path src)
  {
    assert(src.has_parent_path());
    assert(src.has_stem());
    const auto parent_path = src.parent_path();
    const auto stem        = src.stem().string();
    open_viii::tools::execute_on_directory(
      parent_path,
      { stem },
      { FI::EXT, FS::EXT, fl::EXT },
      [this](const std::filesystem::path &path) {
        std::string     cur_path = path.string();
        std::error_code ec{};
        const auto      count
          = static_cast<std::uint32_t>(std::filesystem::file_size(path, ec));
        if (ec) {
          std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                    << ec.value() << ": " << ec.message() << ec.value()
                    << " - path: " << path << std::endl;
          ec.clear();
        }
        const TryAddT result = try_add(FI(count, 0U), path, path);
        switch (result) {
        case TryAddT::added_to_archive:
          // std::cout << "added: " << path << '\n';
          break;
        case TryAddT::archive_full:
          // std::cout << "added: " << path << '\n';
          // std::cout << "Archive Loaded\n";
          break;
        case TryAddT::not_part_of_archive:
          // std::cout << "not added: " << path << '\n';
          break;
        }
      });
  }
  /**
   * @brief Attempts to add an archive component based on its file extension.
   *
   * This function checks the file extension of the provided child_path and
   * tries to add it to the appropriate component (fi, fl, or fs) of the
   * archive.
   *
   * @tparam fiT A template parameter representing the FI_Like type.
   * @param fi_like A reference to an object of type fiT.
   * @param parent_path A const reference to a std::filesystem::path
   * representing the parent path.
   * @param child_path A const reference to a std::filesystem::path representing
   * the child path.
   * @return A TryAddT enum value indicating the result of the operation.
   */
  template<FI_Like fiT>
  [[nodiscard]] TryAddT
    try_add(
      fiT                          fi_like,
      const std::filesystem::path &parent_path,
      const std::filesystem::path &child_path)
  {
    switch (check_extension(child_path)) {
    case fiflfsT::none:
      return TryAddT::not_part_of_archive;
    case fiflfsT::fi:
      m_fi = decltype(m_fi)(fi_like, parent_path, child_path);
      get_count();
      break;
    case fiflfsT::fl:
      m_fl = decltype(m_fl)(fi_like, parent_path, child_path);
      break;
    case fiflfsT::fs:
      m_fs = decltype(m_fs)(fi_like, parent_path, child_path);
      break;
    }
    return all_set() ? TryAddT::archive_full : TryAddT::added_to_archive;
  }
  /**
   * @brief Checks the file extension of the provided path and returns the
   * corresponding fiflfsT type.
   *
   * This function analyzes the file extension of the given path and returns the
   * corresponding enum value from the fiflfsT enumeration.
   *
   * @param path A const reference to a std::filesystem::path object.
   * @return A fiflfsT enum value indicating the type of the archive component.
   */
  fiflfsT static check_extension(const std::filesystem::path &path)
  {
    return static_cast<fiflfsT>(tools::i_ends_with_any_get_offset(
      path.string(),
      std::array{ open_viii::archive::fl::EXT,
                  open_viii::archive::FS::EXT,
                  open_viii::archive::FI::EXT }));
  }
  /**
   * @brief Get the file count.
   * @return The file count as a std::size_t.
   */
  [[maybe_unused]] [[nodiscard]] constexpr std::size_t
    count() const noexcept
  {
    return m_count;
  }

  /**
   * @brief Get a reference to the file information grouping.
   * @return A constant reference to the Grouping of file information data.
   */
  [[maybe_unused]] [[nodiscard]] const Grouping<std::vector<char>> &
    fi() const noexcept
  {
    return m_fi;
  }

  /**
   * @brief Get a reference to the binary file data grouping.
   * @return A constant reference to the Grouping of binary file data.
   */
  [[maybe_unused]] [[nodiscard]] const Grouping<std::vector<char>> &
    fs() const noexcept
  {
    return m_fs;
  }

  /**
   * @brief Get a reference to the file paths grouping.
   * @return A constant reference to the Grouping of file paths.
   */
  [[maybe_unused]] [[nodiscard]] const Grouping<std::string> &
    fl() const noexcept
  {
    return m_fl;
  }

  /**
   * @brief Check if all the groupings (file information, binary file data, and
   * file paths) are set.
   * @return True if all groupings are set, false otherwise.
   */
  [[nodiscard]] bool
    all_set() const noexcept
  {
    return m_fi && m_fs && m_fl;
  }

  /**
   * @brief Explicit conversion operator to check if all groupings are set.
   * @return True if all groupings are set, false otherwise.
   */
  explicit operator bool() const noexcept
  {
    return all_set();
  }
  /**
   * @brief Retrieve a file entry by its index.
   *
   * This function retrieves a file entry from the archive, given its index.
   * If the file information is stored in memory, it will be read from there.
   * Otherwise, it will be read from the file.
   *
   * @tparam T An unsigned integral type representing the index.
   * @param id The index of the file entry to retrieve.
   * @return An archive::FI object representing the file entry. If the index is
   * out of bounds, an empty object will be returned.
   */
  template<std::unsigned_integral T>
  [[nodiscard]] archive::FI
    get_entry_by_index(const T id) const
  {
    if (m_count == 0 || id < m_count) {
      if (!std::ranges::empty(m_fi.data())) {
        return FI(m_fi.data(), id, m_fi.offset());
      }
      return FI(m_fi.path(), id, m_fi.offset());
    }
    return {};
  }
  /**
   * Try and Add Nested data. There are 3 parts to an archive so it this
   adds
   * one and goes to next one till all 3 are added.
   * @tparam srcT Can be an container of data, or it will be a filesystem
   path
   * to a file with the data.
   * @tparam datT Data struct with offset, size and compression type.
   * @param src The data or path.
   * @param srcOffset Bytes skipped at the beginning of data or file.
   * @param fileEntry
   * @param fi The Data struct.
   * @return char of 0,1,2, 0 not part of archive, 1 part added, 2 being
   archive
   * full.;
   */
  template<path_or_range srcT, FI_Like datT = archive::FI>
  TryAddT
    try_add_nested(
      const srcT                  &src,
      const size_t                 src_offset,
      const std::filesystem::path &file_entry,
      const datT                  &fi)
  {
    const fiflfsT i = check_extension(file_entry);
    switch (i) {
    case fiflfsT::none:
      return TryAddT::not_part_of_archive;
    case fiflfsT::fl: {
      m_fl = decltype(m_fl)(
        fl::clean_buffer(FS::get_entry<std::string>(src, fi, src_offset)),
        "",
        file_entry);
      break;
    }
    case fiflfsT::fs: {
      m_fs = decltype(m_fs)(FS::get_entry(src, fi, src_offset), "", file_entry);
      break;
    }
    case fiflfsT::fi: {
      m_fi = decltype(m_fi)(FS::get_entry(src, fi, src_offset), "", file_entry);
      get_count();
      break;
    }
    }
    compare_base_names();
    return all_set() ? TryAddT::archive_full : TryAddT::added_to_archive;
  }
  /**
   * @brief Retrieve the file entry data as a buffer.
   *
   * This function retrieves the file entry data from the archive, given its
   * file information. If the file data is stored in memory, it will be read
   * from there. Otherwise, it will be read from the file.
   *
   * @tparam dstT The type of the buffer to store the data (defaults to
   * std::vector<char>).
   * @tparam fiT The type of the file information object (must be FI_Like).
   * @param fi The file information object containing the entry's details.
   * @return A buffer (dstT) containing the entry data.
   */
  template<std::ranges::contiguous_range dstT = std::vector<char>, FI_Like fiT>
  dstT
    get_entry_buffer(const fiT &fi) const
  {
    if (!std::ranges::empty(m_fs.data())) {
      return FS::get_entry<dstT>(m_fs.data(), fi, m_fs.offset());
    }
    else {
      return FS::get_entry<dstT>(m_fs.path(), fi, m_fs.offset());
    }
  }
  /**
   * @brief Compare base names of the archive components and exit if they don't
   * match.
   *
   * This function checks if the base names of the file information (FI),
   * file data (FS), and file paths (FL) match. If they don't match, the program
   * will print an error message and exit.
   */
  void
    compare_base_names() const
  {
    if (
      (m_fl.base() == m_fs.base() && m_fi.base() == m_fs.base())
      || std::empty(m_fl.base()) || std::empty(m_fi.base())
      || std::empty(m_fs.base())) {
      return;
    }
    if (m_fl.base() != m_fs.base() || m_fi.base() != m_fs.base()) {
      if (m_fl.base() == m_fi.base()) {
        std::cerr << "base name mismatch FS Data: " << m_fs.path() << '\n';
        std::cerr << "not matching:" << m_fl.path() << ", " << m_fi.path()
                  << '\n';
        // m_fs = {};
        exit(EXIT_FAILURE);
      }
    }
    else if (m_fi.base() != m_fl.base()) {
      if (m_fl.base() == m_fs.base()) {
        std::cerr << "base name mismatch FL Data: " << m_fl.path() << "\n";
        std::cerr << "not matching:" << m_fl.base() << ", " << m_fs.base()
                  << '\n';
        // m_fi = {};
        exit(EXIT_FAILURE);
      }
      else if (m_fi.base() == m_fs.base()) {
        std::cerr << "base name mismatch FI Data: " << m_fi.path() << "\n";
        std::cerr << "not matching:" << m_fi.path() << ", " << m_fs.path()
                  << '\n';
        // m_fl = {};
        exit(EXIT_FAILURE);
      }
    }
    else {
      std::cerr << "No basename matched!\n";
      exit(EXIT_FAILURE);
    }
  }
  /**
   * @brief Get the base name of the archive.
   *
   * This function retrieves the base name of the archive by checking
   * the base names of the file information (FI), file data (FS), and
   * file paths (FL).
   *
   * @return A std::string_view representing the base name of the archive. If no
   * base name is found, an empty view is returned.
   */
  [[nodiscard]] std::string_view
    get_base_name() const
  {
    for (const auto &path : { std::string_view{ m_fi.base() },
                              std::string_view{ m_fl.base() },
                              std::string_view{ m_fs.base() } }) {
      if (!std::ranges::empty(path)) {
        return path;
      }
    }
    return {};
  }
  /**
   * @brief Retrieve the file entry data given its file information.
   *
   * This function retrieves the file entry data from the archive, given its
   * file information. If the file data is stored in memory, it will be read
   * from there. Otherwise, it will be read from the file.
   *
   * @tparam outT The type of the buffer to store the data (defaults to
   * std::vector<char>).
   * @tparam fiT The type of the file information object (must be FI_Like).
   * @param fi The file information object containing the entry's details.
   * @return A buffer (outT) containing the entry data.
   */
  template<std::ranges::contiguous_range outT = std::vector<char>, FI_Like fiT>
  [[nodiscard]] outT
    get_entry_data(const fiT &fi) const
  {
    return [this, &fi]() {
      if (std::ranges::empty(m_fs.data())) {
        return open_viii::archive::FS::get_entry<outT>(
          m_fs.path(),
          fi,
          m_fs.offset());
      }
      return open_viii::archive::FS::get_entry<outT>(
        m_fs.data(),
        fi,
        m_fs.offset());
    }();
  }
  /**
   * @brief Get the entry ID and full path given a filename.
   *
   * This function searches the archive for the specified filename and returns
   * the entry ID and the full path of the file.
   *
   * @param filename A string_view of the filename to search for.
   * @return A pair containing the entry ID and the full path of the file.
   */
  [[nodiscard]] auto
    get_entry_id_and_path(const std::string_view &filename) const
  {
    if (std::ranges::empty(m_fl.data())) {
      return open_viii::archive::fl::get_entry(
        m_fl.path(),
        { filename },
        m_fl.offset(),
        m_fl.size(),
        m_count);
    }
    return open_viii::archive::fl::get_entry(
      m_fl.path(),
      m_fl.data(),
      { filename },
      m_fl.offset(),
      m_fl.size(),
      m_count);
  }
  /**
   * @brief Get the full path of a file given its filename.
   *
   * This function searches the archive for the specified filename and returns
   * the full path of the file.
   *
   * @param filename A string_view of the filename to search for.
   * @return A string representing the full path of the file.
   */
  [[nodiscard]] std::string
    get_full_path(const std::string_view &filename) const
  {
    return get_entry_id_and_path(filename).second;
  }
  /**
   * @brief Retrieve the file entry data given its filename.
   *
   * This function searches the archive for the specified filename and retrieves
   * the file entry data.
   *
   * @tparam outT The type of the buffer to store the data (defaults to
   * std::vector<char>).
   * @param filename A string_view of the filename to search for.
   * @return A buffer (outT) containing the entry data.
   */
  template<std::ranges::contiguous_range outT = std::vector<char>>
  [[nodiscard]] outT
    get_entry_data(const std::string_view &filename) const
  {
    const auto &[id, path] = get_entry_id_and_path(filename);
    return get_entry_data<outT>(get_entry_by_index(id));
  }
  /**
   * @brief Retrieve the file entry data given a list of filenames.
   *
   * This function searches the archive for the first matching filename from the
   * provided list and retrieves the file entry data.
   *
   * @tparam outT The type of the buffer to store the data (defaults to
   * std::vector<char>).
   * @param filenames An initializer_list of string_views of the filenames to
   * search for.
   * @param[out] out_name Pointer to a string to store the full path of the
   * found file (optional).
   * @param[out] out_name_pos Pointer to a size_t to store the position of the
   * found filename in the list (optional).
   * @return A buffer (outT) containing the entry data.
   */
  template<std::ranges::contiguous_range outT = std::vector<char>>
  [[nodiscard]] outT
    get_entry_data(
      const std::initializer_list<std::string_view> &filenames,
      std::string *const                             out_name = nullptr,
      std::size_t *const out_name_pos                         = nullptr) const
  {
    if (out_name_pos != nullptr) {
      *out_name_pos = 0U;
    }
    using pair = decltype(get_entry_id_and_path(""));
    pair       tmp{};
    const auto it
      = std::ranges::find_if(filenames, [&tmp, this](std::string_view str) {
          tmp = get_entry_id_and_path(str);
          return tmp != pair{};
        });
    if (it != filenames.end()) {
      if (out_name_pos != nullptr) {
        *out_name_pos
          = static_cast<std::size_t>(std::distance(filenames.begin(), it));
      }
      const auto &[id, path] = tmp;
      if (out_name != nullptr) {
        *out_name = path;
      }
      return get_entry_data<outT>(get_entry_by_index(id));
    }
    return {};
  }
  /**
   * @brief Get a vector of pairs containing entry IDs and file paths.
   *
   * This function searches the archive for the specified filenames and returns
   * a vector of pairs containing the entry IDs and the full paths of the files.
   *
   * @param filename An initializer_list of string_views of the filenames to
   * search for.
   * @return A vector of pairs with entry IDs and file paths.
   */
  [[nodiscard]] std::vector<std::pair<unsigned int, std::string>>
    get_vector_of_indexes_and_files(
      const std::initializer_list<std::string_view> &filename) const
  {
    return archive::fl::get_all_entries(
      m_fl.path(),
      m_fl.data(),
      m_fl.offset(),
      m_fl.size(),
      m_count,
      filename);
  }
  /**
   * @brief Get a lambda function for filling an archive.
   *
   * This function returns a lambda function that takes an entry and fills the
   * given archive based on the entry's data.
   *
   * @param[out] archive A reference to an FIFLFSBase object to fill with data.
   * @return A lambda function for filling the archive.
   */
  auto
    fill_archive_lambda(FIFLFSBase &archive) const
  {
    return [this, &archive](const auto &item) -> bool {
      const auto &[id, strVirtualPath] = item;
      const FI_Like auto    fi         = get_entry_by_index(id);
      std::filesystem::path virtualPath(strVirtualPath);
      const TryAddT         retVal = [&]() {
        if (fi.compression_type() == CompressionTypeT::none) {
          return archive.try_add(
            FileData(
              static_cast<unsigned long>(m_fs.offset() + fi.offset()),
              fi.uncompressed_size()),
            m_fs.path(),
            virtualPath);
        }
        return archive
          .try_add_nested(m_fs.path(), m_fs.offset(), virtualPath, fi);
      }();
      return retVal == TryAddT::archive_full;
    };
  }
  /**
   * @brief Get all file paths from the archive.
   *
   * This function retrieves all file paths from the archive, filtered by the
   * specified list of filenames.
   *
   * @param filename An initializer_list of string_views of the filenames to
   * filter.
   * @return A vector of strings containing the file paths.
   */
  std::vector<std::string>
    get_all_paths_from_fl(
      const std::initializer_list<std::string_view> &filename) const
  {
    return archive::fl::get_all_entry_strings(
      m_fl.path(),
      m_fl.data(),
      m_fl.offset(),
      m_fl.size(),
      m_count,
      filename);
  }
  /**
   * @brief Get all pairs of entry IDs and file paths from the archive.
   *
   * This function retrieves all pairs of entry IDs and file paths from the
   * archive, filtered by the specified list of filenames.
   *
   * @param filename An initializer_list of string_views of the filenames to
   * filter.
   * @return A vector of pairs containing entry IDs and file paths.
   */
  std::vector<std::pair<std::uint32_t, std::string>>
    get_all_pairs_from_fl(
      const std::initializer_list<std::string_view> &filename) const
  {
    return archive::fl::get_all_entries(
      m_fl.path(),
      m_fl.data(),
      m_fl.offset(),
      m_fl.size(),
      m_count,
      filename);
  }
};
/**
 * @brief A class representing a FIFLFS archive.
 *
 * This class provides methods to access and manipulate FIFLFS archives, with
 * support for both nested and non-nested archives.
 *
 * @tparam HasNested A boolean flag indicating whether the archive has nested
 * archives.
 */
template<bool HasNested>
struct FIFLFS : public FIFLFSBase
{
  /// Iterator type for traversing the entries in the archive.
  using iterator = std::conditional_t<
    HasNested,
    FIFLFSArchiveIterator,
    FIFLFSFileIterator<HasNested>>;

  /// Default constructor
  FIFLFS() = default;

  /// Construct a FIFLFS object from a std::filesystem::path object.
  FIFLFS(std::filesystem::path src) : FIFLFSBase(std::move(src)) {}
  /// Construct a FIFLFS object from a FIFLFSBase object.
  FIFLFS(FIFLFSBase src) : FIFLFSBase(std::move(src)) {}

  /**
   * @brief Retrieve an empty list of map data for non-nested archives.
   *
   * This function is only enabled for non-nested archives, and returns an
   * empty list of map data, as map data is not relevant for non-nested
   * archives.
   *
   * @return An empty std::vector of std::string.
   */
  std::vector<std::string>
    map_data_from_maplist() const
    requires(!HasNested)
  {
    return {};
  }

  /**
   * @brief Retrieve a list of map data for nested archives.
   *
   * This function is only enabled for nested archives, and returns a list of
   * map data extracted from the nested "mapdata" archive.
   *
   * @return A std::vector of std::string containing the map data.
   */
  std::vector<std::string>
    map_data_from_maplist() const
    requires(HasNested)
  {
    std::vector<std::string> list{};
    list.reserve(count());
    FIFLFS<false> archive = get_archive_with_nested({ "mapdata" });
    if (!archive) {
      return list;
    }
    const auto raw_list
      = archive.template get_entry_data<std::string>("maplist");
    std::stringstream ss{ raw_list };
    std::string       tmp{};
    while (std::getline(ss, tmp)) {
      while (!tmp.empty() && tmp.back() == '\r') {
        tmp.pop_back();
      }
      if (!tmp.empty()) {
        list.emplace_back(std::move(tmp));
      }
    }
    return list;
  }
  /**
   * @brief Retrieve an empty list of map data for non-nested archives.
   *
   * This function is only enabled for non-nested archives, and returns an
   * empty list of map data, as map data is not relevant for non-nested
   * archives.
   *
   * @return An empty std::vector of std::string.
   */
  std::vector<std::string>
    map_data() const
    requires(!HasNested)
  {
    return {};
  }
  /**
   * @brief Retrieve a list of map data for nested archives.
   *
   * This function is only enabled for nested archives, and returns a list of
   * map data by retrieving all file paths with a ".fs" extension from the
   * archive.
   *
   * @return A std::vector of std::string containing the map data.
   */
  std::vector<std::string>
    map_data() const
    requires(HasNested)
  {
    std::vector<std::string> list{};
    const auto               items = get_all_paths_from_fl({ ".fs" });
    list.reserve(items.size());
    std::ignore = std::ranges::transform(
      items,
      std::back_inserter(list),
      [](const auto &item) {
        const auto fspath = std::filesystem::path(item);
        assert(fspath.has_stem());
        return fspath.filename().stem().string();
      });
    std::ranges::sort(list);
    return list;
  }
  /**
   * @brief Iterate through a list of results and apply a processing function.
   *
   * This function iterates through the given list of results (pairs of unsigned
   * int and std::string) and applies the provided processing function (lambda)
   * to each result.
   *
   * @param results A std::vector of std::pair<unsigned int, std::string>
   * representing the results.
   * @param process A lambda function to be applied to each result.
   */
  template<class lambdaT>
  void
    for_each_sans_nested(
      const std::vector<std::pair<unsigned int, std::string>> &results,
      lambdaT                                                &&process) const;
  /**
   * @brief Execute a given lambda function on nested archives.
   *
   * This function is only enabled for nested archives, and executes the
   * provided lambda function on the specified nested archives, filtered by the
   * provided filter lambda function.
   *
   * @param filename A std::initializer_list of std::string_view representing
   * the filenames to search.
   * @param lambda The lambda function to be executed on the nested archives.
   * @param nested_filename A std::initializer_list of std::string_view
   * representing the nested filenames (default: empty).
   * @param filter_lambda A filter lambda function for filtering the results
   * (default: no filter).
   * @param limit_once A boolean indicating whether to limit the execution to
   * only one matching item (default: false).
   */
  template<
    executable_common_sans_nested lambdaT,
    filter_paths                  filterT = decltype(default_filter_lambda)>
    requires(HasNested)
  void
    execute_with_nested(
      const std::initializer_list<std::string_view> &filename,
      lambdaT                                      &&lambda,
      const std::initializer_list<std::string_view> &nested_filename = {},
      filterT                                      &&filter_lambda   = {},
      bool                                           limit_once = false) const
  {
    FIFLFS<false> archive = {};
    const auto    items   = get_all_pairs_from_fl(filename);
    const auto    pFunction
      = [&lambda, &nested_filename, &archive, this, &filter_lambda](
          const auto &item) -> bool {
      if (filter_lambda(item.second) && fill_archive_lambda(archive)(item)) {
        archive.execute_on(nested_filename, lambda, filter_lambda);
        archive = {};
        return true;
      }
      return false;
    };
    if (!limit_once)
      std::ranges::for_each(items, pFunction);
    else {
      std::ignore = std::ranges::any_of(items, pFunction);
    }
  }
  /**
   * @brief Execute a given lambda function on nested archives.
   *
   * This function is only enabled for nested archives, and executes the
   * provided lambda function on the specified nested archives, filtered by the
   * provided filter lambda function.
   *
   * @tparam lambdaT The type of the lambda function to be executed on the
   * nested archives.
   * @tparam filterT The type of the filter lambda function for filtering the
   * results (default: decltype(default_filter_lambda)).
   * @param filename A std::initializer_list of std::string_view representing
   * the filenames to search.
   * @param lambda The lambda function to be executed on the nested archives.
   * @param nested_filename A std::initializer_list of std::string_view
   * representing the nested filenames (default: empty).
   * @param filter_lambda A filter lambda function for filtering the results
   * (default: no filter).
   * @param limit_once A boolean indicating whether to limit the execution to
   * only one matching item (default: false).
   */
  template<
    executable_fiflfs_sans_nested lambdaT,
    filter_paths                  filterT = decltype(default_filter_lambda)>
    requires(HasNested)
  void
    execute_with_nested(
      const std::initializer_list<std::string_view> &filename,
      lambdaT                                      &&lambda,
      [[maybe_unused]] const std::initializer_list<std::string_view>
        &nested_filename
      = {},
      filterT &&filter_lambda = {},
      bool      limit_once    = false) const
  {
    FIFLFS<false> archive = {};
    const auto    items   = get_all_pairs_from_fl(filename);
    const auto    pFunction
      = [&lambda, &archive, this, &filter_lambda](const auto &item) -> bool {
      if (filter_lambda(item.second) && fill_archive_lambda(archive)(item)) {
        lambda(std::move(archive));
        archive = {};
        return true;
      }
      return false;
    };
    if (!limit_once)
      std::ranges::for_each(items, pFunction);
    else {
      std::ignore = std::ranges::any_of(items, pFunction);
    }
  }

  /**
   * @brief No-op function for non-nested archives.
   *
   * This function does nothing and is only enabled for non-nested archives. It
   * serves as a placeholder for the execute_with_nested() function to ensure
   * compatibility with non-nested archives.
   *
   * @tparam lambdaT The type of the lambda function (unused).
   * @tparam filterT The type of the filter lambda function (unused, default:
   * decltype(default_filter_lambda)).
   * @param filename A std::initializer_list of std::string_view representing
   * the filenames (unused).
   * @param lambda A lambda function (unused).
   * @param nested_filename A std::initializer_list of std::string_view
   * representing the nested filenames (unused, default: empty).
   * @param filter_lambda A filter lambda function (unused, default: no filter).
   * @param limit_once A boolean indicating whether to limit the execution
   * (unused, default: false).
   */
  template<typename lambdaT, typename filterT = decltype(default_filter_lambda)>
    requires(!HasNested)
  void
    execute_with_nested(
      [[maybe_unused]] const std::initializer_list<std::string_view> &filename,
      [[maybe_unused]] lambdaT                                      &&lambda,
      [[maybe_unused]] const std::initializer_list<std::string_view>
        &nested_filename
      = {},
      [[maybe_unused]] filterT &&filter_lambda = {},
      [[maybe_unused]] bool      limit_once    = false) const
  {
  }
  /**
   * @brief Get an archive with nested files.
   *
   * This function searches for a nested archive that matches the provided
   * filename(s) and filter lambda function.
   *
   * @tparam filterT The type of the filter lambda function (default:
   * decltype(default_filter_lambda)).
   * @param filename A std::initializer_list of std::string_view representing
   * the filenames to search.
   * @param filter_lambda A filter lambda function for filtering the results
   * (default: no filter).
   * @return An FIFLFS<false> archive instance with the nested files.
   */
  template<filter_paths filterT = decltype(default_filter_lambda)>
  auto
    get_archive_with_nested(
      const std::initializer_list<std::string_view> &filename,
      filterT &&filter_lambda = {}) -> FIFLFS<false> const
  {
    FIFLFS<false> archive = {};
    const auto    items   = get_all_pairs_from_fl(filename);
    std::ignore           = std::ranges::any_of(items, [&](const auto &item) {
      return filter_lambda(item.second) && fill_archive_lambda(archive)(item);
    });
    return archive;
  }
  /**
   * @brief Execute a given lambda function on the specified files.
   *
   * This function executes the provided lambda function on the specified files,
   * filtered by the provided filter lambda function.
   *
   * @tparam lambdaT The type of the lambda function to be executed on the
   * specified files.
   * @tparam filterT The type of the filter lambda function for filtering the
   * results (default: decltype(default_filter_lambda)).
   * @param filename A std::initializer_list of std::string_view representing
   * the filenames to search.
   * @param lambda The lambda function to be executed on the specified files.
   * @param filter_lambda A filter lambda function for filtering the results
   * (default: no filter).
   */
  template<
    executable_common_sans_nested lambdaT,
    filter_paths                  filterT = decltype(default_filter_lambda)>
  void
    execute_on(
      const std::initializer_list<std::string_view> &filename,
      lambdaT                                      &&lambda,
      filterT                                      &&filter_lambda = {}) const
  {
    const auto results = get_vector_of_indexes_and_files(filename);
    const auto process = [this, &lambda, &filter_lambda](
                           const std::pair<unsigned int, std::string> &pair) {
      if (filter_lambda(pair.second)) {
        auto fi = get_entry_by_index(pair.first);
        if constexpr (executable_buffer_path<lambdaT>) {

          lambda(get_entry_buffer(fi), pair.second);
        }
        else if constexpr (executable_buffer_path_fi<lambdaT>) {
          lambda(get_entry_buffer(fi), pair.second, fi);
        }
      }
    };
    for_each_sans_nested(results, process);
  }
  /**
   * @brief Execute a given lambda function on each element of the results vector.
   *
   * This function executes the provided lambda function on each element of the results vector
   * for the non-nested case (requires(!HasNested)).
   *
   * @tparam lambdaT The type of the lambda function to be executed on each element of the results vector.
   * @param results A vector of std::pair<unsigned int, std::string> to process.
   * @param process The lambda function to be executed on each element of the results vector.
   */
  template<executable_on_pair_of_int_string lambdaT>
    requires(!HasNested)
  void
    for_each_sans_nested(
      const std::vector<std::pair<unsigned int, std::string>> &results,
      lambdaT                                                &&process) const
  {
    std::ranges::for_each(results, process);
  }
  /**
   * @brief Execute a given lambda function on each element of the results vector.
   *
   * This function executes the provided lambda function on each element of the results vector
   * for the nested case (requires(HasNested)). It filters out nested archives before processing.
   *
   * @tparam lambdaT The type of the lambda function to be executed on each element of the results vector.
   * @param results A vector of std::pair<unsigned int, std::string> to process.
   * @param process The lambda function to be executed on each element of the results vector.
   */
  template<executable_on_pair_of_int_string lambdaT>
    requires(HasNested)
  void
    for_each_sans_nested(
      const std::vector<std::pair<unsigned int, std::string>> &results,
      lambdaT                                                &&process) const
  {
    std::ranges::for_each(
      results
        | std::views::filter(
          [](const std::pair<unsigned int, std::string> &pair) -> bool {
            return check_extension(pair.second)
                == fiflfsT::none;// prevent from running on nested archives.
                                 // We have another function for those.
          }),
      process);
  }
  /**
   * @brief Get a range of the files in the archive.
   *
   * @return A subrange containing the files in the archive.
   */
  auto
    file_range() const
  {
    return std::ranges::subrange(
      FIFLFSFileIterator<HasNested>{ *this },
      cend());
  }
  /// Get an iterator pointing to the beginning of the entries in the archive.
  iterator
    begin() const;
  /// Get a const iterator pointing to the beginning of the entries in the
  /// archive.
  iterator
    cbegin() const;

  /// Get a sentinel representing the end of the entries in the archive.
  FIFLFS_sentinel
    end() const
  {
    return {};
  }
  /// Get a const sentinel representing the end of the entries in the archive.
  FIFLFS_sentinel
    cend() const
  {
    return {};
  }
};
static_assert(std::movable<FIFLFS<false>>);
static_assert(std::movable<FIFLFS<true>>);
static_assert(std::copyable<FIFLFS<false>>);
static_assert(std::copyable<FIFLFS<true>>);
}// namespace open_viii::archive
#include "FIFLFSArchiveIterator.hpp"
#include "FIFLFSFileIterator.hpp"
#endif// !VIIIARCHIVE_FIFLFS_HPP
