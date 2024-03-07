//
// Created by pcvii on 5/17/2021.
//

#ifndef VIIIARCHIVE_FIFLFS_EXTRACT_HPP
#define VIIIARCHIVE_FIFLFS_EXTRACT_HPP
#include <filesystem>
void
  fiflfs_extract(
    const std::filesystem::path &src,
    const std::filesystem::path &dst);
auto
  check(std::string_view in, bool create = false) -> std::string_view;
#endif// VIIIARCHIVE_FIFLFS_EXTRACT_HPP
