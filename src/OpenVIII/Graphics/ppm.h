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
namespace OpenVIII::Graphics {
struct ppm
{


  static void save(const std::vector<color32<>>&data, std::size_t width, std::size_t height, std::string_view filename)
  {
    std::fstream fs{};
    fs.open(filename.data(),std::ios::binary|std::ios::out);
    if(fs.is_open()) {
      fs << "P6\n# THIS IS A COMMENT\n"<<width<<" "<<height<<"\n255\n";
      for(const auto &color : data)
      {
        fs << color.R()<<color.G()<<color.B();
      }
    }
  }
};
}
#endif// VIIIARCHIVE_PPM_H
