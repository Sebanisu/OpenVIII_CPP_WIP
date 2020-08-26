//
// Created by pcvii on 8/26/2020.
//

#ifndef VIIIARCHIVE_LZS_H
#define VIIIARCHIVE_LZS_H
#include "OpenVIII/Graphics/color/color16.h"
#include "rectangle.h"
#include "OpenVIII/Compression/LZSS.h"
namespace OpenVIII::Graphics
{
/**
 * lzs images are LZSS images with a small header (X,Y,H,W) and 16bit colors.
 */
struct lzs
{
  rectangle<std::uint16_t> rectangle_{};

};
}
#endif// VIIIARCHIVE_LZS_H
