//
// Created by pcvii on 9/21/2020.
//

#ifndef VIIIARCHIVE_RESWIZZLE_HPP
#define VIIIARCHIVE_RESWIZZLE_HPP
#include <utility>

#include "Map.hpp"
#include "Mim.hpp"
namespace open_viii::graphics::background {
struct Reswizzle
{
private:
  const std::string m_dir_name{};
  const std::filesystem::path m_dir_path{}
  const std::string m_fi_filename{};

public:
  ReswizzleTree(std::filesystem::path dir_path)
  :
  m_dir_name(dir_path.filename().string()),
      m_dir_path(std::move(dir_path)),
  {

  }
};
}
#endif// VIIIARCHIVE_RESWIZZLE_HPP
