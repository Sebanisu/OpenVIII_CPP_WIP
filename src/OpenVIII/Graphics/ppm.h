//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_PPM_H
#define VIIIARCHIVE_PPM_H
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string_view>
#include "color.h"
#include "OpenVIII/concepts.h"
namespace OpenVIII::Graphics {
struct ppm
{

  template<std::ranges::contiguous_range cT>
  static void save(const cT &data, std::size_t width, std::size_t height, std::string_view filename)
  {// how do i make the concept reject ranges that aren't of Colors? I'm at least checking for Color down below.
    if (std::ranges::size(data) < width * height) {
      return;
    }
    std::fstream fs{};
    fs.open(filename.data(), std::ios::binary | std::ios::out);
    if (fs.is_open()) {
      fs << "P6\n# THIS IS A COMMENT\n" << width << " " << height << "\n255\n";
      std::vector<char> outBuffer{};
      // outBuffer.reserve(std::ranges::size(data)*3U);
      for (const Color auto &color : data) {// organize the data in ram first then write all at once.
        // outBuffer.emplace_back(static_cast<char>(color.R()));
        // outBuffer.emplace_back(static_cast<char>(color.G()));
        // outBuffer.emplace_back(static_cast<char>(color.B()));
        fs << color.R();
        fs << color.G();
        fs << color.B();
      }
      // fs.write(outBuffer.data(),static_cast<long>(outBuffer.size()));
      fs.flush();
      fs.close();
    }
  }
};
}// namespace OpenVIII::Graphics
#endif// VIIIARCHIVE_PPM_H
