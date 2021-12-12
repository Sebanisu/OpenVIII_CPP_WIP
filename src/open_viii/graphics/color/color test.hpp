//
// Created by pcvii on 7/4/2021.
//

#ifndef OPENVIII_CPP_WIP_COLOR_TEST_HPP
#define OPENVIII_CPP_WIP_COLOR_TEST_HPP

// todo make actual tests

// int
// main()
//{
//   [](auto... test) {
//     ((void)(std::cout << test << '\n'), ...);
//   }(CommonColor<Color16_impl<ColorLayoutT::RGB | ColorLayoutT::PREA>>{},
//     CommonColor<Color16_impl<ColorLayoutT::BGR | ColorLayoutT::PREA>>{},
//     CommonColor<Color16_impl<ColorLayoutT::RGB | ColorLayoutT::POSTA>>{},
//     CommonColor<Color16_impl<ColorLayoutT::BGR | ColorLayoutT::POSTA>>{},
//     CommonColor<Color16_impl<ColorLayoutT::RGB>>{},
//     CommonColor<Color16_impl<ColorLayoutT::BGR>>{},
//     CommonColor<Color24_impl<ColorLayoutT::RGB>>{},
//     CommonColor<Color24_impl<ColorLayoutT::BGR>>{},
//     CommonColor<Color32_impl<ColorLayoutT::RGB | ColorLayoutT::PREA>>{},
//     CommonColor<Color32_impl<ColorLayoutT::BGR | ColorLayoutT::PREA>>{},
//     CommonColor<Color32_impl<ColorLayoutT::RGB | ColorLayoutT::POSTA>>{},
//     CommonColor<Color32_impl<ColorLayoutT::BGR | ColorLayoutT::POSTA>>{},
//     CommonColor<Color32_impl<ColorLayoutT::BGR | ColorLayoutT::POSTA>>{
//     0xFFU,
//                                                                         0xFFU,
//                                                                         0xFFU,
//                                                                         0xFFU
//                                                                         },
//     CommonColor<Color24_impl<ColorLayoutT::BGR>>{ 0xFFU, 0xFFU, 0xFFU },
//     CommonColor<Color16_impl<ColorLayoutT::BGR>>{ 0xFFU, 0xFFU, 0xFFU },
//     CommonColor<Color16_impl<ColorLayoutT::BGRA>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U
//     }, CommonColor<Color16_impl<ColorLayoutT::BGRA>>{ 0x00U, 0x00U, 0x00U,
//     0XFFU }, CommonColor<Color16_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU,
//     0xFFU, 0X0U }, CommonColor<Color16_impl<ColorLayoutT::ARGB>>{
//       CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU,
//       0X0U } },
//     CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U
//     }, CommonColor<Color16_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU,
//     0X0U }
//       .with_r(0xCC)
//       .with_b(0xDD)
//       .with_g(0xEE)
//       .with_a(0xAA),
//     CommonColor<Color16_impl<ColorLayoutT::RGB>>{ 0xFFU, 0xFFU, 0xFFU }
//       .with_r(0xCC)
//       .with_b(0xDD)
//       .with_g(0xEE),
//     CommonColor<Color24_impl<ColorLayoutT::RGB>>{ 0xFFU, 0xFFU, 0xFFU }
//       .with_r(0xCC)
//       .with_b(0xDD)
//       .with_g(0xEE),
//     CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U
//     }
//       .with_r(0xCC)
//       .with_b(0xDD)
//       .with_g(0xEE)
//       .with_a(0xAA));
// }
template<ColorLayoutT layoutT>
using Color16 = CommonColor<Color16_impl<layoutT>>;
static_assert(
  Color32<ColorLayoutT::ARGB>{ 0xFFU, 0xFFU, 0xFFU, 0X0U }
  == CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU,
                                                    0xFFU,
                                                    0xFFU,
                                                    0X0U });

static_assert(
  CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U }
  != CommonColor<Color24_impl<ColorLayoutT::RGB>>{ 0xFFU, 0xFFU, 0xFFU });

static_assert(
  CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U }
  == Color16<ColorLayoutT::ARGB>{ 0xFFU, 0xFFU, 0xFFU, 0X0U });

static_assert(
  CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U }
  != CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xEEU,
                                                    0xFFU,
                                                    0xFFU,
                                                    0X0U });

static_assert(
  CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xEEU, 0xFFU, 0xFFU, 0X0U }
  < CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U });

static_assert(
  CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xFFU, 0xFFU, 0xFFU, 0X0U }
  > CommonColor<Color32_impl<ColorLayoutT::ARGB>>{ 0xEEU, 0xFFU, 0xFFU, 0X0U });
#endif// OPENVIII_CPP_WIP_COLOR_TEST_HPP
