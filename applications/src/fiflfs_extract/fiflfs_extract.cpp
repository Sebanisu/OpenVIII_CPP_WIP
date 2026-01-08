//
// Created by pcvii on 5/17/2021.
//
#include "fiflfs_extract.hpp"
#include "open_viii/archive/FIFLFS.hpp"
#include <filesystem>
#include <iostream>
#include <string_view>

int
  main(
    [[maybe_unused]] const int         argc,
    [[maybe_unused]] const char *const argv[])
{
  const auto argspan
    = std::span<const char *const>(argv, static_cast<size_t>(argc));
  if (argc >= 3) {
    const auto src = check(argspan[1]);
    const auto dst = check(argspan[2], true);
    if (std::empty(src) || std::empty(dst)) {
      std::cerr << "fiflfs_extract source destination\n";
      return 1;
    }
    fiflfs_extract(src, dst);
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
          std::cerr << __FILE__ << ":" << __LINE__ << " - "
                    << error_code.value() << ": " << error_code.message()
                    << " - " << temp << std::endl;
          error_code.clear();
        }
        if (created) {
          std::cout << "created directory " << temp << "\n";
          break;
        }
      }
      std::cout << temp << " is not a valid path..." << std::endl;
    }
    return temp;
  };
  const std::string src = get_path("Source fi/fl/fs file:");
  const std::string dst = get_path("Destination path:", true);
  fiflfs_extract(src, dst);
  return 0;
}

void
  fiflfs_extract(
    const std::filesystem::path &src,
    const std::filesystem::path &dst)
{
  std::cout << "Loading Archive\n";
  const open_viii::archive::FIFLFS<false> archive(src);
  if (!archive.all_set()) {
    throw std::runtime_error(
      "Failed to open the archive files: " + src.string());
  }
  std::cout << "Extracting " << src << " to " << dst << '\n';
  const auto   target_folder       = dst.string();
  const auto   target_compress_dat = target_folder + ".compress";
  std::fstream fp_compress{};
  fp_compress.open(
    target_compress_dat,
    static_cast<std::ios_base::openmode>(
      static_cast<std::uint32_t>(std::ios::out)
      | static_cast<std::uint32_t>(std::ios::binary)
      | static_cast<std::uint32_t>(std::ios::trunc)));
  const auto   target_names_dat = target_folder + ".names";
  std::fstream fp_names{};
  fp_names.open(
    target_names_dat,
    static_cast<std::ios_base::openmode>(
      static_cast<std::uint32_t>(std::ios::out)
      | static_cast<std::uint32_t>(std::ios::binary)
      | static_cast<std::uint32_t>(std::ios::trunc)));
  [[maybe_unused]] const auto dump = [&target_folder, &fp_compress, &fp_names](
                                       const std::vector<char>      &in_buffer,
                                       const std::string            &in_path,
                                       const open_viii::archive::FI &fi) {
    open_viii::tools::write_buffer(in_buffer, in_path, target_folder);
    char const compress_type = static_cast<char>(fi.compression_type());
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
