//
// Created by pcvii on 11/18/2020.
//

#ifndef VIIIARCHIVE_CAMERAANIMATIONSET_HPP
#define VIIIARCHIVE_CAMERAANIMATIONSET_HPP
#include <cstdint>
#include <vector>
/**
 * http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#CameraAnimationSet
 */
struct CameraAnimationSet
{
public:
  std::vector<std::uint16_t> animOffsets;
  std::uint32_t globalSetPointer;
};
#endif// VIIIARCHIVE_CAMERAANIMATIONSET_HPP
