//
// Created by pcvii on 6/29/2021.
//

#ifndef OPENVIII_CPP_WIP_PNG_HPP
#define OPENVIII_CPP_WIP_PNG_HPP
#include "open_viii/graphics/Color.hpp"
#include "png.h"
#include <cstdio>
#include <memory>
namespace open_viii::graphics {
struct Png
{
  template<Color cT, std::unsigned_integral iT>
  static void
    save(const std::vector<cT> &data,
         iT                     width,
         iT                     height,
         std::filesystem::path  filename,
         std::string            title  = "",
         std::string            prefix = "tmp") noexcept
  {
    if (width == 0U || height == 0U)
      return;
    filename = (prefix / filename.parent_path() / filename.stem()).string()
             + "_" + filename.extension().string() + ".png";
    using safe_fp = std::unique_ptr<FILE, decltype(&fclose)>;
    auto fp       = safe_fp{ fopen(filename.string().c_str(), "wb"),
                       fclose };// todo do I need fopen?

    if (!fp) {
      fprintf(stderr,
              "Could not open file %s for writing\n",
              filename.string().c_str());// todo modernize error message
      return;
    }
    // Initialize write structure
    const auto safe_png_write_struct_deleter = [](png_struct *png_s) {
      png_destroy_write_struct(&png_s, png_infopp{ NULL });
    };
    using safe_png_write_struct
      = std::unique_ptr<png_struct, decltype(safe_png_write_struct_deleter)>;
    auto png_ptr = safe_png_write_struct{
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL),
      safe_png_write_struct_deleter
    };
    if (!png_ptr) {
      fprintf(
        stderr,
        "Could not allocate write struct\n");// todo modernize error message
      return;
    }
    // Initialize info structure
    const auto safe_png_info_deleter = [&png_ptr](png_info *info) {
      png_free_data(png_ptr.get(), info, PNG_FREE_ALL, -1);
    };
    using safe_png_info
      = std::unique_ptr<png_info, decltype(safe_png_info_deleter)>;
    auto info_ptr = safe_png_info{ png_create_info_struct(png_ptr.get()),
                                   safe_png_info_deleter };
    if (!info_ptr) {
      fprintf(
        stderr,
        "Could not allocate info struct\n");// todo modernize error message
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
      static char k[] = "Title";
      png_text    title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key         = k;
      title_text.text        = title.data();
      png_set_text(png_ptr.get(), info_ptr.get(), &title_text, 1);
    }

    png_write_info(png_ptr.get(), info_ptr.get());
    const auto setRBGA = [](png_byte *const out, const cT &in) {
      out[0] = in.r();
      out[1] = in.g();
      out[2] = in.b();
      out[3] = in.a();
    };
    using safe_row                               = std::unique_ptr<png_byte[]>;
    static constexpr png_uint_32 bytes_per_pixel = 4;
    auto                         row
      = safe_row{ new png_byte[bytes_per_pixel * width * sizeof(png_byte)] };
    for (const auto y : std::ranges::iota_view(png_uint_32{ 0U }, height)) {
      for (const auto x : std::ranges::iota_view(png_uint_32{ 0U }, width)) {
        setRBGA(&row.get()[x * bytes_per_pixel], data[y * width + x]);
      }
      png_write_row(png_ptr.get(), row.get());
    }
    png_write_end(png_ptr.get(), NULL);
  }
};
}// namespace open_viii::graphics

#endif// OPENVIII_CPP_WIP_PNG_HPP
