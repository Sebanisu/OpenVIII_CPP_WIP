//
// Created by pcvii on 1/11/2021.
//
#ifndef VIIIARCHIVE_WRITE_HPP
#define VIIIARCHIVE_WRITE_HPP
#include "open_viii/Concepts.hpp"
namespace open_viii::tools {
template<typename lambdaT>
requires(std::invocable<lambdaT, std::ostream &>)
  [[maybe_unused]] inline bool write_buffer(
    const lambdaT          &lambda,
    const std::string_view &path,
    const std::string_view &root = "tmp")
{
  auto dir      = std::filesystem::path(root);
  auto filename = dir / path;
  std::filesystem::create_directories(filename.parent_path());
  auto fp = std::ofstream{};
  for (;;) {
    fp.open(filename, std::ios::out | std::ios::binary | std::ios::trunc);
    if (fp.is_open()) {
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  //    if (fp.is_open()) {
  //  const auto &string = filename.string();
  //  std::cout << "Saving: \t\"" << string << "\"\n";// << std::flush;
  lambda(fp);
  fp.close();
  return true;
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
