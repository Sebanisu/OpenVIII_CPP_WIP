//
// Created by pcvii on 3/7/2023.
//

#ifndef OPENVIII_CPP_WIP_FIFLFS_OPERATIONS_HPP
#define OPENVIII_CPP_WIP_FIFLFS_OPERATIONS_HPP
#include "FIFLFS.hpp"
namespace open_viii::archive
{
[[nodiscard]] inline TryAddT get_fiflfs(const FIFLFS<true>& source,
             FIFLFS<false>& archive,
             const std::uint32_t id,
             const std::string_view str_virtual_path)
{
  FI fi = source.get_entry_by_index(id);
  std::filesystem::path virtualPath(str_virtual_path);
  if (!std::ranges::empty(source.fs().data())) {
    return archive.try_add_nested(source.fs().data(),
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
    //   std::cout << virtualPath.filename() << " is uncompressed pointing at location in actual file!\n";
    // }
    return localRetVal;
  }
  return archive.try_add_nested(source.fs().path(),
                                source.fs().offset(),
                                virtualPath,
                                fi);
}

[[nodiscard]] inline FIFLFS<false>
  get_fiflfs(
    const FIFLFS<true>                            &source,
    const std::initializer_list<std::string_view> &filename)
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

[[nodiscard]] inline bool
  any_matches(
    const std::vector<std::filesystem::path> &paths,
    const std::filesystem::path              &in_path)
{
  return find_match(paths, in_path) != std::ranges::end(paths);
}

[[nodiscard]] inline bool
  any_matches(
    const std::vector<std::filesystem::path>                 &paths,
    const std::vector<std::pair<std::uint32_t, std::string>> &pairs)
{
  return std::ranges::any_of(pairs, [&paths](const auto &pair) {
    const auto in_path = std::filesystem::path(pair.second);
    return in_path.has_filename() && any_matches(paths, in_path);
  });
}
/**
 * create a copy of the archive with the files that have the same filename
 * replaced with a new one. rest are copied with no changes.
 * @param source original archive
 * @param paths paths to new files.
 * @return paths to new archive files.
 */
template<bool Nested = false>
[[nodiscard]] inline std::vector<std::filesystem::path>
  replace_files(
    const FIFLFS<Nested>                     &source,
    const std::vector<std::filesystem::path> &paths)
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
  // std::cout << "Destination FI: " << fi_name << '\n';
  // std::cout << "Destination FL: " << fl_name << '\n';
  // std::cout << "Destination FS: " << fs_name << '\n';
  if (fi_name.empty() || fl_name.empty() || fs_name.empty()) {
    return r;
  }
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
  std::ranges::for_each(pairs, [&](const auto &pair) {
    const auto &[i, in_path]         = pair;
    const FI               source_fi = source.get_entry_by_index(i);
    //    switch (source_fi.compression_type()) {
    //    case open_viii::CompressionTypeT::none:
    //      std::puts("\t Not Compressed");
    //      break;
    //    case open_viii::CompressionTypeT::lzss:
    //      std::puts("\t LZSS Compressed");
    //      break;
    //    case open_viii::CompressionTypeT::lz4:
    //      std::puts("\t LZ4 Compressed");
    //      break;
    //    }

    // match found use the new file
    open_viii::archive::FI fi = [&]([[maybe_unused]] const auto &in_path_ref) {
      if (auto match = find_match(paths, in_path_ref);
          match != std::ranges::end(paths)) {
        // std::cout << "Updated: " << pair.second << std::endl;
        return open_viii::archive::append_entry(
          fs_fs,
          *match,
          source_fi.compression_type());
      }
      return open_viii::archive::append_entry(
        fs_fs,
        source.get_entry_data(source_fi),
        // CompressionTypeT::none);
        source_fi.compression_type());
    }(in_path);
    open_viii::archive::append_entry(fs_fi, fi);
    open_viii::archive::append_entry(fs_fl, std::filesystem::path(in_path));
    //    std::puts("\t\tAdded File");
  });
  r.emplace_back(temp / fi_name);
  r.emplace_back(temp / fl_name);
  r.emplace_back(temp / fs_name);
  return r;
}
inline auto
  get_fiflfs_entries(
    const FIFLFS<true>                            &source,
    const std::initializer_list<std::string_view> &filename)
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
template<bool has_nested>
inline std::ostream &
  operator<<(std::ostream &os, const FIFLFS<has_nested> &data)
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



}
#endif// OPENVIII_CPP_WIP_FIFLFS_OPERATIONS_HPP
