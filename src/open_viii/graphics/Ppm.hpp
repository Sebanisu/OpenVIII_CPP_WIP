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
#include "Color.hpp"
#include "open_viii/Concepts.hpp"
#include "open_viii/tools/Tools.hpp"
#include "Point.hpp"
#include <execution>

namespace open_viii::graphics {
struct Ppm
{
private:
  std::filesystem::path                   m_path{};
  Point<std::uint16_t>                    m_width_height{};
  std::vector<Color24<ColorLayoutT::RGB>> m_colors{};
  Point<std::uint16_t>
    get_width_height(const std::string &buffer)
  {
    Point<std::uint16_t> point{};
    // the support is mostly limited to the same type that is made by the save
    // function. I can't use span because span_stream does not exist yet it
    // missed cpp20... so this will need to be a string stream? I know not to
    // use std::regex but i'm feeling like that's better than what I'm doing
    // here. heh.
    // sample header ss << "P6\n# THIS IS A COMMENT\n" << width << " " << height
    // << "\n255\n"; sample header ss << "P6\n# THIS IS A COMMENT\n" << width <<
    // " " << height << " 255\n";
    auto                 ss = std::stringstream(buffer);
    std::string          line{};
    std::string          type{};
    std::string          comment{};
    std::string          w_h{};
    std::string          bytesize{};
    while ((std::ranges::empty(bytesize) || std::ranges::empty(w_h))
           && std::getline(ss, line)) {
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
      if ([&line, &point, &bytesize]() -> bool {
            const auto    count = std::ranges::count(line, ' ');
            std::uint16_t width{};
            std::uint16_t height{};
            if (count == 1) {// 000 000\n000
              auto lss = std::stringstream(line);
              lss >> width;
              lss >> height;
            }
            else if (count == 2) {// 000 000 000
              auto lss = std::stringstream(line);
              lss >> width;
              lss >> height;
              lss >> bytesize;
            }
            point.x(width);
            point.y(height);
            return width > 0 || height > 0;
          }()) {
        w_h = line;
        continue;
      }
    }
    const auto start = ss.tellg();
    ss.seekg(0, std::ios::end);
    const auto end = ss.tellg();
    const auto sz  = static_cast<std::size_t>(end - start)
                  / sizeof(Color24<ColorLayoutT::RGB>);
    if (sz != point.area()) {
      std::cerr << m_path << "\n\t" << sz << " != area of " << point
                << std::endl;
      // assert(sz == m_width_height.area());
      return {};// instead of throwing reset the value and quit.
    }
    return point;
  }
  auto
    get_colors(std::span<const char> buffer_span)
  {
    std::vector<Color24<ColorLayoutT::RGB>> colors{};
    const auto                              area = m_width_height.area();
    static constexpr auto color_size    = sizeof(Color24<ColorLayoutT::RGB>);
    const auto            size_of_bytes = area * color_size;
    if (area > 0 && std::ranges::size(buffer_span) > size_of_bytes) {
      buffer_span = buffer_span.subspan(
        std::ranges::size(buffer_span) - size_of_bytes,
        size_of_bytes);
      colors.resize(area);
      std::memcpy(
        std::ranges::data(colors),
        std::ranges::data(buffer_span),
        size_of_bytes);
    }
    return colors;
  }

public:
  Ppm() = default;
  explicit Ppm(const std::filesystem::path &path)
    : Ppm(tools::read_entire_file<std::string>(path), path)
  {}
  explicit Ppm(const std::string &buffer, std::filesystem::path path = {})
    : m_path(std::move(path)), m_width_height(get_width_height(buffer)),
      m_colors(get_colors(buffer))
  {}
  bool
    empty() const noexcept
  {
    return std::ranges::empty(m_colors) || m_width_height.x() <= 0
        || m_width_height.y() <= 0;
  }
  template<std::ranges::contiguous_range cT>
  static void
    save(
      const cT               &data,
      std::size_t             width,
      std::size_t             height,
      const std::string_view &input,
      bool                    skip_check = false)
  {// how do i make the concept reject ranges that aren't of Colors? I'm at
   // least checking for Color down below.
    if (!skip_check) {
      bool are_colors_all_black = check_if_colors_are_black(data);
      if (
        width == 0 || height == 0 || std::ranges::empty(data)
        || are_colors_all_black) {
        return;
      }
    }
    const std::string filename = [&]() {
      if (!open_viii::tools::i_ends_with(input, ".ppm")) {
        auto        tmp = std::filesystem::path(input);
        std::string local_filename{ (tmp.parent_path() / tmp.stem()).string() };
        if (tmp.has_extension()) {
          local_filename += "_" + tmp.extension().string().substr(1);
        }
        local_filename += ".ppm";
        return local_filename;
      }
      return std::string{ input };
    }();
    if (std::ranges::size(data) < width * height) {
      std::cout << std::ranges::size(data) << ", " << width << '*' << height
                << '=' << width * height << '\n';
      return;
    }
    tools::write_buffer(
      [&data, &width, &height](std::ostream &ss) {
        ss << "P6\n# THIS IS A COMMENT\n"
           << width << " " << height << "\n255\n";
        for (const Color auto &color :
             data) {// organize the data in ram first then write all at once.
          ss << color.r();
          ss << color.g();
          ss << color.b();
        }
      },
      filename);
  }
  static bool
    check_if_colors_are_black(const auto &data)
  {
    return std::all_of(
      std::execution::par_unseq,
      data.begin(),
      data.end(),
      [](const Color auto &color) -> bool {
        return color.a() == 0U
            || (color.b() == 0U && color.g() == 0U && color.r() == 0U);
      });
  }
  [[nodiscard]] const std::vector<Color24<ColorLayoutT::RGB>> &
    colors() const noexcept
  {
    return m_colors;
  }
  [[nodiscard]] constexpr auto
    width() const noexcept
  {
    return m_width_height.x();
  }
  [[nodiscard]] constexpr auto
    height() const noexcept
  {
    return m_width_height.y();
  }
  [[nodiscard]] const auto &
    width_height() const noexcept
  {
    return m_width_height;
  }
  [[nodiscard]] const Color24<ColorLayoutT::RGB> &
    color(const std::size_t &x, const std::size_t &y) const noexcept
  {
    return at(x + (y * static_cast<std::size_t>(m_width_height.x())));
  }
  [[nodiscard]] const Color24<ColorLayoutT::RGB> &
    at(const size_t i) const noexcept
  {
    static constexpr Color24<ColorLayoutT::RGB> black{};
    if (i < std::ranges::size(m_colors)) {
      return m_colors[i];
    }
    return black;
  }
  [[nodiscard]] const std::filesystem::path &
    path() const noexcept
  {
    return m_path;
  }
};
inline std::ostream &
  operator<<(std::ostream &os, const Ppm &ppm)
{
  return os << "(Width, Height): " << ppm.width_height() << "\t" << ppm.path()
            << '\n';
}
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_PPM_HPP
