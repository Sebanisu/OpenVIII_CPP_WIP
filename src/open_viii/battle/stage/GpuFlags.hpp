//
// Created by pcvii on 11/17/2020.
//

#ifndef VIIIARCHIVE_GPUFLAGS_HPP
#define VIIIARCHIVE_GPUFLAGS_HPP

namespace open_viii::battle::stage {
/**
 * Flags that effect how the triangles or quads are drawn.
 */
struct GpuFlags
{
public:
  bool v0 : 1 {};// 0x1
  /**
   * Second bit seems to effect fire / light blending.
   */
  bool add : 1 {};// 0x2
  bool v2 : 1 {};// 0x4
  bool v3 : 1 {};// 0x8
  bool v4 : 1 {};// 0x10
  bool v5 : 1 {};// 0x20
  bool v6 : 1 {};// 0x40
  bool v7 : 1 {};// 0x80
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_GPUFLAGS_HPP
