//
// Created by pcvii on 1/11/2021.
//
#ifndef VIIIARCHIVE_RANDOMIOTA_HPP
#define VIIIARCHIVE_RANDOMIOTA_HPP
#include <iterator>
#include <random>
template<> struct RandomIOTAInterator
{
  using iterator_category = std::forward_iterator_tag;
  using difference_type   = std::ptrdiff_t;
  using value_type        = std::size_t;
  using pointer           = value_type *;// or also value_type*
  using reference         = value_type &;// or also value_type&
  explicit RandomIOTA(std::size_t current) : gen(rd()), m_current(size) {}
  reference operator*() const
  {
    return m_current;
  }
  pointer operator->()
  {
    return &m_current;
  }
  Iterator &operator++()
  {
    ++m_current;
    m_random_value = get_random();
    return *this;
  }
  Iterator operator++(int)
  {
    Iterator tmp = *this;
    ++(*this);
    return tmp;
  }
  friend bool operator==(const Iterator &a, const Iterator &b)
  {
    return a.m_current == b.m_current;
  };
  friend bool operator!=(const Iterator &a, const Iterator &b)
  {
    return a.m_current != b.m_current;
  };
private:
  auto get_random()
  {
    return dis(gen);
  }
  std::random_device                        rd{};
  std::mt19937                              gen;
  std::uniform_int_distribution<value_type> dis{};
  value_type                                m_current {}
  value_type                                m_random_value{};
};
struct RandomIOTA
{
public:
  using value_type = std::size_t;
  explicit RandomIOTA(value_type size) : m_size(size) {}
  // Iterator definition here ...
  RandomIOTAInterator begin()
  {
    return RandomIOTAInterator(0);
  }
  RandomIOTAInterator end()
  {
    return RandomIOTAInterator(m_size);
  }// 200 is out of bounds
private:
  value_type m_size{};
};
#endif// VIIIARCHIVE_RANDOMIOTA_HPP
