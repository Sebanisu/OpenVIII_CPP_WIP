//
// Created by pcvii on 3/7/2023.
//

#ifndef OPENVIII_CPP_WIP_FIFLFS_OPERATIONS_HPP
#define OPENVIII_CPP_WIP_FIFLFS_OPERATIONS_HPP
#include "FIFLFS.hpp"
namespace open_viii::archive {
/**
 * @brief Tries to add an entry from the source FIFLFS to the archive FIFLFS.
 * @param source The source FIFLFS object.
 * @param archive The archive FIFLFS object.
 * @param id The index of the entry in the source.
 * @param str_virtual_path The virtual path of the entry in the archive.
 * @return The result of the addition attempt.
 */
[[nodiscard]] inline auto
  get_fiflfs(
    const FIFLFS<true>    &source,
    FIFLFS<false>         &archive,
    const std::uint32_t    id,
    const std::string_view str_virtual_path) -> TryAddT
{
  FI                    fi = source.get_entry_by_index(id);
  std::filesystem::path virtualPath(str_virtual_path);
  if (!std::ranges::empty(source.fs().data())) {
    return archive.try_add_nested(
      source.fs().data(),
      source.fs().offset(),
      virtualPath,
      fi);
  }
  if (fi.compression_type() == CompressionTypeT::none) {
    auto localRetVal = archive.try_add(
      FileData(source.fs().offset() + fi.offset(), fi.uncompressed_size()),
      source.fs().path(),
      virtualPath);
    // if (localRetVal != TryAddT::not_part_of_archive) {
    //   std::cout << virtualPath.filename() << " is uncompressed pointing at
    //   location in actual file!\n";
    // }
    return localRetVal;
  }
  return archive
    .try_add_nested(source.fs().path(), source.fs().offset(), virtualPath, fi);
}
/**
 * @brief Creates a FIFLFS object containing entries from the source FIFLFS that
 * match the specified filenames.
 * @param source The source FIFLFS object.
 * @param filename The list of filenames to search for in the source FIFLFS.
 * @return The created FIFLFS object containing the matched entries.
 */
[[nodiscard]] inline auto
  get_fiflfs(
    const FIFLFS<true>                            &source,
    const std::initializer_list<std::string_view> &filename) -> FIFLFS<false>
{
  FIFLFS<false> archive{};
  const auto    items = archive::fl::get_all_entries(
    source.fl().path(),
    source.fl().data(),
    source.fl().offset(),
    source.fl().size(),
    source.count(),
    filename);
  for (const auto &[id, strVirtualPath] : items) {
    TryAddT tryAddT = get_fiflfs(source, archive, id, strVirtualPath);
    if (tryAddT == TryAddT::archive_full) {
      break;
    }
  }
  return archive;
}
/**
 * @brief Finds a matching path in a vector of paths.
 * @param paths The vector of paths to search.
 * @param in_path The path to search for in the vector.
 * @return An iterator to the matching path if found; otherwise, an iterator to
 * the end of the vector.
 */
[[nodiscard]] inline auto
  find_match(
    const std::vector<std::filesystem::path> &paths,
    const std::filesystem::path              &in_path)
{
  return std::ranges::find_if(
    paths,
    [&in_path](const std::filesystem::path &new_path) {
      return new_path.has_filename()
          && in_path.filename() == new_path.filename();
    });
}
/**
 * @brief Checks if any path in a vector of paths matches a given path.
 *
 * @param paths Vector of paths to check.
 * @param in_path Path to search for in `paths`.
 * @return `true` if `in_path` matches any path in `paths`, `false` otherwise.
 */
[[nodiscard]] inline auto
  any_matches(
    const std::vector<std::filesystem::path> &paths,
    const std::filesystem::path              &in_path) -> bool
{
  return find_match(paths, in_path) != std::ranges::end(paths);
}

/**
 * @brief Checks if any path in a vector of paths matches any path in a vector
 * of pairs.
 *
 * @param paths Vector of paths to check.
 * @param pairs Vector of pairs where the second element is a path to search for
 * in `paths`.
 * @return `true` if any path in `pairs` matches any path in `paths`, `false`
 * otherwise.
 */
[[nodiscard]] inline auto
  any_matches(
    const std::vector<std::filesystem::path>                 &paths,
    const std::vector<std::pair<std::uint32_t, std::string>> &pairs) -> bool
{
  return std::ranges::any_of(pairs, [&paths](const auto &pair) {
    const auto in_path = std::filesystem::path(pair.second);
    return in_path.has_filename() && any_matches(paths, in_path);
  });
}

/**
 * @brief Create a copy of the archive with the files that have the same
 * filename replaced with a new one. Rest are copied with no changes.
 *
 * @tparam Nested flag indicating whether the archive is nested.
 * @param source original archive.
 * @param paths paths to new files.
 * @return Paths to new archive files.
 */
template<bool Nested = false>
[[nodiscard]] inline auto
  replace_files(
    const FIFLFS<Nested>                     &source,
    const std::vector<std::filesystem::path> &paths)
    -> std::vector<std::filesystem::path>
{

  std::vector<std::filesystem::path> r{};
  auto pairs = source.get_vector_of_indexes_and_files({});

  // make sure there is a match.
  if (!any_matches(paths, pairs)) {
    return r;
  }
  std::ranges::sort(pairs);
  const auto &fi_name = source.fi().nested_path_or_path().filename();
  const auto &fl_name = source.fl().nested_path_or_path().filename();
  const auto &fs_name = source.fs().nested_path_or_path().filename();

  // Check if any of the file names are empty
  if (fi_name.empty() || fl_name.empty() || fs_name.empty()) {
    return r;
  }

  // Create temporary output streams for each archive file
  const auto   &temp = std::filesystem::temp_directory_path();
  std::ofstream fs_fi(
    temp / fi_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::ofstream fs_fl(
    temp / fl_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::ofstream fs_fs(
    temp / fs_name,
    std::ios::out | std::ios::binary | std::ios::trunc);

  // Iterate through each pair of index and file in the source archive
  std::ranges::for_each(pairs, [&](const auto &pair) {
    const auto &[i, in_path]         = pair;
    const FI               source_fi = source.get_entry_by_index(i);

    // match found use the new file
    open_viii::archive::FI fi = [&]([[maybe_unused]] const auto &in_path_ref) {
      if (auto match = find_match(paths, in_path_ref);
          match != std::ranges::end(paths)) {
        return open_viii::archive::append_entry(
          fs_fs,
          *match,
          source_fi.compression_type());
      }
      return open_viii::archive::append_entry(
        fs_fs,
        source.get_entry_data(source_fi),
        source_fi.compression_type());
    }(in_path);

    // Append the new entry to the archive files
    open_viii::archive::append_entry(fs_fi, fi);
    open_viii::archive::append_entry(fs_fl, std::filesystem::path(in_path));
  });

  // Add the paths to the new archive files to the result vector
  r.emplace_back(temp / fi_name);
  r.emplace_back(temp / fl_name);
  r.emplace_back(temp / fs_name);

  return r;
}

/**
 * @brief Retrieves all FIFLFS entries matching the specified filenames from the
 * given source.
 *
 * This function searches for entries in the FIFLFS<true> source object, and
 * then creates a vector of FIFLFS<false> objects containing the found entries.
 * The entries are filtered based on the provided initializer list of filenames.
 *
 * @param source A const reference to a FIFLFS<true> object, which serves as the
 * source archive.
 * @param filename A const reference to an initializer list of std::string_view,
 * which contains the filenames to search for.
 * @return A std::vector of FIFLFS<false> objects containing the found entries.
 */
inline auto
  get_fiflfs_entries(
    const FIFLFS<true>                            &source,
    const std::initializer_list<std::string_view> &filename)
    -> std::vector<FIFLFS<false>>
{
  std::vector<FIFLFS<false>> out{};
  const auto                 items = archive::fl::get_all_entries(
    source.fl().path(),
    source.fl().data(),
    source.fl().offset(),
    source.fl().size(),
    source.count(),
    filename);
  for (const auto &[id, strVirtualPath] : items) {
    const TryAddT t
      = get_fiflfs(source, out.emplace_back(), id, strVirtualPath);
    if (t != TryAddT::archive_full) {
      out.pop_back();
    }
  }
  return out;
}
/**
 * @brief Overloads the ostream insertion operator for FIFLFS objects.
 *
 * This function provides a way to output the FIFLFS object's contents to an
 * ostream in a human-readable format. The output will include the base name,
 * number of file entries, and the paths of the fi, fl, and fs components.
 *
 * @tparam has_nested A bool template parameter indicating whether the FIFLFS
 * object has nested data.
 * @param os A reference to an std::ostream object where the output will be
 * sent.
 * @param data A const reference to the FIFLFS object whose data will be output.
 * @return A reference to the same std::ostream object to allow for chained
 * insertion operations.
 */
template<bool has_nested>
inline auto
  operator<<(std::ostream &os, const FIFLFS<has_nested> &data) -> std::ostream &
{
  const auto str = [](auto fiflfs) {
    return std::ranges::empty(fiflfs.nested_path())
           ? fiflfs.path()
           : fiflfs.path() / fiflfs.nested_path();
  };
  return os << '{' << data.get_base_name() << " {" << data.count()
            << " File Entries from : " << str(data.fi()) << ", "
            << str(data.fl()) << ", " << str(data.fs()) << "}}";
}

}// namespace open_viii::archive
#endif// OPENVIII_CPP_WIP_FIFLFS_OPERATIONS_HPP
