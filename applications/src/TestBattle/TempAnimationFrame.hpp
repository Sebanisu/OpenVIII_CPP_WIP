//
// Created by pcvii on 4/27/2023.
//

#ifndef OPENVIII_CPP_WIP_TEMPANIMATIONFRAME_HPP
#define OPENVIII_CPP_WIP_TEMPANIMATIONFRAME_HPP
#include "BitReader.hpp"
#include "open_viii/battle/dat/Section1_Skeleton.hpp"
#include "open_viii/graphics/Vertice.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numbers>
#include <stdexcept>
#include <vector>

struct Matrix
{
  float M11, M12, M13, M14;
  float M21, M22, M23, M24;
  float M31, M32, M33, M34;
  float M41, M42, M43, M44;

  Matrix()
    : M11(1), M12(0), M13(0), M14(0), M21(0), M22(1), M23(0), M24(0), M31(0),
      M32(0), M33(1), M34(0), M41(0), M42(0), M43(0), M44(1)
  {}

  Matrix(
    float m11,
    float m12,
    float m13,
    float m14,
    float m21,
    float m22,
    float m23,
    float m24,
    float m31,
    float m32,
    float m33,
    float m34,
    float m41,
    float m42,
    float m43,
    float m44)
    : M11(m11), M12(m12), M13(m13), M14(m14), M21(m21), M22(m22), M23(m23),
      M24(m24), M31(m31), M32(m32), M33(m33), M34(m34), M41(m41), M42(m42),
      M43(m43), M44(m44)
  {}

  static Matrix
    Multiply(const Matrix &a, const Matrix &b)
  {
    Matrix result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        float sum = 0;
        for (int k = 0; k < 4; ++k) {
          sum += a.Element(i, k) * b.Element(k, j);
        }
        result.Element(i, j) = sum;
      }
    }
    return result;
  }
  static Matrix
    MatrixMultiply_transpose(Matrix a, Matrix b)
  {

    return { b.M11 * a.M11 + b.M21 * a.M12 + b.M31 * a.M13,
             b.M11 * a.M21 + b.M21 * a.M22 + b.M31 * a.M23,
             b.M11 * a.M31 + b.M21 * a.M32 + b.M31 * a.M33,
             0,
             b.M12 * a.M11 + b.M22 * a.M12 + b.M32 * a.M13,
             b.M12 * a.M21 + b.M22 * a.M22 + b.M32 * a.M23,
             b.M12 * a.M31 + b.M22 * a.M32 + b.M32 * a.M33,
             0,
             b.M13 * a.M11 + b.M23 * a.M12 + b.M33 * a.M13,
             b.M13 * a.M21 + b.M23 * a.M22 + b.M33 * a.M23,
             b.M13 * a.M31 + b.M23 * a.M32 + b.M33 * a.M33,
             0,
             0,
             0,
             0,
             0 };
  }
  float &
    Element(int row, int col)
  {
    switch (row) {
    case 0:
      switch (col) {
      case 0:
        return M11;
      case 1:
        return M12;
      case 2:
        return M13;
      case 3:
        return M14;
      }
    case 1:
      switch (col) {
      case 0:
        return M21;
      case 1:
        return M22;
      case 2:
        return M23;
      case 3:
        return M24;
      }
    case 2:
      switch (col) {
      case 0:
        return M31;
      case 1:
        return M32;
      case 2:
        return M33;
      case 3:
        return M34;
      }
    case 3:
      switch (col) {
      case 0:
        return M41;
      case 1:
        return M42;
      case 2:
        return M43;
      case 3:
        return M44;
      }
    }
    throw std::out_of_range("Invalid row or column index");
  }
  float
    Element(int row, int col) const
  {
    switch (row) {
    case 0:
      switch (col) {
      case 0:
        return M11;
      case 1:
        return M12;
      case 2:
        return M13;
      case 3:
        return M14;
      }
    case 1:
      switch (col) {
      case 0:
        return M21;
      case 1:
        return M22;
      case 2:
        return M23;
      case 3:
        return M24;
      }
    case 2:
      switch (col) {
      case 0:
        return M31;
      case 1:
        return M32;
      case 2:
        return M33;
      case 3:
        return M34;
      }
    case 3:
      switch (col) {
      case 0:
        return M41;
      case 1:
        return M42;
      case 2:
        return M43;
      case 3:
        return M44;
      }
    }
    throw std::out_of_range("Invalid row or column index");
  }
};
// Convert degrees to radians
static float
  Radians(float angle)
{
  return angle * (std::numbers::pi_v<float> / 180.0f);
}

// Generate X-axis rotation matrix
inline static Matrix
  GetRotationMatrixX(float angle)
{
  return Matrix(
    1,
    0,
    0,
    0,
    0,
    std::cos(Radians(angle)),
    -std::sin(Radians(angle)),
    0,
    0,
    std::sin(Radians(angle)),
    std::cos(Radians(angle)),
    0,
    0,
    0,
    0,
    0);
}

// Generate Y-axis rotation matrix
inline static Matrix
  GetRotationMatrixY(float angle)
{
  return Matrix(
    std::cos(Radians(angle)),
    0,
    std::sin(Radians(angle)),
    0,
    0,
    1,
    0,
    0,
    -std::sin(Radians(angle)),
    0,
    std::cos(Radians(angle)),
    0,
    0,
    0,
    0,
    0);
}

// Generate Z-axis rotation matrix
inline static Matrix
  GetRotationMatrixZ(float angle)
{
  return Matrix(
    std::cos(Radians(angle)),
    -std::sin(Radians(angle)),
    0,
    0,
    std::sin(Radians(angle)),
    std::cos(Radians(angle)),
    0,
    0,
    0,
    0,
    1,
    0,
    0,
    0,
    0,
    0);
}
struct AnimationFrame
{
  static const float                               Degrees;

  std::vector<Matrix>                              bone_matrix;
  std::vector<open_viii::graphics::Vertice<float>> bones_vector_rotations;
  open_viii::graphics::Vertice<float>              position;

  AnimationFrame(
    BitReader                                       &bit_reader,
    const open_viii::battle::dat::Section1_Skeleton &skeleton,
    const AnimationFrame                            *previous = nullptr);

  static std::vector<AnimationFrame>
    create_instances(
      BitReader                                       &bit_reader,
      uint8_t                                          num_frames,
      const open_viii::battle::dat::Section1_Skeleton &skeleton);
};

const float AnimationFrame::Degrees = 360.0f;

// ... implement necessary functions like GetAdditionalRotationInformation,
// Extended, and any other helper functions ...

AnimationFrame::AnimationFrame(
  BitReader                                       &bit_reader,
  const open_viii::battle::dat::Section1_Skeleton &skeleton,
  const AnimationFrame                            *previous)
  : bone_matrix(skeleton.m_header.numBones),
    bones_vector_rotations(skeleton.m_header.numBones)
{
  auto GetAdditionalRotationInformation = [&]() {
    auto calc = [&]() {
      auto      unk1    = bit_reader.ReadBits(1);
      const int special = 1024;
      if (static_cast<uint8_t>(unk1) <= 0)
        return static_cast<int16_t>(special);
      auto unk1V = bit_reader.ReadBits(16);
      return static_cast<int16_t>(unk1V + special);
    };

    return std::make_tuple(calc(), calc(), calc());
  };

  float const x = bit_reader.ReadPositionType() * 0.01F;
  float const y = bit_reader.ReadPositionType() * 0.01F;
  float const z = bit_reader.ReadPositionType() * 0.01F;

  position = previous == nullptr ? open_viii::graphics::Vertice<float>(x, y, z)
                                 : open_viii::graphics::Vertice<float>(
                                     previous->position.x() + x,
                                     previous->position.y() + y,
                                     previous->position.z() + z);

  auto modeTest = static_cast<uint8_t>(bit_reader.ReadBits(1));

  for (size_t k = 0; k < skeleton.m_header.numBones; ++k) {
    if (previous != nullptr) {
      bones_vector_rotations[k] = open_viii::graphics::Vertice<float>{
        static_cast<float>(bit_reader.ReadRotationType()),
        static_cast<float>(bit_reader.ReadRotationType()),
        static_cast<float>(bit_reader.ReadRotationType())
      };

      if (modeTest > 0) {
        auto [unk1V, unk2V, unk3V] = GetAdditionalRotationInformation();
      }

      bones_vector_rotations[k]
        = bones_vector_rotations[k] + previous->bones_vector_rotations[k];
    }
    else {
      bones_vector_rotations[k] = open_viii::graphics::Vertice<float>{
        static_cast<float>(bit_reader.ReadRotationType()),
        static_cast<float>(bit_reader.ReadRotationType()),
        static_cast<float>(bit_reader.ReadRotationType())
      };

      if (modeTest > 0) {
        auto [unk1V, unk2V, unk3V] = GetAdditionalRotationInformation();
      }
    }

    auto boneRotation = bones_vector_rotations[k] / 4096.F;
    boneRotation      = boneRotation * Degrees;

    auto xRot         = GetRotationMatrixX(-boneRotation.x());
    auto yRot         = GetRotationMatrixY(-boneRotation.y());
    auto zRot         = GetRotationMatrixZ(-boneRotation.z());

    auto matrixZ      = Matrix::MatrixMultiply_transpose(yRot, xRot);
    matrixZ           = Matrix::MatrixMultiply_transpose(zRot, matrixZ);

    if (
      skeleton.m_bones[k].parentId
      == (std::numeric_limits<std::uint16_t>::max)()) {
      matrixZ.M41 = -position.x();
      matrixZ.M42 = -position.y();
      matrixZ.M43 = position.z();
      matrixZ.M44 = 1;
    }
    else {
      auto parentBone = bone_matrix[skeleton.m_bones[k].parentId];
      matrixZ.M43     = skeleton.m_bones[skeleton.m_bones[k].parentId].boneSize;

      auto rMatrix    = Matrix::Multiply(parentBone, matrixZ);
      rMatrix.M41 = parentBone.M11 * matrixZ.M41 + parentBone.M12 * matrixZ.M42
                  + parentBone.M13 * matrixZ.M43 + parentBone.M41;
      rMatrix.M42 = parentBone.M21 * matrixZ.M41 + parentBone.M22 * matrixZ.M42
                  + parentBone.M23 * matrixZ.M43 + parentBone.M42;
      rMatrix.M43 = parentBone.M31 * matrixZ.M41 + parentBone.M32 * matrixZ.M42
                  + parentBone.M33 * matrixZ.M43 + parentBone.M43;
      rMatrix.M44 = 1;
      matrixZ     = rMatrix;
    }

    bone_matrix[k] = matrixZ;
  }
}

std::vector<AnimationFrame>
  AnimationFrame::create_instances(
    BitReader                                       &bit_reader,
    uint8_t                                          num_frames,
    const open_viii::battle::dat::Section1_Skeleton &skeleton)
{
  std::vector<AnimationFrame> animation_frames;
  animation_frames.reserve(num_frames);

  for (size_t i = 0; i < num_frames; ++i) {
    if (i == 0) {
      animation_frames.emplace_back(bit_reader, skeleton);
    }
    else {
      animation_frames.emplace_back(
        bit_reader,
        skeleton,
        &animation_frames[i - 1]);
    }
  }

  return animation_frames;
}

#endif// OPENVIII_CPP_WIP_TEMPANIMATIONFRAME_HPP
