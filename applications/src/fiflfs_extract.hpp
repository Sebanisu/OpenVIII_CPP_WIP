//
// Created by pcvii on 5/17/2021.
//

#ifndef VIIIARCHIVE_FIFLFS_EXTRACT_HPP
#define VIIIARCHIVE_FIFLFS_EXTRACT_HPP
#include "open_viii/archive/FIFLFS.hpp"
#include <filesystem>
#include <iostream>
#include <string_view>
void
  fiflfs_extract(const std::filesystem::path src,
                 const std::filesystem::path dst)
{
  std::cout << "Loading Archive\n";
  const open_viii::archive::FIFLFS<false> archive(src);
  std::cout << "Extracting " << src << " to " << dst << '\n';
  const auto   target_folder       = dst.string();
  const auto   target_compress_dat = target_folder + ".compress";
  std::fstream fp_compress{};
  fp_compress.open(target_compress_dat,
                   std::ios::out | std::ios::binary | std::ios::trunc);
  const auto   target_names_dat = target_folder + ".names";
  std::fstream fp_names{};
  fp_names.open(target_names_dat,
                std::ios::out | std::ios::binary | std::ios::trunc);
  [[maybe_unused]] const auto dump = [&target_folder, &fp_compress, &fp_names](
                                       const std::vector<char> &     in_buffer,
                                       const std::string &           in_path,
                                       const open_viii::archive::FI &fi) {
    open_viii::tools::write_buffer(in_buffer, in_path, target_folder);
    char compress_type = static_cast<char>(fi.compression_type());
    fp_compress.write(&compress_type, 1);
    fp_names.write(in_path.data(), static_cast<long>(in_path.size()));
    fp_names.write("\n", 1);
  };
  archive.execute_on({}, dump);
  std::cout << "Done! - Files wrote to " << target_folder << '\n';
  std::cout << target_compress_dat
            << " has a byte for each file saying if it's compressed or not\n";
  std::cout << target_names_dat << " has the paths in the same order\n";
}
#endif// VIIIARCHIVE_FIFLFS_EXTRACT_HPP
