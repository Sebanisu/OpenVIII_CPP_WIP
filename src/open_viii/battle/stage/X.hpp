//
// Created by pcvii on 11/30/2020.
//

#ifndef VIIIARCHIVE_X_HPP
#define VIIIARCHIVE_X_HPP
#include "Camera.hpp"
#include "open_viii/graphics/Tim.hpp"
#include <algorithm>
#include <array>
#include <iterator>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>
namespace open_viii::battle::stage {
/**
 * X File format
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X
 */
struct X
{
private:
  std::vector<char> m_buffer{};
  std::string m_path{};
  Camera m_camera{};
  graphics::Tim m_tim{};

  static constexpr std::array<char, 5U> TIM_START{ 0x10, 0x0, 0x0, 0x0, 0x09 };
  static constexpr std::array<char, 4U> MODEL_START{ 0x06, 0x0, 0x0, 0x0 };
  static constexpr std::array<char, 6U> CAMERA_START{ 0x02, 0x0, 0x08, 0x0, 0x20, 0x0 };

public:
  static constexpr auto EXT = std::string_view(".x");
  X() = default;
  X(std::vector<char> &&rvalue_buffer, std::string &&rvalue_path)
    : m_buffer(std::move(rvalue_buffer)), m_path(std::move(rvalue_path))
  {
    std::cout << '\n' << m_path << '\n';
    {
      const auto &buffer_begin = std::ranges::begin(m_buffer);
      const auto buffer_end = std::ranges::end(m_buffer);
      const auto search = [this, &buffer_begin, &buffer_end](const std::span<const char> &needle, auto lambda) {
        const auto start = std::search(buffer_begin, buffer_end, std::ranges::begin(needle), std::ranges::end(needle));

        const auto match = start == buffer_end;
        const auto message = [&match]() {
          if (match) {
            return "Couldn't find it.";
          }
          // return "Found it.";
          return "";
        };
        std::cout << message() << '\n';
        if (!match) {
          lambda(start);
        }
        return start;
      };

      const auto offset = [&buffer_begin](const auto &start) {
        std::cout << "\tOffset: " << std::hex << std::uppercase << std::distance(buffer_begin, start) << std::dec
                  << std::nouppercase << '\n';
      };

      std::cout << "TIM: ";
      const auto tim_start = search(TIM_START, [this, &buffer_end](const auto &start) {
        const auto &span = std::span<const char>(start, buffer_end);
        m_tim = graphics::Tim(span);
        std::cout << "\tSIZE: " << span.size() << " bytes" << std::endl;
        std::cout << "\tINFO: " << m_tim << std::endl;
      });
      offset(tim_start);

      std::cout << "MODEL: ";
      const auto model_start = search(MODEL_START, [this, &tim_start](const auto &start) {
        std::cout << "\tSIZE: " << std::span<const char>(start, tim_start).size() << " bytes" << std::endl;
      });
      offset(model_start);

      std::cout << "CAMERA: ";
      const auto camera_start = search(CAMERA_START, [this, &model_start](const auto &start) {
        auto span = std::span<const char>(start, model_start);

        std::cout << "\tSIZE: " << span.size() << " bytes" << std::endl;

        m_camera = Camera(span);
        std::cout << "\tINFO: " << m_camera << std::endl;
      });
      offset(camera_start);
      if (m_tim.check()) {
        m_tim.save(m_path);
      }
    }
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_X_HPP
