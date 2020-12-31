//
// Created by pcvii on 12/27/2020.
//

#ifndef VIIIARCHIVE_CAM_HPP
#define VIIIARCHIVE_CAM_HPP
#include "CamFrame.hpp"
#include "CamHeader.hpp"
#include "open_viii/tools/Tools.hpp"
#include <istream>
#include <vector>
namespace open_viii {
/**
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_PAK#CAM_files
 */
struct Cam
{
private:
  CamHeader m_header{};
  std::vector<CamFrame> m_frames{};

public:
  Cam() = default;
  Cam(std::istream &is, const std::size_t &m_frame_count)
  : m_header(Tools::read_val<m_header>(is))
  {
    m_frames.resize(m_frame_count);
    Tools::read_val(is,m_frames);
  }
  explicit Cam(std::istream &is)
    : Cam(is, [&is]() {
        const std::size_t cur = is.tellg();
        is.seekg(0, std::ios::end);
        const std::size_t sz = is.tellg() - cur;
        is.seekg(cur, std::ios::beg);
        return sz / sizeof(CamFrame);
      }())
  {}
};
}// namespace open_viii
#endif// VIIIARCHIVE_CAM_HPP
