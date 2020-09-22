//
// Created by pcvii on 9/21/2020.
//

#ifndef VIIIARCHIVE_RESWIZZLE_HPP
#define VIIIARCHIVE_RESWIZZLE_HPP
#include "ReswizzleTree.hpp"
namespace open_viii::graphics::background
{
struct Reswizzle
{
private:
  const ReswizzleTree m_tree{};
public:
  Reswizzle(ReswizzleTree && tree)
  : m_tree(std::move(tree))
  {

  }
};
}
#endif// VIIIARCHIVE_RESWIZZLE_HPP
