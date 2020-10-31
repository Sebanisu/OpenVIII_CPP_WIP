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
    return os << "(BinkHigh)=" << movie_clip.BinkHigh << ", (BinkLow)=" << movie_clip.BinkLow << ", (Cam)=" << movie_clip.Cam << '\n';
  }
};
}// namespace open_viii
#endif// VIIIARCHIVE_MOVIECLIP_HPP
