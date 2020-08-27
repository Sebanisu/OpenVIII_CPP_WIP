//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_PPM_H
#define VIIIARCHIVE_PPM_H
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>
#include "color.h"
#include "OpenVIII/concepts.h"
namespace OpenVIII::Graphics {
struct ppm
{

  template<std::ranges::range cT>
  static void save(const cT &data, std::size_t width, std::size_t height, std::string_view filename)
  {// how do i make the concept reject ranges that aren't of Colors? I'm at least checking for Color down below.
    std::fstream fs{};
    fs.open(filename.data(), std::ios::binary | std::ios::out);
    if (fs.is_open()) {
      fs << "P6\n# THIS IS A COMMENT\n" << width << " " << height << "\n255\n";
      for (const Color auto &color : data) { fs << color.R() << color.G() << color.B(); }
    }
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_PPM_H
