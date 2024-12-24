//
// Created by pcvii on 6/29/2021.
//

#ifndef OPENVIII_CPP_WIP_PNG_HPP
#define OPENVIII_CPP_WIP_PNG_HPP
#include "open_viii/graphics/Color.hpp"
#include "open_viii/tools/Tools.hpp"
#include "png.h"
namespace open_viii::graphics {

/**
 * @brief A class for reading and writing PNG files.
 */
struct Png
{
private:
  std::filesystem::path m_filename{};  ///< The filename of the PNG file.
  png_uint_32           m_width{};     ///< The width of the PNG in pixels.
  png_uint_32           m_height{};    ///< The height of the PNG in pixels.
  int                   m_bit_depth{}; ///< The bit depth of the PNG.
  int                   m_color_type{};///< The color type of the PNG.
  std::size_t m_row_bytes{};///< The number of bytes per row in the PNG.
  png_byte    m_channels{}; ///< The number of color channels in the PNG.
  double      m_gamma{};    ///< The gamma value of the PNG.
  std::vector<Color32<ColorLayoutT::RGBA>>
    m_color{};///< The color data of the PNG.
  Color32<ColorLayoutT::RGBA>
              m_background_color{};///< The background color of the PNG.
  png_uint_32 bytes_per_pixel = 4;///< The number of bytes per pixel in the PNG.

  static constexpr auto fclose_wrapper = [](FILE *fp) -> int {
    return fclose(fp);
  };
  /**
   * @brief Alias for a unique pointer to a FILE, with a custom deleter that
   * uses fclose().
   */
  using safe_fp = std::unique_ptr<FILE, decltype(fclose_wrapper)>;

  /**
   * @brief Deleter for a PNG read structure that uses
   * png_destroy_read_struct().
   */
  static constexpr auto safe_png_read_struct_deleter = [](png_struct *png_s) {
    png_destroy_read_struct(&png_s, nullptr, nullptr);
  };

  /**
   * @brief Alias for a unique pointer to a PNG read structure, with a custom
   * deleter.
   */
  using safe_png_read_struct
    = std::unique_ptr<png_struct, decltype(safe_png_read_struct_deleter)>;

  /**
   * @brief Deleter for a PNG write structure that uses
   * png_destroy_write_struct().
   */
  static constexpr auto safe_png_write_struct_deleter = [](png_struct *png_s) {
    png_destroy_write_struct(&png_s, png_infopp{ nullptr });
  };

  /**
   * @brief Alias for a unique pointer to a PNG write structure, with a custom
   * deleter.
   */
  using safe_png_write_struct
    = std::unique_ptr<png_struct, decltype(safe_png_write_struct_deleter)>;

  /**
   * @brief Creates a new `png_info` structure.
   *
   * @tparam deleter The type of the deleter for the `png_struct` pointer.
   * @param png_ptr The `png_struct` pointer.
   * @return A `unique_ptr` to the newly created `png_info` structure.
   */
  template<typename deleter>
  static auto
    create_info_struct(const std::unique_ptr<png_struct, deleter> &png_ptr)
  {
    // Initialize info structure
    const auto safe_png_info_deleter = [&png_ptr](png_info *info) {
      png_free_data(png_ptr.get(), info, PNG_FREE_ALL, -1);
    };
    using safe_png_info
      = std::unique_ptr<png_info, decltype(safe_png_info_deleter)>;
    auto info_ptr = safe_png_info{ png_create_info_struct(png_ptr.get()),
                                   safe_png_info_deleter };
    if (!info_ptr) {
      std::cerr << "Could not allocate info struct\n";
    }
    return info_ptr;
  }

  /**
   * @brief Calculates the display gamma exponent.
   *
   * @return The display gamma exponent.
   */
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
      auto infile = safe_fp = {fopen("/etc/config/system.glGammaVal", "r"),{});
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

  /**
   * @brief Returns the gamma value of the image.
   *
   * @tparam deleter Deleter of the std::unique_ptr<>::deleter_type type.
   * @param png_ptr Safe pointer to the png_struct object.
   * @param info_ptr Safe pointer to the png_info object.
   * @return double The gamma value of the image.
   */
  template<typename deleter>
  [[nodiscard]] double
    get_gamma(
      const safe_png_read_struct               &png_ptr,
      const std::unique_ptr<png_info, deleter> &info_ptr) const noexcept
  {
    static constexpr auto display_exponent = Png::get_display_exponent();
    double                g{};
    if (png_get_gAMA(png_ptr.get(), info_ptr.get(), &g))
      png_set_gamma(png_ptr.get(), display_exponent, g);
    return g;
  }

  /**
   * @brief Gets the background color of the image.
   *
   * @tparam deleter Deleter of the std::unique_ptr<>::deleter_type type.
   * @param png_ptr Safe pointer to the png_struct object.
   * @param info_ptr Safe pointer to the png_info object.
   * @return auto The background color of the image.
   */
  template<typename deleter>
  [[nodiscard]] auto
    get_background_color(
      const safe_png_read_struct               &png_ptr,
      const std::unique_ptr<png_info, deleter> &info_ptr) const
  {
    png_color_16 *pBackground{};
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
  /**
   * @brief Default constructor for Png object.
   */
  Png() = default;

  /**
   * @brief Constructor for Png object.
   *
   * @param filename Path to the PNG file.
   * @param flipv Flag to indicate if the image should be flipped vertically.
   */
  Png(const std::filesystem::path &filename, bool flipv = false)
  {
    auto fp = safe_fp{ fopen(filename.string().c_str(), "rb"),
                       {} };// todo do I need fopen?

    if (fp) {
      uint8_t     sig[8]     = {};
      std::size_t bytes_read = fread(sig, 1, 8, fp.get());
      if (bytes_read != 8U || png_sig_cmp(sig, 0, 8) != 0) {

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
    auto png_ptr = safe_png_read_struct{
      png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr),
      safe_png_read_struct_deleter
    };
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
      png_set_expand(png_ptr.get());
    if (m_color_type == PNG_COLOR_TYPE_GRAY && m_bit_depth < 8)
      png_set_expand(png_ptr.get());
    if (png_get_valid(png_ptr.get(), info_ptr.get(), PNG_INFO_tRNS))
      png_set_expand(png_ptr.get());

    if (m_bit_depth == 16)
      png_set_strip_16(png_ptr.get());
    if (
      m_color_type == PNG_COLOR_TYPE_GRAY
      || m_color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
      png_set_gray_to_rgb(png_ptr.get());

    m_gamma           = get_gamma(png_ptr, info_ptr);

    auto row_pointers = std::vector<png_byte *>{};
    row_pointers.reserve(m_height);
    png_read_update_info(png_ptr.get(), info_ptr.get());

    m_row_bytes           = png_get_rowbytes(png_ptr.get(), info_ptr.get());
    m_channels            = png_get_channels(png_ptr.get(), info_ptr.get());

    bytes_per_pixel       = static_cast<png_uint_32>(m_row_bytes / m_width);
    const auto get_colors = [this](
                              const auto &in_flipv,
                              auto       &in_row_pointers,
                              auto       &in_png_ptr,
                              auto       &in_color) {
      in_color.resize(std::size_t{ m_width } * m_height);
      auto rows = std::views::iota(std::uint32_t{}, std::uint32_t{ m_height });
      if (in_flipv) {
        auto b = std::ranges::rbegin(rows);
        auto e = std::ranges::rend(rows);
        for (; b != e; ++b)
          in_row_pointers.emplace_back(
            reinterpret_cast<png_byte *>(&in_color[*b * m_width]));
      }
      else {
        auto b = std::ranges::begin(rows);
        auto e = std::ranges::end(rows);
        for (; b != e; ++b)
          in_row_pointers.emplace_back(
            reinterpret_cast<png_byte *>(&in_color[*b * m_width]));
      }
      // the underlying type of color32 are bytes. So all we're doing is
      // pointing at them.
      png_read_image(in_png_ptr.get(), in_row_pointers.data());
    };
    if (bytes_per_pixel == 3U) {
      std::vector<Color24RGB> color24{};
      get_colors(flipv, row_pointers, png_ptr, color24);
      m_color.resize(std::size(color24));
      std::ranges::transform(color24, std::begin(m_color), [](const auto &c24) {
        return static_cast<Color32RGBA>(c24);
      });
    }
    else if (bytes_per_pixel == 4U) {
      get_colors(flipv, row_pointers, png_ptr, m_color);
    }
    else {
      assert(false);
    }
  }

  /**
   * @brief Returns the width of the PNG image
   * @return Width of the PNG image
   */
  auto
    width() const noexcept
  {
    return m_width;
  }

  /**
   * @brief Returns the height of the PNG image
   * @return Height of the PNG image
   */
  auto
    height() const noexcept
  {
    return m_height;
  }

  /**
   * @brief Saves an array of color data as a PNG image file
   *
   * @tparam cT Color type
   * @param data Array of color data
   * @param width Width of the image
   * @param height Height of the image
   * @param filename Filename to save the PNG image as
   * @param title Optional title for the PNG image
   * @param prefix Optional prefix for the filename
   * @return The saved filename or std::nullopt if saving failed
   */
  template<Color cT = Color32RGBA>
  static std::optional<std::filesystem::path>
    save(
      const std::uint8_t   *data,
      png_uint_32           width,
      png_uint_32           height,
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
                       {} };// todo do I need fopen?

    if (!fp) {
      std::cerr << "Could not open file " << filename.string()
                << " for writing\n";
      return std::nullopt;
    }
    // Initialize write structure
    auto png_ptr = safe_png_write_struct{
      png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr),
      safe_png_write_struct_deleter
    };
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
      static char k[] = "Title";
      png_text    title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key         = k;
      title_text.text        = title.data();
      png_set_text(png_ptr.get(), info_ptr.get(), &title_text, 1);
    }

    png_write_info(png_ptr.get(), info_ptr.get());
    static constexpr auto setRBGA = [&](png_byte *const out, const cT in) {
      out[0] = in.r();
      out[1] = in.g();
      out[2] = in.b();
      out[3] = in.a();
    };
    auto row = std::vector<png_byte>(4 * width * sizeof(png_byte));
    for (const auto y : std::ranges::iota_view(png_uint_32{ 0U }, height)) {
      for (const auto x : std::ranges::iota_view(png_uint_32{ 0U }, width)) {
        setRBGA(&row[x * 4], reinterpret_cast<const cT *>(data)[y * width + x]);
      }
      png_write_row(png_ptr.get(), row.data());
    }
    png_write_end(png_ptr.get(), nullptr);
    return filename;
  }

  /**
   * @brief Saves a vector of color data as a PNG image file
   *
   * @tparam cT Color type
   * @tparam T Variadic template parameters
   * @param data Vector of color data
   * @param width Width of the image
   * @param height Height of the image
   * @param t Variadic template parameters
   * @return The saved filename or std::nullopt if saving failed
   */
  template<Color cT, typename... T>
  static std::optional<std::filesystem::path>
    save(
      const std::vector<cT> &data,
      png_uint_32            width,
      png_uint_32            height,
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

  /**
   * @brief Saves the color data of a Png object as a PNG image file
   *
   * @tparam T Variadic template parameters
   * @param data Png object
   * @param t Variadic template parameters
   * @return The saved filename or std::nullopt if saving failed
   */
  template<class... T>
  static std::optional<std::filesystem::path>
    save(const Png &data, T &&...t) noexcept
  {
    return save(data.m_color, std::forward<T>(t)...);
  }

  /**
   * @brief Returns an iterator to the beginning of the color data.
   * @return An iterator pointing to the first element of the color data.
   */
  inline auto
    begin() const noexcept
  {
    return m_color.begin();
  }

  /**
   * @brief Returns an iterator to the end of the color data.
   * @return An iterator pointing to the element past the end of the color data.
   */
  inline auto
    end() const noexcept
  {
    return m_color.end();
  }

  /**
   * @brief Returns the number of elements in the color data.
   * @return The number of elements in the color data.
   */
  inline auto
    size() const noexcept
  {
    return m_color.size();
  }
};
}// namespace open_viii::graphics

#endif// OPENVIII_CPP_WIP_PNG_HPP
