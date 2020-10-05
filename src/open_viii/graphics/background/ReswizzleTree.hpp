//
// Created by pcvii on 9/21/2020.
//

#ifndef VIIIARCHIVE_RESWIZZLETREE_HPP
#define VIIIARCHIVE_RESWIZZLETREE_HPP
#include <utility>

#include "Deswizzle.hpp"
#include "Map.hpp"
#include "Mim.hpp"
#include "Reswizzle.hpp"
#include "open_viii/archive/FIFLFS.hpp"
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

  [[nodiscard]] std::string get_path_with_ext(const std::string_view &extension) const
  {
    return "\\" + m_dir_name + std::string(extension);
    // the FL files contain \\ I change to / in post for easy dumping to linux and support of filesystem.
  }
  [[nodiscard]] MimType get_mim_type() const
  {
    if (static_cast<bool>(m_archive)) {
      std::vector<std::pair<unsigned int, std::string>> mims =
        m_archive.get_vector_of_indexs_and_files({ m_mim_filename });
      for (const std::pair<unsigned int, std::string> &pair : mims) {
        std::cout << '\t' << pair.first << '\t' << pair.second << std::endl;
      }
      if (std::ranges::empty(mims)) {
        return {};// no mim file.
      }
      open_viii::FI_Like auto mim_fi = m_archive.get_entry_by_index(mims[0].first);
      return Mim::get_texture_type(mim_fi.uncompressed_size(), m_dir_name);
    }
    return {};
  }

  const MimType &mim_type() const noexcept
  {
    return m_mim_type;
  }
  [[nodiscard]] std::vector<char> get_mim_buffer() const
  {
    if (static_cast<bool>(m_archive)) {
      return m_archive.get_entry_data({ m_mim_filename });
    }
    return {};
  }
  [[nodiscard]] std::vector<char> get_map_buffer() const
  {
    if (static_cast<bool>(m_archive)) {
      return m_archive.get_entry_data({ m_map_filename });
    }
    return {};
  }
  template<typename map_type>
  requires(
    std::is_same_v<map_type,
      Tile1> || std::is_same_v<map_type, Tile2> || std::is_same_v<map_type, Tile3>) void reswizzle_with_type() const
  {
    const auto r = Reswizzle<map_type>(get_map_buffer(), m_dir_path, m_dir_name, m_output_prefix);
    r.process();
  }
  template<typename map_type>
  requires(
    std::is_same_v<map_type,
      Tile1> || std::is_same_v<map_type, Tile2> || std::is_same_v<map_type, Tile3>) void deswizzle_with_type() const
  {

    const auto mim = Mim(get_mim_buffer(),m_mim_filename);
    const auto map = Map<map_type>(get_map_buffer());
    const auto r = Deswizzle(mim, map , m_output_prefix);
    r.save();
    // const auto r = Deswizzle<map_type>(get_map_buffer(), m_dir_path, m_dir_name, m_output_prefix);
    // r.process();
  }

public:
  ReswizzleTree(const open_viii::archive::FIFLFS<true> &field, const std::filesystem::path &dir_path)
    : m_dir_path(dir_path),
      m_dir_name(dir_path.filename().string()),
      m_fi_filename(get_path_with_ext(archive::FI::EXT)),
      m_fl_filename(get_path_with_ext(open_viii::archive::FL::EXT)),
      m_fs_filename(get_path_with_ext(open_viii::archive::FS::EXT)),
      m_map_filename(get_path_with_ext(open_viii::graphics::background::Map<>::EXT).substr(1)),
      m_mim_filename(get_path_with_ext(open_viii::graphics::background::Mim::EXT).substr(1)),
      m_output_prefix((m_dir_path / m_dir_name).string()),
      m_archive(field.get_fiflfs({ m_fi_filename, m_fl_filename, m_fs_filename })),
      m_mim_type(get_mim_type())
  {}
  /**
   * true if archive is set and full. false if invalid.
   * @return bool
   */
  [[nodiscard]] explicit operator bool() const noexcept
  {
    return static_cast<bool>(m_archive);
  }

  /**
   * Point to members of the archive
   * @return FIFLFS*
   */
  [[nodiscard]] const open_viii::archive::FIFLFS<false> *operator->() const noexcept
  {
    return &m_archive;
  }
  /**
   * Get the archive
   * @return FIFLFS
   */
  [[nodiscard]] const open_viii::archive::FIFLFS<false> &operator*() const noexcept
  {
    return m_archive;
  }


  /**
   * spawn a reswizzle object using the mim_type to decide what template arguments should be.
   */
  void reswizzle() const
  {
    if (m_mim_type.type() == 1) {
      reswizzle_with_type<Tile1>();
    } else if (m_mim_type.type() == 2) {
      reswizzle_with_type<Tile2>();
    } else if (m_mim_type.type() == 3) {
      reswizzle_with_type<Tile3>();
    }
  }
  /**
   * spawn a deswizzle object using the mim_type to decide what template arguments should be.
   */
  void deswizzle() const
  {
    if (m_mim_type.type() == 1) {
      deswizzle_with_type<Tile1>();
    } else if (m_mim_type.type() == 2) {
      deswizzle_with_type<Tile2>();
    } else if (m_mim_type.type() == 3) {
      deswizzle_with_type<Tile3>();
    }
  }
};
}// namespace open_viii::graphics::background
#endif// VIIIARCHIVE_RESWIZZLETREE_HPP
