//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GEOMETRIES_HPP
#define VIIIARCHIVE_GEOMETRIES_HPP
#include "Geometry.hpp"
namespace open_viii::battle::stage {
/**
 * @brief Represents a collection of geometry objects in the battle stage.
 * There may be more than one Geometry object. Check for 01 00 01 00 to know if
 * there are one or more.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry
 */
struct Geometries
{
public:
  std::vector<Geometry> m_geometries{};///< @brief Vector of Geometry objects
                                       ///< (usually one, but can be more).

  static constexpr std::array<char, 4> START_OF_HEADER
    = { 1, 0, 1, 0 };///< @brief Sequence indicating the start of a header.
};
}// namespace open_viii::battle::stage

#endif// VIIIARCHIVE_GEOMETRIES_HPP
