//
// Created by pcvii on 9/21/2020.
//

#ifndef VIIIARCHIVE_RESWIZZLETREE_HPP
#define VIIIARCHIVE_RESWIZZLETREE_HPP
#include <utility>

#include "open_viii/archive/FIFLFS.hpp"
#include "Map.hpp"
#include "Mim.hpp"
namespace open_viii::graphics::background {
struct ReswizzleTree
{
private:
  const std::filesystem::path m_dir_path{};
  const std::string m_dir_name{};
  const std::string m_fi_filename{};
  const std::string m_fl_filename{};
  const std::string m_fs_filename{};
  const std::string m_map_filename{};
  const std::string m_mim_filename{};
  const std::string m_output_prefix{};
  const open_viii::archive::FIFLFS<false> m_archive{};
  const MimType m_mim_type{};

  [[nodiscard]] std::string get_path_with_ext(std::string_view extension) const
  {
    return "\\" + m_dir_name + std::string(extension);
    // the FL files contain \\ I change to / in post for easy dumping to linux and support of filesystem.
  }
  [[nodiscard]] MimType get_mim_type() const
  {
    if(static_cast<bool>(m_archive)) {
      auto mims = m_archive.get_vector_of_indexs_and_files({ m_mim_filename });
      if (std::ranges::empty(mims)) {
        return {};// no mim file.
      }
      open_viii::FI_Like auto mim_fi = m_archive.get_entry_by_index(mims[0].first);
      return Mim::get_texture_type(mim_fi.uncompressed_size(), m_dir_name);
    }
    return {};
  }

public:
  ReswizzleTree(const open_viii::archive::FIFLFS<true> &field, const std::filesystem::path &dir_path)
    : m_dir_path(dir_path), m_dir_name(dir_path.filename().string()),
      m_fi_filename(get_path_with_ext(archive::FI::EXT)), m_fl_filename(get_path_with_ext(open_viii::archive::FL::EXT)),
      m_fs_filename(get_path_with_ext(open_viii::archive::FS::EXT)),
      m_map_filename(get_path_with_ext(open_viii::graphics::background::Map<>::EXT)),
      m_mim_filename(get_path_with_ext(open_viii::graphics::background::Mim::EXT)),
      m_output_prefix((m_dir_path / m_dir_name).string()),
      m_archive(field.get_fiflfs({ m_fi_filename, m_fl_filename, m_fs_filename })),
      m_mim_type(get_mim_type())
  {}
  /**
   * true if archive is set and full. false if invalid.
   * @return bool
   */
  [[nodiscard]] explicit operator bool() const noexcept { return static_cast<bool>(m_archive); }

  /**
   * Point to members of the archive
   * @return FIFLFS*
   */
  [[nodiscard]] const open_viii::archive::FIFLFS<false> *operator->() const noexcept { return &m_archive; }
  /**
   * Get the archive
   * @return FIFLFS
   */
  [[nodiscard]] const open_viii::archive::FIFLFS<false> &operator*() const noexcept { return m_archive; }

  /**
   * name of directory
   * @return string
   */
  const std::string &dir_name() const noexcept { return m_dir_name; }
  /**
   * filename of fi file
   * @return string
   */
  const std::string &fi_filename() const noexcept { return m_fi_filename; }
  /**
   * filename of fl file
   * @return string
   */
  const std::string &fl_filename() const noexcept { return m_fl_filename; }
  /**
   * filename of fs file
   * @return string
   */
  const std::string &fs_filename() const noexcept { return m_fs_filename; }
  /**
   * filename of map file
   * @return string
   */
  const std::string &map_filename() const noexcept { return m_map_filename; }
  /**
   * filename of mim file
   * @return string
   */
  const std::string &mim_filename() const noexcept { return m_mim_filename; }
  /**
   * This will put the give you a path you can append what you want to and save. It'll be in the same directory with the
   * same prefix.
   * @return string of the path minus extension.
   */
  const std::string &output_prefix() const noexcept { return m_output_prefix; }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_RESWIZZLETREE_HPP
