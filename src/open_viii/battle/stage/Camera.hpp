//
// Created by pcvii on 11/30/2020.
//
#ifndef VIIIARCHIVE_CAMERA_HPP
#define VIIIARCHIVE_CAMERA_HPP
#include "CameraAnimationCollection.hpp"
#include "CameraHeader.hpp"
#include "CameraSettings.hpp"
#include "open_viii/tools/Tools.hpp"
namespace open_viii::battle::stage {

/**
 * @brief Represents the camera object in the battle stage, containing the
 * camera header, settings, and animation collection.
 * @see
 * https://github.com/MaKiPL/OpenVIII-monogame/blob/master/Core/Battle/Camera/Camera.cs
 * @see https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Camera_data
 */
struct Camera
{
private:
  /**
   * @brief The camera header containing basic information about the camera.
   */
  CameraHeader              m_camera_header{};

  /**
   * @brief The camera settings containing various flags and variables used in
   * the stage.
   */
  CameraSettings            m_camera_settings{};

  /**
   * @brief The camera animation collection containing all camera animations.
   */
  CameraAnimationCollection m_camera_animation_collection{};

public:
  /**
   * @brief Default constructor for Camera.
   */
  Camera() = default;

  /**
   * @brief Constructs a Camera object from a span of bytes.
   * @param data The span of bytes to parse the Camera from.
   */
  explicit Camera(const std::span<const char> &data)
    : m_camera_header(tools::read_val<CameraHeader>(data)),
      m_camera_settings(
        tools::read_val<CameraSettings>(data.subspan(sizeof(CameraHeader)))),
      m_camera_animation_collection(
        data.subspan(sizeof(CameraHeader) + sizeof(CameraSettings)))
  {
    // can be constexpr with bitcast. memcpy is not constexpr.
  }

  /**
   * @brief Get the CameraHeader object.
   * @return A reference to the CameraHeader object.
   */
  [[nodiscard]] const CameraHeader &
    camera_header() const noexcept
  {
    return m_camera_header;
  }

  /**
   * @brief Get the CameraSettings object.
   * @return A reference to the CameraSettings object.
   */
  [[nodiscard]] const CameraSettings &
    camera_settings() const noexcept
  {
    return m_camera_settings;
  }

  /**
   * @brief Get the CameraAnimationCollection object.
   * @return A reference to the CameraAnimationCollection object.
   */
  [[nodiscard]] const CameraAnimationCollection &
    camera_animation_collection() const noexcept
  {
    return m_camera_animation_collection;
  }
};

/**
 * @brief Stream output operator for Camera.
 * @param os The output stream.
 * @param in The Camera object to output.
 * @return A reference to the output stream.
 */
inline std::ostream &
  operator<<(std::ostream &os, const Camera &in)
{
  return os << "{\n\t\t HEADER: " << in.camera_header()
            << "\n\t\t SETTINGS: " << in.camera_settings()
            << "\n\t\t COLLECTION: " << in.camera_animation_collection() << "}";
}
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERA_HPP
