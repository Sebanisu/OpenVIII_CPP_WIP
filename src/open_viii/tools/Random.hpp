//
// Created by pcvii on 1/11/2021.
//
#ifndef VIIIARCHIVE_RANDOM_HPP
#define VIIIARCHIVE_RANDOM_HPP
#include "Make_Container.hpp"
#include <atomic>
#include <iostream>
#include <random>
#include <ranges>
#include <vector>
namespace open_viii::tools {
template<typename lambdaT>
static void random_iota(const lambdaT &callback, const std::atomic_bool &stop)
{
  // test LZSS
  static std::random_device                           rd{};
  static std::mt19937                                 gen(rd());
  static std::uniform_int_distribution<unsigned char> dis(0U);
  size_t                                       i{ 0 };
  static const auto run_once = [&callback](const size_t &size) {
    return callback(make_container<std::vector<char>>(
      [] {
        return dis(gen);
      },
      size));
  };
  while (!stop && run_once(++i)) {}// inplace lambda
}
}// namespace open_viii::tools
#endif// VIIIARCHIVE_RANDOM_HPP
