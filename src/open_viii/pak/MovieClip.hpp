//
// Created by pcvii on 10/30/2020.
//
#ifndef VIIIARCHIVE_MOVIECLIP_HPP
#define VIIIARCHIVE_MOVIECLIP_HPP
#include "Cam.hpp"
#include "FileSection.hpp"
#include <iostream>
namespace open_viii {
struct MovieClip
{
private:
  /**
   * High res bink video
   */
  FileSection m_bink_high;
  /**
   * Low res bink video
   */
  FileSection m_bink_low;
  /**
   * Cam file
   */
  Cam         m_cam;
  /**
   * Cam file file section data
   */
  FileSection m_cam_fs;

public:
  /**
   * Get High res bink video
   */
  [[nodiscard]] const FileSection &
    bink_high() const noexcept
  {
    return m_bink_high;
  }
  /**
   * Get Mutable High res bink video
   */
  [[nodiscard]] FileSection &
    mutable_bink_high() noexcept
  {
    return m_bink_high;
  }
  /**
   * Set High res bink video
   */
  void
    bink_high(FileSection &&in_bink_high) noexcept
  {
    m_bink_high = std::move(in_bink_high);
  }
  /**
   * Get Low res bink video
   */
  [[nodiscard]] const FileSection &
    bink_low() const noexcept
  {
    return m_bink_low;
  }
  /**
   * Get Mutable Low res bink video
   */
  [[nodiscard]] FileSection &
    mutable_bink_low() noexcept
  {
    return m_bink_low;
  }
  void
    swap_bink()
  {
    std::swap(m_bink_high, m_bink_low);
  }
  /**
   * Set Low res bink video
   */
  void
    bink_low(FileSection &&in_bink_low) noexcept
  {
    m_bink_low = std::move(in_bink_low);
  }
  /**
   * Get Cam file
   */
  [[nodiscard]] const auto &
    cam() const noexcept
  {
    return m_cam;
  }
  /**
   * Get Mutable Cam file
   */
  [[nodiscard]] auto &
    mutable_cam() noexcept
  {
    return m_cam;
  }
  /**
   * Set Cam file
   */
  void
    cam(Cam &&in_cam) noexcept
  {
    m_cam = std::move(in_cam);
  }
  /**
   * Get Cam file
   */
  [[nodiscard]] const auto &
    cam_fs() const noexcept
  {
    return m_cam_fs;
  }
  /**
   * Get Mutable Cam file
   */
  [[nodiscard]] auto &
    mutable_cam_fs() noexcept
  {
    return m_cam_fs;
  }
  /**
   * Set Cam file
   */
  void
    cam_fs(FileSection &&in_cam_fs) noexcept
  {
    m_cam_fs = std::move(in_cam_fs);
  }
  friend std::ostream &
    operator<<(std::ostream &os, const MovieClip &movie_clip)
  {
    const auto out = [&os](const auto &section) {
      if constexpr (has_size<decltype(section)>) {
        if (section.size() == 0) {
          return;
        }
      }
      os << "|-\n" << section;
    };
    out(movie_clip.bink_high());
    out(movie_clip.bink_low());
    out(movie_clip.cam());
    return os;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_MOVIECLIP_HPP
