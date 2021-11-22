//
// Created by pcvii on 6/29/2021.
//

#ifndef OPENVIII_CPP_WIP_PNG_HPP
#define OPENVIII_CPP_WIP_PNG_HPP
#include "open_viii/graphics/Color.hpp"
#include "open_viii/tools/Tools.hpp"
namespace libpng {
#include "png.h"
}// namespace libpng
#include <cstdio>
#include <memory>
namespace open_viii::graphics {

struct Png
{
private:
  std::filesystem::path                    m_filename{};
  ::libpng::png_uint_32                    m_width{};
  ::libpng::png_uint_32                    m_height{};
  int                                      m_bit_depth{};
  int                                      m_color_type{};
  std::size_t                              m_row_bytes{};
  ::libpng::png_byte                       m_channels{};
  double                                   m_gamma{};
  std::vector<Color32<ColorLayoutT::RGBA>> m_color{};
  Color32<ColorLayoutT::RGBA>              m_background_color{};
  static constexpr ::libpng::png_uint_32   bytes_per_pixel = 4;

  using safe_fp = std::unique_ptr<FILE, decltype(&fclose)>;

  static constexpr auto safe_png_read_struct_deleter
    = [](::libpng::png_struct *png_s) {
        png_destroy_read_struct(&png_s, nullptr, nullptr);
      };

  using safe_png_read_struct = std::
    unique_ptr<::libpng::png_struct, decltype(safe_png_read_struct_deleter)>;

  static constexpr auto safe_png_write_struct_deleter
    = [](::libpng::png_struct *png_s) {
        png_destroy_write_struct(&png_s, ::libpng::png_infopp{ nullptr });
      };
  using safe_png_write_struct = std::
    unique_ptr<::libpng::png_struct, decltype(safe_png_write_struct_deleter)>;
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
      std::cerr << "Could not allocate info struct\n";
    }
    return info_ptr;
  }

  static constexpr double
    get_display_exponent() noexcept
  {
    const double CRT_exponent = { 2.2 };
    const double LUT_exponent = []() {
#if defined(NeXT)
      return 1.0 / 2.2;
      /*
      if (some_next_function_that_returns_gamma(&next_gamma))
      return 1.0 / next_gamma;
      */
#elif defined(sgi)
      /* there doesn't seem to be any documented function to
       * get the "gamma" value, so we do it the hard way */
      auto infile = safe_fp = {fopen("/etc/config/system.glGammaVal", "r"),fclose);
      if (infile) {
        double sgi_gamma;

        fgets(fooline, 80, infile.get());
        sgi_gamma = atof(fooline);
        if (sgi_gamma > 0.0)
          return 1.0 / sgi_gamma;
      }
      return 1.0 / 1.7;
#elif defined(Macintosh)
      return 1.8 / 2.61;
      /*
      if (some_mac_function_that_returns_gamma(&mac_gamma))
      return mac_gamma / 2.61;
      */
#else
      return 1.0; /* assume no LUT: most PCs */
#endif
    }();
    return LUT_exponent * CRT_exponent;
  }

  template<typename deleter>
  [[nodiscard]] double
    get_gamma(
      const safe_png_read_struct                         &png_ptr,
      const std::unique_ptr<::libpng::png_info, deleter> &info_ptr)
      const noexcept
  {
    static constexpr auto display_exponent = Png::get_display_exponent();
    double                g{};
    if (::libpng::png_get_gAMA(png_ptr.get(), info_ptr.get(), &g))
      ::libpng::png_set_gamma(png_ptr.get(), display_exponent, g);
    return g;
  }
  template<typename deleter>
  [[nodiscard]] auto
    get_background_color(
      const safe_png_read_struct                         &png_ptr,
      const std::unique_ptr<::libpng::png_info, deleter> &info_ptr) const
  {
    libpng::png_color_16 *pBackground{};
    png_get_bKGD(png_ptr.get(), info_ptr.get(), &pBackground);
    if (pBackground == nullptr)
      return Color32<ColorLayoutT::RGBA>{};
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
        red = green = blue
          = static_cast<std::uint8_t>((255 / 3) * pBackground->gray);
      else /* bit_depth == 4 */ /* i.e., max value is 15 */
        red = green = blue
          = static_cast<std::uint8_t>((255 / 15) * pBackground->gray);
    }
    else {
      red   = static_cast<std::uint8_t>(pBackground->red);
      green = static_cast<std::uint8_t>(pBackground->green);
      blue  = static_cast<std::uint8_t>(pBackground->blue);
    }
    return Color32<ColorLayoutT::RGBA>{ red, green, blue, 0xFFU };
  }

public:
  Png() = default;
  Png(const std::filesystem::path &filename)
  {
    auto fp = safe_fp{ fopen(filename.string().c_str(), "rb"),
                       fclose };// todo do I need fopen?

    if (fp) {
      uint8_t     sig[8]     = {};
      std::size_t bytes_read = fread(sig, 1, 8, fp.get());
      if (bytes_read != 8U || ::libpng::png_sig_cmp(sig, 0, 8) != 0) {

        std::cerr << "Bad signature \n" << filename.string() << '\n';
        std::cerr << +sig[0] << ',' << +sig[1] << ',' << +sig[2] << ','
                  << +sig[3] << ',' << +sig[4] << ',' << +sig[5] << ','
                  << +sig[6] << ',' << +sig[7] << '\n';

        return;
      }
    }
    else {
      std::cerr << "Could not open file " << filename.string()
                << " for reading\n";

      return;
    }

    // Initialize read structure
    auto png_ptr = safe_png_read_struct{ ::libpng::png_create_read_struct(
                                           PNG_LIBPNG_VER_STRING,
                                           nullptr,
                                           nullptr,
                                           nullptr),
                                         safe_png_read_struct_deleter };
    if (!png_ptr) {
      std::cerr << "Could not allocate read struct\n";
      return;
    }

    // Initialize info structure
    auto info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
      return;
    }
    png_init_io(png_ptr.get(), fp.get());
    png_set_sig_bytes(png_ptr.get(), 8);
    png_read_info(png_ptr.get(), info_ptr.get());
    png_get_IHDR(
      png_ptr.get(),
      info_ptr.get(),
      &m_width,
      &m_height,
      &m_bit_depth,
      &m_color_type,
      nullptr,
      nullptr,
      nullptr);
    m_background_color = get_background_color(png_ptr, info_ptr);
    // http://www.libpng.org/pub/png/book/chapter13.html
    // begin readpng_get_image
    if (m_color_type == PNG_COLOR_TYPE_PALETTE)
      ::libpng::png_set_expand(png_ptr.get());
    if (m_color_type == PNG_COLOR_TYPE_GRAY && m_bit_depth < 8)
      ::libpng::png_set_expand(png_ptr.get());
    if (::libpng::png_get_valid(png_ptr.get(), info_ptr.get(), PNG_INFO_tRNS))
      ::libpng::png_set_expand(png_ptr.get());

    if (m_bit_depth == 16)
      ::libpng::png_set_strip_16(png_ptr.get());
    if (
      m_color_type == PNG_COLOR_TYPE_GRAY
      || m_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      ::libpng::png_set_gray_to_rgb(png_ptr.get());

    m_gamma           = get_gamma(png_ptr, info_ptr);

    auto row_pointers = std::vector<::libpng::png_byte *>{};
    row_pointers.reserve(m_height);
    ::libpng::png_read_update_info(png_ptr.get(), info_ptr.get());

    m_row_bytes = ::libpng::png_get_rowbytes(png_ptr.get(), info_ptr.get());
    m_channels  = ::libpng::png_get_channels(png_ptr.get(), info_ptr.get());
    m_color.resize(std::size_t{ m_width } * m_height);
    assert(m_width * bytes_per_pixel == m_row_bytes);
    for (std::size_t i = 0; i != m_height; ++i)
      row_pointers.emplace_back(
        reinterpret_cast<::libpng::png_byte *>(&m_color[i * m_width]));
    // the underlying type of color32 are bytes. So all we're doing is pointing
    // at them.
    ::libpng::png_read_image(png_ptr.get(), row_pointers.data());
  }

  auto
    width() const noexcept
  {
    return m_width;
  }
  auto
    height() const noexcept
  {
    return m_height;
  }
  template<Color cT = Color32RGBA>
  static std::optional<std::filesystem::path>
    save(
      const std::uint8_t   *data,
      ::libpng::png_uint_32 width,
      ::libpng::png_uint_32 height,
      std::filesystem::path filename,
      std::string           title  = "",
      std::string           prefix = "tmp") noexcept
  {
    if (width == 0U || height == 0U)
      return std::nullopt;
    if (!open_viii::tools::i_ends_with(filename.string(), ".png")) {
      filename
        = (!filename.string().starts_with(prefix)
             ? (prefix / filename.parent_path() / filename.stem()).string()
             : (filename.parent_path() / filename.stem()).string())
        + (filename.has_extension()
             ? "_" + filename.extension().string().substr(1)
             : "")
        + ".png";
    }
    auto fp = safe_fp{ fopen(filename.string().c_str(), "wb"),
                       fclose };// todo do I need fopen?

    if (!fp) {
      std::cerr << "Could not open file " << filename.string()
                << " for writing\n";
      return std::nullopt;
    }
    // Initialize write structure
    auto png_ptr = safe_png_write_struct{ ::libpng::png_create_write_struct(
                                            PNG_LIBPNG_VER_STRING,
                                            nullptr,
                                            nullptr,
                                            nullptr),
                                          safe_png_write_struct_deleter };
    if (!png_ptr) {
      std::cerr << "Could not allocate write struct\n";
      return std::nullopt;
    }
    // Initialize info structure
    auto info_ptr = create_info_struct(png_ptr);
    if (!info_ptr) {
      return std::nullopt;
    }
    //    // Setup Exception handling
    //    if (setjmp(png_jmpbuf(png_ptr.get()))) {
    //      std::cerr <<
    //              "Error during png creation\n");// todo modernize error
    //              message
    //      return;
    //    }
    png_init_io(png_ptr.get(), fp.get());
    // Write header (8 bit colour depth)
    png_set_IHDR(
      png_ptr.get(),
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
    static constexpr auto setRBGA
      = [](::libpng::png_byte *const out, const cT in) {
          out[0] = in.r();
          out[1] = in.g();
          out[2] = in.b();
          out[3] = in.a();
        };
    auto row = std::vector<::libpng::png_byte>(
      bytes_per_pixel * width * sizeof(::libpng::png_byte));
    for (const auto y :
         std::ranges::iota_view(::libpng::png_uint_32{ 0U }, height)) {
      for (const auto x :
           std::ranges::iota_view(::libpng::png_uint_32{ 0U }, width)) {
        setRBGA(
          &row[x * bytes_per_pixel],
          reinterpret_cast<const cT *>(data)[y * width + x]);
      }
      png_write_row(png_ptr.get(), row.data());
    }
    png_write_end(png_ptr.get(), nullptr);
    return filename;
  }
  template<Color cT, typename... T>
  static std::optional<std::filesystem::path>
    save(
      const std::vector<cT> &data,
      ::libpng::png_uint_32  width,
      ::libpng::png_uint_32  height,
      T &&...t) noexcept
  {
    if (std::cmp_less(data.size(), std::size_t{ width } * height)) {
      std::cerr << "Size is wrong! " << data.size() << " != " << width << " x "
                << height << '\n';
      return std::nullopt;
    }
    return save<cT>(
      reinterpret_cast<const std::uint8_t *>(data.data()),
      width,
      height,
      std::forward<T>(t)...);
  }
  template<class... T>
  static std::optional<std::filesystem::path>
    save(const Png &data, T &&...t) noexcept
  {
    return save(data.m_color, std::forward<T>(t)...);
  }
  inline auto
    begin() const noexcept
  {
    return m_color.begin();
  }
  inline auto
    end() const noexcept
  {
    return m_color.end();
  }
  inline auto
    size() const noexcept
  {
    return m_color.size();
  }
};
}// namespace open_viii::graphics

#endif// OPENVIII_CPP_WIP_PNG_HPP
