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
struct Camera
{
private:
  CameraHeader              m_camera_header{};
  CameraSettings            m_camera_settings{};
  CameraAnimationCollection m_camera_animation_collection{};

public:
  Camera() = default;
  explicit Camera(const std::span<const char> &data)
    : m_camera_header(tools::read_val<CameraHeader>(data)),
      m_camera_settings(
        tools::read_val<CameraSettings>(data.subspan(sizeof(CameraHeader)))),
      m_camera_animation_collection(
        data.subspan(sizeof(CameraHeader) + sizeof(CameraSettings)))
  {
    // can be constexpr with bitcast. memcpy is not constexpr.
  }
  [[nodiscard]] const CameraHeader &
    camera_header() const noexcept
  {
    return m_camera_header;
  }
  [[nodiscard]] const CameraSettings &
    camera_settings() const noexcept
  {
    return m_camera_settings;
  }
  [[nodiscard]] const CameraAnimationCollection &
    camera_animation_collection() const noexcept
  {
    return m_camera_animation_collection;
  }
};
inline std::ostream &
  operator<<(std::ostream &os, const Camera &in)
{
  return os << "{\n\t\t HEADER: " << in.camera_header()
            << "\n\t\t SETTINGS: " << in.camera_settings()
            << "\n\t\t COLLECTION: " << in.camera_animation_collection() << "}";
}
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERA_HPP
