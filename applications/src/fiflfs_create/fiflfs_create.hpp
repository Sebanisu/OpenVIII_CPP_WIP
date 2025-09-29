//
// Created by pcvii on 5/17/2021.
//

#ifndef VIIIARCHIVE_FIFLFS_CREATE_HPP
#define VIIIARCHIVE_FIFLFS_CREATE_HPP
#include <filesystem>
#include <string_view>

void
  fiflfs_create(
    const std::filesystem::path &src,
    const std::filesystem::path &dst,
    const std::string_view       archive_name);
auto
  check(std::string_view in, bool create = false) -> std::string_view;

#endif// VIIIARCHIVE_FIFLFS_CREATE_HPP
