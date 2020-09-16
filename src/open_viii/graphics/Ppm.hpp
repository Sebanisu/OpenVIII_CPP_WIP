// This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef VIIIARCHIVE_PPM_HPP
#define VIIIARCHIVE_PPM_HPP
#include <cstdint>
#include <fstream>
#include <iostream>
#include <cctype>
#include <string_view>
#include "Color.hpp"
#include "open_viii/Concepts.hpp"
namespace open_viii::graphics {
struct Ppm
{
private:
  std::vector<Color24<0, 1, 2>> m_colors{};

  std::uint16_t m_width{};
  std::uint16_t m_height{};

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
    std::string bytesize{};
    while ((std::ranges::empty(bytesize) || std::ranges::empty(w_h)) && std::getline(ss, line)) {
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
      if ([&line, this, &w_h]() -> bool {
            auto lss = std::stringstream(line);
            lss >> m_width;
            lss >> m_height;
            return m_width > 0 || m_height > 0;
          }()) {
        w_h = line;
        continue;
      }
    }

    auto bufferspan = (std::span<const char>(buffer)).subspan(static_cast<std::size_t>(ss.tellg()));
    if (std::ranges::size(bufferspan) > 0) {
      auto sz = std::ranges::size(bufferspan) / sizeof(Color24<0, 1, 2>);
      const auto colorspan =
        std::span<const Color24<0, 1, 2>>{ reinterpret_cast<const Color24<0, 1, 2> *>(std::ranges::data(bufferspan)),
          sz };
      m_colors = { std::ranges::cbegin(colorspan), std::ranges::cend(colorspan) };
      assert(std::ranges::size(m_colors) == m_height*m_width);
    }
  }
  [[nodiscard]] const auto &colors() { return m_colors; }
  template<std::ranges::contiguous_range cT>
  static void
    save(const cT &data, std::size_t width, std::size_t height, const std::string_view &input, bool skip_check = false)
  {// how do i make the concept reject ranges that aren't of Colors? I'm at least checking for Color down below.
    if (!skip_check) {
      if (width == 0 || height == 0 || std::ranges::empty(data)
          || std::all_of(std::execution::par_unseq, data.begin(), data.end(), [](const Color auto &color) -> bool {
               return color.a() == 0U || (color.b() == 0U && color.g() == 0U && color.r() == 0U);
             })) {
        return;
      }
    }

    auto tmp = std::filesystem::path(input);

    std::string filename{ (tmp.parent_path() / tmp.stem()).string() };
    if (tmp.has_extension())
      filename += "_" + tmp.extension().string().substr(1);
    filename += ".ppm";
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

  [[nodiscard]] const std::vector<Color24<0, 1, 2>> &colors() const { return m_colors; }
  [[nodiscard]] const std::uint16_t &width() const { return m_width; }
  [[nodiscard]] const std::uint16_t &height() const { return m_height; }
  [[nodiscard]] const Color24<0, 1, 2> &color(const std::size_t &x, const std::size_t &y) const
  {
    return m_colors.at(x + (y * static_cast<std::size_t>(m_width)));
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_PPM_HPP
