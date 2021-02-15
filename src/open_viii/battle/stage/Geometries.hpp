//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRIES_HPP
#define VIIIARCHIVE_GEOMETRIES_HPP
#include "Geometry.hpp"
#include <array>
namespace open_viii::battle::stage {
/**
 * There maybe more than one Geometry checking for 01 00 01 00 to know if there
 * is one or more
 */
struct Geometries
{
private:
  std::vector<Geometry>
    m_geometries{};// there is usually one. But can be more than 1.
  static constexpr std::array<char, 4> START_OF_HEADER = { 1, 0, 1, 0 };
public:
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GEOMETRIES_HPP
