//
// Created by pcvii on 3/21/2023.
//

#ifndef OPENVIII_CPP_WIP_FIOUTITERATOR_HPP
#define OPENVIII_CPP_WIP_FIOUTITERATOR_HPP
#include "FI.hpp"
namespace open_viii::archive {
struct FIOutIterator
{
  using iterator_category = std::output_iterator_tag;
  using value_type        = FI;
  using difference_type   = std::ptrdiff_t;
  using pointer           = void;
  using reference         = void;

  explicit FIOutIterator(const std::filesystem::path &filename)
    : ofs_{ filename, std::ios::binary | std::ios::out | std::ios::trunc }
  {}

  FIOutIterator &
    operator++() noexcept
  {
    return *this;
  }
  FIOutIterator &
    operator++(int) noexcept
  {
    return *this;
  }

  FIOutIterator &
    operator*() noexcept
  {
    return *this;
  }

  FIOutIterator &
    operator=(FI fi)
  {
    ofs_.write(std::bit_cast<const char *>(&fi), sizeof(FI));
    return *this;
  }

private:
  std::ofstream ofs_;
};
/**
 * Verify that the iterator type for FIOutIterator meets the requirements of an
 * output iterator.
 */
static_assert(std::output_iterator<FIOutIterator, FI>);
}// namespace open_viii::archive
#endif// OPENVIII_CPP_WIP_FIOUTITERATOR_HPP
