//
// Created by pcvii on 1/11/2021.
//
#ifndef VIIIARCHIVE_WRITE_HPP
#define VIIIARCHIVE_WRITE_HPP
#include "open_viii/Concepts.hpp"
#include <iostream>
namespace open_viii::tools {
template<typename lambdaT>
  requires(std::invocable<lambdaT, std::ostream &>)
[[maybe_unused]] inline bool
  write_buffer(
    const lambdaT          &lambda,
    const std::string_view &path,
    const std::string_view &root = "tmp")
{
  bool            ret = false;
  std::error_code ec{};
  auto            dir      = std::filesystem::path(root);
  auto            filename = dir / path;

  std::filesystem::create_directories(filename.parent_path(), ec);
  if (ec) {
    std::cerr
      << __FILE__ << ":" << __LINE__
      << " - Failed to create directories (permission denied? invalid path?) - "
      << ec.value() << ": " << ec.message() << " - "
      << filename.parent_path().string() << '\n';
    ec.clear();
  }

  std::filesystem::remove(filename, ec);
  if (ec) {
    std::cerr << __FILE__ << ":" << __LINE__
              << " - Failed to remove existing file (file locked? permission "
                 "denied?) - "
              << ec.value() << ": " << ec.message() << " - " << filename
              << '\n';
    ec.clear();
  }

  auto fp    = std::ofstream{};
  int  tries = 10;
  for (; tries != 0; --tries) {
    try {
      fp.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    }
    catch (const std::ios_base::failure &e) {
      std::cerr << __FILE__ << ":" << __LINE__
                << "ofstream::open threw exception: " << e.what() << '\n';
    }
    if (!fp.is_open()) {
      std::cerr << __FILE__ << ":" << __LINE__
                << " - Failed to open file for writing (disk full? no write "
                   "permission?) - "
                << filename.string() << '\n';
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    else {
      ret = true;
      break;
    }
  }

  if (tries <= 0 && !fp.is_open()) {
    std::cerr << __FILE__ << ":" << __LINE__
              << " - Max retries reached. Still unable to open file\n";
  }

  lambda(fp);
  fp.close();
  return ret;
}

[[maybe_unused]] inline bool
  write_buffer(
    [[maybe_unused]] const std::span<const char> &buffer,
    const std::string_view                       &path,
    const std::string_view                       &root = "tmp")
{
  if (std::ranges::empty(buffer)) {
    // std::cout << (std::string("Buffer is empty: \"")+ std::string(path) +
    // std::string("\"\n"));
    return false;
  }
  return write_buffer(
    [&buffer](std::ostream &fp) {
      fp.write(
        std::ranges::data(buffer),
        static_cast<long>(std::ranges::size(buffer)));
    },
    path,
    root);
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_WRITE_HPP
