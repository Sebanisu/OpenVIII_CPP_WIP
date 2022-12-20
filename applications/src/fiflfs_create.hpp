//
// Created by pcvii on 5/17/2021.
//

#ifndef VIIIARCHIVE_FIFLFS_CREATE_HPP
#define VIIIARCHIVE_FIFLFS_CREATE_HPP
#include "open_viii/archive/FIFLFS.hpp"
#include <filesystem>
#include <iostream>
#include <string_view>
void
  fiflfs_create(
    const std::filesystem::path src,
    const std::filesystem::path dst,
    const std::string_view      archive_name)
{
  std::cout << "Source directory: " << src << '\n';
  std::cout << "Destination path: " << dst << '\n';
  std::cout << "Archive Name: " << archive_name << '\n';

  const auto source_folder       = src.string();
  const auto source_compress_dat = source_folder + ".compress";
  const auto source_names_dat    = source_folder + ".names";
  {
    std::cout << "Source directory compressed data: \n\t" << source_compress_dat
              << '\n';
    std::error_code ec{};
    const bool      found = std::filesystem::exists(source_compress_dat, ec);
    if (ec) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << ec.value() << ": " << ec.message() << ec.value()
                << " - path: " << source_compress_dat << std::endl;
      ec.clear();
      exit(1);
    }
    else if (found) {
      std::cout << "\t\tExists\n";
    }
    else {
      std::cout << "\t\tMissing\n";
      exit(1);
    }
  }
  {
    std::cout << "Source directory names data: \n\t" << source_names_dat
              << '\n';
    std::error_code ec{};
    const bool      found = std::filesystem::exists(source_names_dat, ec);
    if (ec) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << ec.value() << ": " << ec.message() << ec.value()
                << " - path: " << source_names_dat << std::endl;
      ec.clear();
      exit(1);
    }
    if (found) {
      std::cout << "\t\tExists\n";
    }
    else {
      std::cout << "\t\tMissing\n";
      exit(1);
    }
  }
  const auto fi_name
    = (dst
       / (std::string(archive_name) + std::string(open_viii::archive::FI::EXT)))
        .string();
  const auto fl_name
    = (dst
       / (std::string(archive_name) + std::string(open_viii::archive::fl::EXT)))
        .string();
  const auto fs_name
    = (dst
       / (std::string(archive_name) + std::string(open_viii::archive::FS::EXT)))
        .string();

  std::cout << "Destination FI: " << fi_name << '\n';
  std::cout << "Destination FL: " << fl_name << '\n';
  std::cout << "Destination FS: " << fs_name << '\n';
  std::fstream fs_fi(
    fi_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::fstream fs_fl(
    fl_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::fstream fs_fs(
    fs_name,
    std::ios::out | std::ios::binary | std::ios::trunc);
  std::fstream fs_source_names(
    source_names_dat,
    std::ios::in | std::ios::binary);
  std::fstream fs_source_compress(
    source_compress_dat,
    std::ios::in | std::ios::binary);
  const auto get_compression
    = [&fs_source_compress]() -> open_viii::CompressionTypeT {
    char temp{};
    fs_source_compress.read(&temp, 1);
    return static_cast<open_viii::CompressionTypeT>(temp);
  };
  const auto get_name = [&fs_source_names]() -> std::string {
    std::string temp{};
    std::getline(fs_source_names, temp);
    return temp;
  };
  while (true) {
    const auto current_name = get_name();
    if (std::empty(current_name)) {
      break;
    }
    std::puts(current_name.data());
    const auto current_compression = get_compression();
    switch (current_compression) {
    case open_viii::CompressionTypeT::none:
      std::puts("\t Not Compressed");
      break;
    case open_viii::CompressionTypeT::lzss:
      std::puts("\t LZSS Compressed");
      break;
    case open_viii::CompressionTypeT::lz4:
      std::puts("\t LZ4 Compressed");
      break;
    }
    open_viii::archive::FI fi = open_viii::archive::append_entry(
      fs_fs,
      src / current_name,
      current_compression);
    open_viii::archive::append_entry(fs_fi, fi);
    open_viii::archive::append_entry(
      fs_fl,
      std::filesystem::path(current_name));
    std::puts("\t\tAdded File");
  }
  std::puts("Operation Complete!");
}
#endif// VIIIARCHIVE_FIFLFS_CREATE_HPP
