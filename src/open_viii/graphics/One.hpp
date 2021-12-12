//
// Created by pcvii on 2/18/2021.
//
#ifndef VIIIARCHIVE_ONE_HPP
#define VIIIARCHIVE_ONE_HPP
#include "Mch.hpp"
#include "one/OneModel.hpp"
namespace open_viii::graphics {
/**
 * @note world\esk\chara.one has 16 tim files
 */
struct One
{
private:
  std::vector<char>                 m_buffer{};
  std::uint32_t                     m_count{};
  static constexpr auto             SIZE_TIM_START = 5U;
  /**
   * 4 bit color tim.
   */
  static constexpr std::string_view TIM4_START{ "\x10\x00\x00\x00\x08",
                                                SIZE_TIM_START };
  /**
   * 8 bit color tim.
   */
  static constexpr std::string_view TIM8_START{ "\x10\x00\x00\x00\x09",
                                                SIZE_TIM_START };
  /**
   * 16 bit color tim.
   */
  static constexpr std::string_view TIM16_START{ "\x10\x00\x00\x00\x02",
                                                 SIZE_TIM_START };
  /**
   * 24 bit color tim.
   */
  static constexpr std::string_view TIM24_START{ "\x10\x00\x00\x00\x03",
                                                 SIZE_TIM_START };
  static_assert(std::ranges::size(TIM4_START) == SIZE_TIM_START);
  static_assert(std::ranges::size(TIM8_START) == SIZE_TIM_START);
  static_assert(std::ranges::size(TIM16_START) == SIZE_TIM_START);
  static_assert(std::ranges::size(TIM24_START) == SIZE_TIM_START);
  static constexpr std::array TIM_STARTS{ TIM4_START,
                                          TIM8_START,
                                          TIM16_START,
                                          TIM24_START };
  auto
    get_next_offset(const std::span<const char> &buffer) const
  {
    // could be a coroutine using std::generator
    // because these tim can be in any order.
    // todo. maybe have a way to pop items from the TIM_STARTS that don't
    // have at least 1 match.
    return (std::ranges::min)(
      TIM_STARTS | std::ranges::views::transform([&buffer](const auto &start) {
        return tools::search(buffer, start);
      }));
  }
  static Tim
    get_tim(auto offset, std::span<const char> &buffer)
  {
    buffer        = std::span<const char>(offset, std::ranges::end(buffer));
    const Tim tim = Tim(buffer);
    buffer        = buffer.subspan(SIZE_TIM_START);
    return tim;
  }

public:
  One(std::vector<char> &&buffer)
    : m_buffer(std::move(buffer)),
      m_count(tools::read_val<std::uint32_t>(m_buffer))
  {}
  bool
    save_tim(const std::string &path, const size_t i, const Tim &tim) const
  {
    if (tim) {
      auto path_parts = std::filesystem::path(path);
      const auto out_path   = (path_parts.parent_path()
                             / (path_parts.stem().string() + std::to_string(i)
                                + path_parts.extension().string()))
                              .string();
      tim.save(out_path);
      return true;
    }
    return false;
  }
  void
    save([[maybe_unused]] const std::string &path) const
  {
    // todo make this generic, extract tim file code to a free function.
    size_t i{};
    auto   buffer = std::span<const char>(m_buffer);
    while (true) {
      const auto offset = get_next_offset(buffer);
      if (offset == std::ranges::end(buffer)) {
        break;
      }
      if (save_tim(path, i, get_tim(offset, buffer))) {
        ++i;
      }
    }
  }
};
}// namespace open_viii::graphics
#endif// VIIIARCHIVE_ONE_HPP
