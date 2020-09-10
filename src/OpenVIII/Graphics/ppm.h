//
// Created by pcvii on 8/25/2020.
//

#ifndef VIIIARCHIVE_PPM_H
#define VIIIARCHIVE_PPM_H
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string_view>
#include "color.h"
#include "OpenVIII/concepts.h"
namespace open_viii::graphics {
struct Ppm
{
private:
  std::vector<Color24<0, 1, 2>> m_colors{};

public:
  explicit Ppm(const std::string &buffer)
  {
    // the support is mostly limited to the same type that is made by the save function.
    // I can't use span because span_stream does not exist yet it missed cpp20... so this will need to be a string
    // stream? I know not to use std::regex but i'm feeling like that's better than what I'm doing here. heh.


    // sample header ss << "P6\n# THIS IS A COMMENT\n" << width << " " << height << "\n255\n";
    auto ss = std::stringstream(buffer);
    std::string line{};
    std::string type{};
    std::string comment{};
    std::string w_h{};
    std::uint16_t width{};
    std::uint16_t height{};
    std::string bytesize{};
    while (std::getline(ss, line)) {
      if (line == "P6") {
        type = line;
        continue;
      }
      if (line[0] == '#') {
        comment += line;
        continue;
      }
      if (line == "255") {
        bytesize = line;
        continue;
      }
      if ([&line, &width, &height, &w_h]() -> bool {
            char space{};
            auto lss = std::stringstream(line);
            lss >> width >> space >> height;
            return space == '_';
          }()) {
        w_h = line;
        continue;
      }
      if (!std::ranges::empty(bytesize) && ((height > 0 && width > 0) || !std::ranges::empty(w_h))) {
        break;
      }
    }

    auto bufferspan = (std::span<const char>(buffer)).subspan(static_cast<std::size_t>(ss.tellg()));
    if (std::ranges::size(bufferspan) > 0) {
      auto sz = std::ranges::size(bufferspan) / sizeof(Color24<0, 1, 2>);
      const auto colorspan =
        std::span<const Color24<0, 1, 2>>{ reinterpret_cast<const Color24<0, 1, 2> *>(std::ranges::data(bufferspan)),
          sz };
      m_colors = { std::ranges::cbegin(colorspan), std::ranges::cend(colorspan) };
    }
  }
  [[nodiscard]] const auto &colors() { return m_colors; }
  template<std::ranges::contiguous_range cT>
  static void save(const cT &data, std::size_t width, std::size_t height, const std::string_view &input, bool skip_check=false)
  {// how do i make the concept reject ranges that aren't of Colors? I'm at least checking for Color down below.
    if(!skip_check) {
      if (width == 0 || height == 0 || std::ranges::empty(data)
          || std::all_of(std::execution::par_unseq, data.begin(), data.end(), [](const Color auto &color) -> bool {
               return color.a() == 0U;
             })) {
        return;
      }
    }

    auto tmp = std::filesystem::path(input);
    std::string filename{ (tmp.parent_path() / tmp.stem()).string() + "_" + tmp.extension().string().substr(1)
                          + ".ppm" };
    if (std::ranges::size(data) < width * height) {
      std::cout << std::ranges::size(data) << ", " << width << '*' << height << '=' << width * height << '\n';
      return;
    }


    Tools::write_buffer(
      [&data, &width, &height](std::ostream &ss) {
        ss << "P6\n# THIS IS A COMMENT\n" << width << " " << height << "\n255\n";
        for (const Color auto &color : data) {// organize the data in ram first then write all at once.
          ss << color.r();
          ss << color.g();
          ss << color.b();
        }
      },
      filename);
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_PPM_H
