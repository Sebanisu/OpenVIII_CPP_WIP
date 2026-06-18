//
// Created by pcvii on 1/11/2021.
//
#ifndef VIIIARCHIVE_WRITE_HPP
#define VIIIARCHIVE_WRITE_HPP
#include "open_viii/Concepts.hpp"
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <fmt/std.h>
#include <fstream>
#include <iostream>
#include <span>
#include <spdlog/spdlog.h>
#include <string>
#include <string_view>
#include <system_error>
#include <thread>
namespace open_viii::tools {
template<typename lambdaT>
  requires(std::invocable<lambdaT, std::ostream &>)
[[maybe_unused]] inline bool
  write_buffer(
    const lambdaT               &lambda,
    const std::filesystem::path &path,
    const std::filesystem::path &root = "tmp")
{
  bool            ret = false;
  std::error_code ec{};
  const auto      filename = root / path;

  std::filesystem::create_directories(filename.parent_path(), ec);
  if (ec) {
    spdlog::error(
      "Failed to create directories (permission denied? invalid path?) "
      "- {}: {} - {}",
      ec.value(),
      ec.message(),
      filename.parent_path().string());

    ec.clear();
  }

  std::filesystem::remove(filename, ec);
  if (ec) {
    spdlog::error(
      "Failed to remove existing file (file locked? permission denied?) "
      "- {}: {} - {}",
      ec.value(),
      ec.message(),
      filename.string());

    ec.clear();
  }

  auto fp    = std::ofstream{};
  int  tries = 10;
  for (; tries != 0; --tries) {
    try {
      fp.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    }
    catch (const std::ios_base::failure &e) {
      spdlog::error("ofstream::open threw exception: {}", e.what());
    }

    if (!fp.is_open()) {
      spdlog::error(
        "Failed to open file for writing (disk full? no write permission?) - "
        "{}",
        filename);

      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    else {
      ret = true;
      break;
    }
  }

  if (tries <= 0 && !fp.is_open()) {
    spdlog::error("Max retries reached. Still unable to open file");
  }

  lambda(fp);
  fp.close();
  return ret;
}

[[maybe_unused]] inline bool
  write_buffer(
    [[maybe_unused]] const std::span<const char> &buffer,
    const std::filesystem::path                  &path,
    const std::filesystem::path                  &root = "tmp")
{
  if (std::ranges::empty(buffer)) {
    spdlog::trace("Buffer is empty: {}", path);
    return false;
  }
  return write_buffer(
    [&buffer](std::ostream &fp) {
      fp.write(
        std::ranges::data(buffer),
        static_cast<std::streamsize>(std::ranges::size(buffer)));
    },
    path,
    root);
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_WRITE_HPP
