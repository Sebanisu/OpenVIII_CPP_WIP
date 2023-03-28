//
// Created by pcvii on 11/18/2020.
//
#ifndef VIIIARCHIVE_CAMERAHEADER_HPP
#define VIIIARCHIVE_CAMERAHEADER_HPP
namespace open_viii::battle::stage {
/**
 * @brief Represents the header for the camera data of a battle stage.
 * @see http://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_data
 */
struct CameraHeader
{
private:
  /**
   * @brief The number of offsets in the header.
   */
  std::uint16_t                  m_offset_count{};

  /**
   * @brief The offset to the camera settings data.
   */
  std::uint16_t                  m_offset_camera_settings{};

  /**
   * @brief The offset to the camera animation collection data.
   */
  std::uint16_t                  m_offset_camera_animation_collection{};

  /**
   * @brief The size of the camera data.
   */
  std::uint16_t                  m_camera_data_size{};

  /**
   * @brief The expected number of offsets in the header.
   */
  static constexpr std::uint16_t EXPECTED_OFFSET_COUNT{ 0x2U };

  /**
   * @brief The expected offset to the camera settings data.
   */
  static constexpr std::uint16_t EXPECTED_OFFSET_CAMERA_SETTINGS{ 0x8U };

  /**
   * @brief The expected offset to the camera animation collection data.
   */
  static constexpr std::uint16_t EXPECTED_OFFSET_CAMERA_ANIMATION_COLLECTION{
    0x8U
  };

public:

  /**
   * @brief Check if the header fields have expected values.
   * @return true if all fields have expected values, false otherwise.
   */
  [[nodiscard]] bool constexpr check() const noexcept
  {
    return m_offset_count == EXPECTED_OFFSET_COUNT
        && m_offset_camera_settings == EXPECTED_OFFSET_CAMERA_SETTINGS
        && m_offset_camera_animation_collection
             == EXPECTED_OFFSET_CAMERA_ANIMATION_COLLECTION;
  }

  /**
   * @brief Get the number of offsets in the header.
   * @return The number of offsets as a 16-bit unsigned integer.
   */
  [[nodiscard]] constexpr std::uint16_t
    offset_count() const noexcept
  {
    return m_offset_count;
  }

  /**
   * @brief Get the offset to the camera settings data.
   * @return The offset to the camera settings as a 16-bit unsigned integer.
   */
  [[nodiscard]] constexpr std::uint16_t
    offset_camera_settings() const noexcept
  {
    return m_offset_camera_settings;
  }

  /**
   * @brief Get the offset to the camera animation collection data.
   * @return The offset to the camera animation collection as a 16-bit unsigned
   * integer.
   */
  [[nodiscard]] constexpr std::uint16_t
    offset_camera_animation_collection() const noexcept
  {
    return m_offset_camera_animation_collection;
  }

  /**
   * @brief Get the size of the camera data.
   * @return The size of the camera data as a 16-bit unsigned integer.
   */
  [[nodiscard]] constexpr std::uint16_t
    camera_data_size() const noexcept
  {
    return m_camera_data_size;
  }
};

/**
 * @brief Output stream operator for CameraHeader.
 * @param os The output stream to write the CameraHeader to.
 * @param in The CameraHeader to write to the output stream.
 * @return A reference to the output stream.
 */
inline std::ostream &
  operator<<(std::ostream &os, const CameraHeader &in)
{
  return os << "{\n\t\t\tOFFSET COUNT: " << in.offset_count()
            << "\n\t\t\tSETTINGS OFFSET: 0x" << std::hex << std::uppercase
            << in.offset_camera_settings() << std::dec << std::nouppercase
            << "\n\t\t\tCOLLECTION OFFSET: 0x" << std::hex << std::uppercase
            << in.offset_camera_animation_collection() << std::dec
            << std::nouppercase
            << "\n\t\t\tDATA SIZE: " << in.camera_data_size() << "}";
}
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERAHEADER_HPP
