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
public:
  /**
   * High res bink video
   */
  FileSection BinkHigh;
  /**
   * Low res bink video
   */
  FileSection BinkLow;

  /**
   * Cam file
   */

  FileSection Cam;
  friend std::ostream &operator<<(std::ostream &os, const MovieClip &movie_clip)
  {
    const auto out = [&os](const FileSection &section) {
      if (section.Size > 0) {
        os << "|-\n" << section;
      }
    };
    out(movie_clip.Cam);
    out(movie_clip.BinkHigh);
    out(movie_clip.BinkLow);
    return os;
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_MOVIECLIP_HPP
