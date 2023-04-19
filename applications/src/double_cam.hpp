//
// Created by pcvii on 8/17/2021.
//

#ifndef OPENVIII_CPP_WIP_DOUBLE_CAM_HPP
#define OPENVIII_CPP_WIP_DOUBLE_CAM_HPP
#include "open_viii/pak/Cam.hpp"
#include "open_viii/tools/Tools.hpp"
#include <filesystem>
#include <ranges>
#include <span>
template<
  std::input_iterator       I1,
  std::sentinel_for<I1>     S1,
  std::input_iterator       I2,
  std::sentinel_for<I2>     S2,
  std::weakly_incrementable O>
void
  double_input(I1 i1, S1 s1, I2 i2, S2 s2, O o)
{
  for (; i1 != s1 && i2 != s2; (void)++i1, (void)++i2, (void)++o) {
    *o = *i1;
    ++o;
    *o = *i2;
  }
}
template<
  std::ranges::range        R1,
  std::ranges::range        R2,
  std::weakly_incrementable O>
void
  double_input(R1 &&r1, R2 &&r2, O o)
{
  double_input(r1.begin(), r1.end(), r2.begin(), r2.end(), o);
}
template<std::ranges::range R, std::weakly_incrementable O>
void
  double_input(R &&r, O o)
{
  double_input(r, r, o);
}
template<
  std::input_iterator       I1,
  std::sentinel_for<I1>     S1,
  std::input_iterator       I2,
  std::sentinel_for<I2>     S2,
  std::weakly_incrementable O,
  std::copy_constructible   F>
  requires(std::indirectly_writable<O, std::indirect_result_t<F, I1, I2>>)
void
  double_input_transform(I1 i1, S1 s1, I2 i2, S2 s2, O o, F f)
{
  for (; i1 != s1 && i2 != s2; (void)++i1, (void)++i2, (void)++o) {
    *o = *i1;
    ++o;
    *o = std::move(f(*i1, *i2));
  }
  if (i1 != s1)
    *o = *i1;
}
template<
  std::ranges::range        R1,
  std::ranges::range        R2,
  std::weakly_incrementable O,
  std::copy_constructible   F>
void
  double_input_transform(R1 &&r1, R2 &&r2, O o, F f)
{
  double_input_transform(r1.begin(), r1.end(), r2.begin(), r2.end(), o, f);
}
template<
  std::ranges::range        R,
  std::weakly_incrementable O,
  std::copy_constructible   F>
void
  double_input_transform(R &&r, O o, F f)
{
  double_input_transform(r, r | std::views::drop(1), o, f);
}
void
  process_cam(const std::filesystem::path &);

constexpr void
  interleave(std::weakly_incrementable auto o, std::ranges::range auto &&...r)
{
  constexpr auto advance_all = [](std::copy_constructible auto &&...lambdas) {
    while (std::ranges::any_of(std::array{ lambdas()... }, std::identity())) {}
  };
  advance_all([b = r.begin(), e = r.end(), &o]() mutable -> bool {
    if (b != e) {
      *o = *b;
      (void)++o, ++b;
      return true;
    }
    return false;
  }...);
}

#endif// OPENVIII_CPP_WIP_DOUBLE_CAM_HPP
