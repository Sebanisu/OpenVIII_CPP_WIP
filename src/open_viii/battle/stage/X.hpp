//
// Created by pcvii on 11/30/2020.
//
#ifndef VIIIARCHIVE_X_HPP
#define VIIIARCHIVE_X_HPP
#include "Camera.hpp"
#include "Geometries.hpp"
#include "GeometryGroupPointers.hpp"
#include "GeometryPointers.hpp"
#include "open_viii/graphics/Tim.hpp"
namespace open_viii::battle::stage {
/**
 * @brief Represents the X file format for battle stages in Final Fantasy VIII.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X
 */
struct X
{
private:
  std::vector<char> m_buffer{};///< @brief Buffer holding the file content.
  std::string       m_path{};  ///< @brief Path to the X file.
  Camera            m_camera{};///< @brief Camera object from the X file.
  std::vector<Geometries>
                m_geometries{};///< @brief Geometries object from the X file.
  graphics::Tim m_tim{};       ///< @brief TIM texture from the X file.

  /**
   * @brief Searches for a needle in the buffer and executes a lambda function
   * if found.
   * @param needle The needle to search for in the buffer.
   * @param buffer_begin Pointer to the beginning of the buffer.
   * @param buffer_end Pointer to the end of the buffer.
   * @param lambda Lambda function to execute if the needle is found.
   * @return Pointer to the start of the needle if found, or buffer_end if not
   * found.
   */
  auto
    search(
      const std::span<const char> &needle,
      const char                  *buffer_begin,
      const char                  *buffer_end,
      auto                         lambda)
  {
    const auto start = std::search(
      buffer_begin,
      buffer_end,
      std::ranges::begin(needle),
      std::ranges::end(needle));
    const auto no_match = start == buffer_end;
    using ret_t         = std::optional<decltype(start)>;
    if (!no_match) {
      lambda(start);
      return ret_t{ start };
    }
    else {
      std::cout << std::flush;
      std::cerr << "Couldn't find camera start." << '\n';
      return ret_t{};
    }
  }

  /**
   * @brief Prints the offset of the start pointer relative to the buffer_begin
   * pointer.
   * @param buffer_begin Pointer to the beginning of the buffer.
   * @param start Pointer to the start position to calculate the offset.
   */
  void
    offset(const char *buffer_begin, const char *start)
  {
    std::cout << "\tOffset: " << std::hex << std::uppercase
              << std::distance(buffer_begin, start) << std::dec
              << std::nouppercase
              << '\n';// base() isn't in visual studio so i might need to
                      // cast buffer_begin to a span so they are the same
                      // type. base() converts the iterator to the pointer.
  };

  /**
   * @brief Processes the TIM data from the buffer.
   * @param buffer_begin Pointer to the beginning of the buffer.
   * @param buffer_end Pointer to the end of the buffer.
   */
  void
    process_tim(const char *buffer_begin, const char *buffer_end)
  {
    puts("TIM");
    const auto tim_start = search(
      TIM_START,
      buffer_begin,
      buffer_end,
      [this, &buffer_end](const auto &start) {
        const auto &span = std::span<const char>(start, buffer_end);
        m_tim            = graphics::Tim(span);
        std::cout << "\tSIZE: " << span.size() << " bytes" << std::endl;
        std::cout << "\tINFO: " << m_tim << std::endl;
      });
    if (tim_start.has_value())
      offset(buffer_begin, tim_start.value());
  }

  /**
   * @brief Processes the camera data from the buffer.
   * @param buffer_begin Pointer to the beginning of the buffer.
   * @param tim_start Pointer to the start of the TIM data.
   */
  const char *
    process_camera(const char *buffer_begin, const char *tim_start)
  {
    const auto generate = [this, &tim_start](const auto &start) {
      auto span = std::span<const char>(start, tim_start);
      m_camera  = Camera(span);
      std::cout << "\tINFO: " << m_camera << std::endl;
    };
    puts("CAMERA1");
    const auto camera_start
      = search(CAMERA_START_ARR[0], buffer_begin, tim_start, generate);
    if (camera_start.has_value()) {
      offset(buffer_begin, camera_start.value());
      puts("CAMERA2");
      const auto camera_start2 = search(
        CAMERA_START_ARR[1],
        camera_start.value(),
        tim_start,
        generate);
      if (camera_start2.has_value()) {
        offset(buffer_begin, camera_start2.value());
        return camera_start2.value();
      }
      return camera_start.value();
    }
    return nullptr;
  }

  /**
   * @brief Processes the model data from the buffer.
   * @param buffer_begin Pointer to the beginning of the buffer.
   * @param tim_start Pointer to the start of the TIM data.
   * @param camera_start Pointer to the start of the camera data.
   */
  int
    process_model(
      const char *buffer_begin,
      const char *tim_start,
      const char *camera_start)
  {
    puts("MODEL");
    const auto camera_size = m_camera.camera_header().camera_data_size();
    const auto model_start = camera_start + camera_size;
    assert(model_start < tim_start);
    const auto size = std::distance(model_start, tim_start);
    std::cout << "\tSIZE: " << size << " bytes" << std::endl;
    offset(buffer_begin, model_start);
    auto span
      = std::span<const char>{ model_start, static_cast<std::size_t>(size) };
    {
      std::array<char, sizeof(std::uint32_t)> tmp{};
      std::ranges::copy(span.subspan(0, sizeof(std::uint32_t)), tmp.begin());
      [[maybe_unused]] const auto section_counter
        = std::bit_cast<std::uint32_t>(tmp);
      span = span.subspan(sizeof(std::uint32_t));
      if (section_counter != 6) {
        std::cout << std::flush;
        std::cerr << "Error in " << __FILE__ << ":" << __LINE__
                  << ": section_counter is " << section_counter
                  << ". We're probably reading the wrong offset or this file "
                     "is in a different format.\n";
        return 1;// Return with an error code
      }
    }
    GeometryPointers geometry_pointers;
    {
      std::array<char, sizeof(GeometryOffsets)> tmp{};
      std::ranges::copy(span.subspan(0, sizeof(GeometryOffsets)), tmp.begin());
      geometry_pointers
        = std::bit_cast<GeometryOffsets>(tmp) + (span.data() - 4U);
      // span = span.subspan(sizeof(GeometryOffsets));
    }
    std::array<GeometryGroupPointers, 4> group_pointers{};
    for (std::uint8_t i{};
         const auto  *ptr : std::array{ geometry_pointers.group_1_pointer,
                                       geometry_pointers.group_2_pointer,
                                       geometry_pointers.group_3_pointer,
                                       geometry_pointers.group_4_pointer }) {
      std::array<char, sizeof(GeometryGroupOffsets)> tmp{};
      std::ranges::copy(
        std::span(ptr, sizeof(GeometryGroupOffsets)),
        tmp.begin());
      group_pointers[i++] = std::bit_cast<GeometryGroupOffsets>(tmp) + ptr;
      // span = span.subspan(sizeof(GeometryOffsets));
    }
    m_geometries.reserve(4);
    for (const auto &group_pointer : group_pointers) {
      m_geometries.emplace_back(
        buffer_begin,
        std::span(group_pointer.object_list_pointer, tim_start));
    }
    return 0;
  }

public:
  /// @brief MIPS start marker.
  static constexpr auto MIPS_START
    = std::span{ "\xE8\xFF\xBD\x27\x01\x00\x02", 7 };
  /// @brief TIM start marker.
  static constexpr auto TIM_START   = std::span{ "\x10\x00\x00\x00\x09", 5 };
  /// @brief Model start marker.
  static constexpr auto MODEL_START = std::span{ "\x06\x00\x00\x00", 4 };
  /// @brief Camera start marker.
  static constexpr auto CAMERA_START
    = std::span{ "\x02\x00\x08\x00\x20\x00", 6 };

  /**
   * @brief A constexpr array containing two spans representing the starting
   * sequence of the camera data.
   */
  static constexpr auto CAMERA_START_ARR
    = std::array{ std::span{ "\x02\x00\x08\x00\x20\x00", 6 },
                  std::span{ "\x02\x00\x08\x00\x18\x00", 6 } };
  static constexpr auto EXT
    = std::string_view(".x");///< @brief File extension for the X file format.

  /**
   * @brief Default constructor.
   */
  X() = default;

  /**
   * @brief Construct an X object by taking ownership of the given buffer and
   * path.
   * @param rvalue_buffer Buffer holding the file content.
   * @param rvalue_path Path to the X file.
   */
  X(std::vector<char> &&rvalue_buffer, std::string &&rvalue_path)
    : m_buffer(std::move(rvalue_buffer)), m_path(std::move(rvalue_path))
  {
    std::cout << '\n' << m_path << '\n';

    const char *buffer_begin = std::ranges::data(m_buffer);
    const char *buffer_end
      = std::ranges::data(m_buffer)
      + std::distance(std::ranges::begin(m_buffer), std::ranges::end(m_buffer));

    process_tim(buffer_begin, buffer_end);
    const auto tim_start = std::ranges::find_end(
      buffer_begin,
      buffer_end,
      TIM_START.begin(),
      TIM_START.end());
    const char *camera_start = process_camera(buffer_begin, tim_start.data());
    if (camera_start != nullptr)
      process_model(buffer_begin, tim_start.begin(), camera_start);
  }
  const std::vector<Geometries> &
    geometries() const
  {
    return m_geometries;
  }

  /**
   * @brief Get the path to the X file.
   * @return const std::string& The path to the X file.
   */
  const std::string &
    path() const
  {
    return m_path;
  }

  /**
   * @brief Get the Camera object from the X file.
   * @return const Camera& The Camera object from the X file.
   */
  const Camera &
    camera() const
  {
    return m_camera;
  }
  /**
   * @brief Get the TIM texture from the X file.
   * @return const graphics::Tim& The TIM texture from the X file.
   */
  const graphics::Tim &
    tim() const
  {
    return m_tim;
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_X_HPP
