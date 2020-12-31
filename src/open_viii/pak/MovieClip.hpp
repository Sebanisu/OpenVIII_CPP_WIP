//
// Created by pcvii on 10/30/2020.
//

#ifndef VIIIARCHIVE_MOVIECLIP_HPP
#define VIIIARCHIVE_MOVIECLIP_HPP
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
  FileSection m_cam;

public:
  /**
   * Get High res bink video
   */
  [[nodiscard]] const FileSection &bink_high() const noexcept
  {
    return m_bink_high;
  }

  /**
   * Get Mutable High res bink video
   */
  [[nodiscard]] FileSection &mutable_bink_high() noexcept
  {
    return m_bink_high;
  }

  /**
   * Set High res bink video
   */
  void bink_high(FileSection &&in_bink_high) noexcept
  {
    m_bink_high = std::move(in_bink_high);
  }
  /**
   * Get Low res bink video
   */
  [[nodiscard]] const FileSection &bink_low() const noexcept
  {
    return m_bink_low;
  }

  /**
   * Get Mutable Low res bink video
   */
  [[nodiscard]] FileSection &mutable_bink_low() noexcept
  {
    return m_bink_low;
  }
  void swap_bink()
  {
    std::swap(m_bink_high, m_bink_low);
  }
  /**
   * Set Low res bink video
   */
  void bink_low(FileSection &&in_bink_low) noexcept
  {
    m_bink_low = std::move(in_bink_low);
  }
  /**
   * Get Cam file
   */
  [[nodiscard]] const FileSection &cam() const noexcept
  {
    return m_cam;
  }
  /**
   * Get Mutable Cam file
   */
  [[nodiscard]] FileSection &mutable_cam() noexcept
  {
    return m_cam;
  }
  /**
   * Set Cam file
   */
  void cam(FileSection &&in_cam) noexcept
  {
    m_cam = std::move(in_cam);
  }
  friend std::ostream &operator<<(std::ostream &os, const MovieClip &movie_clip)
  {
    const auto out = [&os](const FileSection &section) {
      if (section.size() > 0) {
        os << "|-\n" << section;
      }
    };
    out(movie_clip.bink_high());
    out(movie_clip.bink_low());
    out(movie_clip.cam());
    return os;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_MOVIECLIP_HPP
