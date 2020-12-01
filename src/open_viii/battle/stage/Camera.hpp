//
// Created by pcvii on 11/30/2020.
//

#ifndef VIIIARCHIVE_CAMERA_HPP
#define VIIIARCHIVE_CAMERA_HPP
#include "CameraAnimationCollection.hpp"
#include "CameraHeader.hpp"
#include "CameraSettings.hpp"
#include <cstdint>
#include <open_viii/tools/Tools.hpp>
#include <ostream>
#include <span>
namespace open_viii::battle::stage {
struct Camera
{
private:
  CameraHeader m_camera_header{};
  CameraSettings m_camera_settings{};
  CameraAnimationCollection m_camera_animation_collection{};

public:
  constexpr Camera() = default;
  Camera(const std::span<const char> &data)
    : m_camera_header(Tools::read_val<CameraHeader>(data)),
      m_camera_settings(Tools::read_val<CameraSettings>(data.subspan(sizeof(CameraHeader)))),
      m_camera_animation_collection(data.subspan(sizeof(CameraHeader) + sizeof(CameraSettings)))
  {
    // can be constexpr with bitcast. memcpy is not constexpr.
  }
  friend std::ostream &operator<<(std::ostream &os, const Camera &in)
  {
    return os << '{' << in.m_camera_header << ',' << in.m_camera_settings << in.m_camera_animation_collection << ','
              << "}\n";
  }
};
}// namespace open_viii::battle::stage
#endif// VIIIARCHIVE_CAMERA_HPP
