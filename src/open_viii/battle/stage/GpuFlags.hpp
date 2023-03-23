//
// Created by pcvii on 11/17/2020.
//
#ifndef VIIIARCHIVE_GPUFLAGS_HPP
#define VIIIARCHIVE_GPUFLAGS_HPP
namespace open_viii::battle::stage {
/**
 * @brief Flags that affect how the triangles or quads are drawn.
 * @see https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X
 */
struct GpuFlags
{
public:
  bool v0  : 1 {};///< @brief Bit 0 (0x1)
  /**
   * @brief Bit 1 (0x2): Second bit seems to affect fire/light blending.
   */
  bool add : 1 {};
  bool v2  : 1 {};///< @brief Bit 2 (0x4)
  bool v3  : 1 {};///< @brief Bit 3 (0x8)
  bool v4  : 1 {};///< @brief Bit 4 (0x10)
  bool v5  : 1 {};///< @brief Bit 5 (0x20)
  bool v6  : 1 {};///< @brief Bit 6 (0x40)
  bool v7  : 1 {};///< @brief Bit 7 (0x80)
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GPUFLAGS_HPP
