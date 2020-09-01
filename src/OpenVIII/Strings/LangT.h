//
// Created by pcvii on 8/4/2020.
//

#ifndef VIIIARCHIVE_LANGT_H
#define VIIIARCHIVE_LANGT_H
#include <cstdint>
#include <string_view>
#include "OpenVIII/Tools/Tools.h"
namespace open_viii {
enum class LangT : std::uint8_t {
  en,
  fr,
  de,
  it,
  jp,
};

}// namespace open_viii
#endif// VIIIARCHIVE_LANGT_H
