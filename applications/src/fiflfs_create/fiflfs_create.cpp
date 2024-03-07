//
// Created by pcvii on 5/17/2021.
//

#include "fiflfs_create.hpp"
#include "open_viii/archive/FIFLFS_operations.hpp"
#include <filesystem>
#include <iostream>
#include <string_view>

int
  main(
    [[maybe_unused]] const int         argc,
    [[maybe_unused]] const char *const argv[])
{
  const auto argspan = std::span<const char *const>(argv, argc);
  if (argc >= 4) {
    const auto src      = check(argspan[1]);
    const auto dst      = check(argspan[2], true);
    const auto dst_name = std::string_view(argspan[3]);
    if (std::empty(src) || std::empty(dst)) {
      std::cerr << "fiflfs_create source destination archive\n";
      return 1;
    }
    fiflfs_create(src, dst, dst_name);
    return 0;
  }
  const auto get_path
    = [](std::string_view msg, bool create = false) -> std::string {
    std::string temp;
    while (true) {
      std::cout << msg << std::flush;
      std::getline(std::cin, temp);
      std::error_code error_code{};
      const bool      found = std::filesystem::exists(temp, error_code);
      if (error_code) {
        std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                  << error_code.value() << ": " << error_code.message()
                  << error_code.value() << " - path: " << temp << std::endl;
        error_code.clear();
      }
      if (found) {
        break;
      }
      if (create) {
        bool const created
          = std::filesystem::create_directories(temp, error_code);
        if (error_code) {
          std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                    << error_code.value() << ": " << error_code.message()
                    << error_code.value() << " - path: " << temp << std::endl;
          error_code.clear();
          if (created) {
            std::cout << "created directory " << temp << "\n";
          }
        }
        break;
      }
      std::cout << temp << " is not a valid path..." << std::endl;
    }
    return temp;
  };
  const auto get_name = [](std::string_view msg) -> std::string {
    std::string temp;
    std::cout << msg << std::flush;
    std::getline(std::cin, temp);
    return temp;
  };
  const std::string src      = get_path("Source directory:");
  const std::string dst      = get_path("Destination path:", true);
  const std::string dst_name = get_name("Archive name:");
  fiflfs_create(src, dst, dst_name);
  return 0;
}

void
  fiflfs_create(
    const std::filesystem::path &src,
    const std::filesystem::path &dst,
    const std::string_view       archive_name)
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
    std::error_code error_code{};
    const bool      found = std::filesystem::exists(source_compress_dat, error_code);
    if (error_code) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << error_code.value() << ": " << error_code.message() << error_code.value()
                << " - path: " << source_compress_dat << std::endl;
      error_code.clear();
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
    std::error_code error_code{};
    const bool found = std::filesystem::exists(source_names_dat, error_code);
    if (error_code) {
      std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - "
                << error_code.value() << ": " << error_code.message()
                << error_code.value() << " - path: " << source_names_dat
                << std::endl;
      error_code.clear();
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
    static_cast<std::ios_base::openmode>(
      static_cast<std::uint32_t>(std::ios::out)
      | static_cast<std::uint32_t>(std::ios::binary)
      | static_cast<std::uint32_t>(std::ios::trunc)));
  std::fstream fs_fl(
    fl_name,
    static_cast<std::ios_base::openmode>(
      static_cast<std::uint32_t>(std::ios::out)
      | static_cast<std::uint32_t>(std::ios::binary)
      | static_cast<std::uint32_t>(std::ios::trunc)));
  std::fstream fs_fs(
    fs_name,
    static_cast<std::ios_base::openmode>(
      static_cast<std::uint32_t>(std::ios::out)
      | static_cast<std::uint32_t>(std::ios::binary)
      | static_cast<std::uint32_t>(std::ios::trunc)));
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
    open_viii::archive::FI const fi = open_viii::archive::append_entry(
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

auto
  check(std::string_view in, bool create) -> std::string_view
{
  std::error_code ec    = {};
  const bool      found = std::filesystem::exists(in, ec);
  if (ec) {
    std::cerr << "error " << __FILE__ << ":" << __LINE__ << " - " << ec.value()
              << ": " << ec.message() << ec.value() << " - path: " << in
              << std::endl;
    ec.clear();
  }
  if (found) {
    return in;
  }
  if (create) {
    bool const created = std::filesystem::create_directories(in, ec);
    if (ec) {
      std::cerr << __FILE__ << ":" << __LINE__ << " - " << ec.value() << ": "
                << ec.message() << " - " << in << std::endl;
      ec.clear();
    }
    if (created) {
      std::cout << "created directory " << in << "\n";
      return in;
    }
  }
  std::cerr << in << " is not a valid path..." << std::endl;
  return std::string_view{};
}