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
namespace open_viii::graphics {
struct Ppm
{

  template<std::ranges::contiguous_range cT>
  static void save(const cT &data, std::size_t width, std::size_t height, const std::string_view &input)
  {// how do i make the concept reject ranges that aren't of Colors? I'm at least checking for Color down below.
    if (width == 0 || height == 0 || std::ranges::empty(data)) {
      return;
    }

    auto tmp = std::filesystem::path(input);
    std::string filename{ (tmp.parent_path() / tmp.stem()).string() + "_" + tmp.extension().string().substr(1)
                          + ".ppm" };
    if (std::ranges::size(data) < width * height) {
      std::cout << std::ranges::size(data) << ", " << width << '*' << height << '=' << width * height << '\n';
      return;
    }
    std::stringstream ss{};
    ss << "P6\n# THIS IS A COMMENT\n" << width << " " << height << "\n255\n";
    for (const Color auto &color : data) {// organize the data in ram first then write all at once.
      ss << color.r();
      ss << color.g();
      ss << color.b();
    }
    Tools::write_buffer(ss.str(), filename);
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_PPM_H
