//
// Created by pcvii on 12/27/2020.
//
#ifndef VIIIARCHIVE_CAM_HPP
#define VIIIARCHIVE_CAM_HPP
#include "CamFrame.hpp"
#include "CamHeader.hpp"
#include "FileSection.hpp"
#include "open_viii/tools/Tools.hpp"
#include <istream>
#include <ostream>
#include <ranges>
#include <vector>
namespace open_viii {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_PAK#CAM_files
 */
struct Cam
{
private:
  CamHeader             m_header{};
  std::vector<CamFrame> m_frames{};

public:
  Cam() = default;
  Cam(std::istream &is, const std::size_t &m_frame_count)
    : m_header(tools::read_val<CamHeader>(is))
  {
    m_frames.resize(m_frame_count);
    tools::read_val(is, m_frames);
  }
  Cam(std::istream &is, const FileSection &fs)
    : Cam(
      [&fs](std::istream &in_is) -> std::istream & {
        in_is.seekg(fs.offset());
        return in_is;
      }(is),
      fs.frames())
  {}
  explicit Cam(std::istream &is)
    : Cam(is, [&is]() {
        const auto cur = is.tellg();
        is.seekg(0, std::ios::end);
        const auto sz = is.tellg() - cur;
        is.seekg(cur, std::ios::beg);
        return static_cast<std::size_t>(sz) / sizeof(CamFrame);
      }())
  {}
  friend std::ostream &operator<<(std::ostream &os, const Cam &cam)
  {
    os << cam.m_header << '\n';
    os << '{';
    std::ranges::for_each(cam.m_frames,
                          [&os, not_first = false](const auto &frame) mutable {
                            if (!not_first) {
                              not_first = true;
                              os << ',';
                            }
                            os << frame << '\n';
                          });
    os << '}';
    return os;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_CAM_HPP
