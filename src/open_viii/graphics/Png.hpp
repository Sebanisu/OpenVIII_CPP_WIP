//
// Created by pcvii on 6/29/2021.
//

#ifndef OPENVIII_CPP_WIP_PNG_HPP
#define OPENVIII_CPP_WIP_PNG_HPP
#include "open_viii/graphics/Color.hpp"
namespace libpng {
#include "png.h"
}
#include <cstdio>
#include <memory>
namespace open_viii::graphics {

struct Png
{
private:
  std::filesystem::path m_filename{};
  ::libpng::png_uint_32 m_width{};
  ::libpng::png_uint_32 m_height{};
  int                   m_bit_depth{};
  int                   m_color_type{};

  using safe_fp = std::unique_ptr<FILE, decltype(&fclose)>;
  safe_fp               m_fp{ nullptr, fclose };

  static constexpr auto safe_png_read_struct_deleter
    = [](::libpng::png_struct *png_s) {
        png_destroy_read_struct(&png_s, nullptr, nullptr);
      };

  using safe_png_read_struct
    = std::unique_ptr<::libpng::png_struct,
                      decltype(safe_png_read_struct_deleter)>;

  static constexpr auto safe_png_write_struct_deleter
    = [](::libpng::png_struct *png_s) {
        png_destroy_write_struct(&png_s, ::libpng::png_infopp{ nullptr });
      };
  using safe_png_write_struct
    = std::unique_ptr<::libpng::png_struct,
                      decltype(safe_png_write_struct_deleter)>;
  template<typename deleter>
  static auto
    create_info_struct(
      const std::unique_ptr<::libpng::png_struct, deleter> &png_ptr)
  {
    // Initialize info structure
    const auto safe_png_info_deleter = [&png_ptr](::libpng::png_info *info) {
      png_free_data(png_ptr.get(), info, PNG_FREE_ALL, -1);
    };
    using safe_png_info
      = std::unique_ptr<::libpng::png_info, decltype(safe_png_info_deleter)>;
    auto info_ptr = safe_png_info{ png_create_info_struct(png_ptr.get()),
                                   safe_png_info_deleter };
    if (!info_ptr) {
      fprintf(
        stderr,
        "Could not allocate info struct\n");// todo modernize error message
    }
    return info_ptr;
  }

public:
  template<typename deleter>
  auto
    get_background_color(
      const safe_png_read_struct &                  png_ptr,
      const std::unique_ptr<::libpng::png_info, deleter> &info_ptr) const
  {
    libpng::png_color_16 *pBackground{};
    png_get_bKGD(png_ptr.get(), info_ptr.get(), &pBackground);

    uint8_t red{}, green{}, blue{};
    if (m_bit_depth == 16) {
      red   = static_cast<std::uint8_t>(pBackground->red >> 8);
      green = static_cast<std::uint8_t>(pBackground->green >> 8);
      blue  = static_cast<std::uint8_t>(pBackground->blue >> 8);
    }
    else if (m_color_type == PNG_COLOR_TYPE_GRAY && m_bit_depth < 8) {
      if (m_bit_depth == 1)
        red = green = blue = pBackground->gray ? 255 : 0;
      else if (m_bit_depth == 2) /* i.e., max value is 3 */
        red = green = blue = static_cast<std::uint8_t>((255 / 3) * pBackground->gray);
      else /* bit_depth == 4 */ /* i.e., max value is 15 */
        red = green = blue = static_cast<std::uint8_t>((255 / 15) * pBackground->gray);
    }
    else {
      red   = static_cast<std::uint8_t>(pBackground->red);
      green = static_cast<std::uint8_t>(pBackground->green);
      blue  = static_cast<std::uint8_t>(pBackground->blue);
    }
    return Color24<ColorLayoutT::RGB>{ red, green, blue };
  }
  Png(std::filesystem::path filename)
  {
    m_fp = safe_fp{ fopen(filename.string().c_str(), "wb"),
                    fclose };// todo do I need fopen?
    uint8_t sig[8]{};
    if (m_fp) {
      fread(sig, 1, sizeof(sig), m_fp.get());
      if (::libpng::png_sig_cmp(sig, 0, sizeof(sig)) != 0)
        return; /* bad signature */
    }

    // Initialize read structure
    auto png_ptr = safe_png_read_struct{ ::libpng::png_create_read_struct(
                                           PNG_LIBPNG_VER_STRING,
                                           nullptr,
                                           nullptr,
                                           nullptr),
                                         safe_png_read_struct_deleter };
    if (!png_ptr) {
      fprintf(
        stderr,
        "Could not allocate read struct\n");// todo modernize error message
      return;
    }

    // Initialize info structure
    auto info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
      return;
    }
    png_init_io(png_ptr.get(), m_fp.get());
    png_set_sig_bytes(png_ptr.get(), sizeof(sig));
    png_read_info(png_ptr.get(), info_ptr.get());
    png_get_IHDR(png_ptr.get(),
                 info_ptr.get(),
                 &m_width,
                 &m_height,
                 &m_bit_depth,
                 &m_color_type,
                 nullptr,
                 nullptr,
                 nullptr);
    [[maybe_unused]] const auto bg_color = get_background_color(png_ptr, info_ptr);
    //http://www.libpng.org/pub/png/book/chapter13.html
  }

  template<Color cT>
  static void
    save(const std::vector<cT> &data,
         ::libpng::png_uint_32  width,
         ::libpng::png_uint_32  height,
         std::filesystem::path  filename,
         std::string            title  = "",
         std::string            prefix = "tmp") noexcept
  {
    if (width == 0U || height == 0U)
      return;
    filename = (prefix / filename.parent_path() / filename.stem()).string()
             + "_" + filename.extension().string() + ".png";
    auto fp = safe_fp{ fopen(filename.string().c_str(), "wb"),
                       fclose };// todo do I need fopen?

    if (!fp) {
      fprintf(stderr,
              "Could not open file %s for writing\n",
              filename.string().c_str());// todo modernize error message
      return;
    }
    // Initialize write structure
    auto png_ptr = safe_png_write_struct{ ::libpng::png_create_write_struct(
                                            PNG_LIBPNG_VER_STRING,
                                            nullptr,
                                            nullptr,
                                            nullptr),
                                          safe_png_write_struct_deleter };
    if (!png_ptr) {
      fprintf(
        stderr,
        "Could not allocate write struct\n");// todo modernize error message
      return;
    }
    // Initialize info structure
    auto info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
      return;
    }
    //    // Setup Exception handling
    //    if (setjmp(png_jmpbuf(png_ptr.get()))) {
    //      fprintf(stderr,
    //              "Error during png creation\n");// todo modernize error
    //              message
    //      return;
    //    }
    png_init_io(png_ptr.get(), fp.get());
    // Write header (8 bit colour depth)
    png_set_IHDR(png_ptr.get(),
                 info_ptr.get(),
                 width,
                 height,
                 8,
                 PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE,
                 PNG_FILTER_TYPE_BASE);

    // Set title
    if (!title.empty()) {
      static char        k[] = "Title";
      ::libpng::png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key         = k;
      title_text.text        = title.data();
      png_set_text(png_ptr.get(), info_ptr.get(), &title_text, 1);
    }

    png_write_info(png_ptr.get(), info_ptr.get());
    const auto setRBGA = [](::libpng::png_byte *const out, const cT &in) {
      out[0] = in.r();
      out[1] = in.g();
      out[2] = in.b();
      out[3] = in.a();
    };
    using safe_row = std::unique_ptr<::libpng::png_byte[]>;
    static constexpr ::libpng::png_uint_32 bytes_per_pixel = 4;
    auto row = safe_row{ new ::libpng::png_byte[bytes_per_pixel * width
                                                * sizeof(::libpng::png_byte)] };
    for (const auto y :
         std::ranges::iota_view(::libpng::png_uint_32{ 0U }, height)) {
      for (const auto x :
           std::ranges::iota_view(::libpng::png_uint_32{ 0U }, width)) {
        setRBGA(&row.get()[x * bytes_per_pixel], data[y * width + x]);
      }
      png_write_row(png_ptr.get(), row.get());
    }
    png_write_end(png_ptr.get(), nullptr);
  }
};
}// namespace open_viii::graphics

#endif// OPENVIII_CPP_WIP_PNG_HPP
