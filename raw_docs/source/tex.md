TEX file
=====
[TEX](http://wiki.ffrtt.ru/index.php?title=FF7/TEX_format)


Used in only the PC games for 2x resolution fonts and UI elements.

## Layout

| Offset (Bytes) | Size (Bytes) | Name                       | Description                            |
|----------------|--------------|----------------------------|----------------------------------------|
| 0              | 108          | m_tex_header               | The main TEX header                    |
| 108            | 80           | m_tex_pixel_format_header  | The pixel format header                |
| 188            | 48           | m_tex_header2              | The second part of the TEX header      |
| 236            | 4 or 0       | m_tex_header2_version2     | The third part of the TEX header (FF8) |
| 236 or 240     | Varies       | m_palette_data             | The palette data                       |
| Varies         | Varies       | m_image_data               | The image data                         |


```{eval-rst}
.. doxygenstruct:: open_viii::graphics::Tex
    :members:
```

## Header

### Part 1

Total Size: 108 bytes

| Offset (bytes) | Size (bytes) | Name                         | Description                                          |
| -------------- | ------------ | ---------------------------- | ---------------------------------------------------- |
| 0              | 4            | m_version                    | Version (must be 1 for FF7 or 2 for FF8)             |
| 4              | 4            | m_unknown_1                  | Unknown 1                                            |
| 8              | 4            | m_color_key_flag             | Color Key Flag                                       |
| 12             | 4            | m_unknown_2                  | Unknown 2                                            |
| 16             | 4            | m_unknown_3                  | Unknown 3                                            |
| 20             | 4            | m_min_bits_per_color         | Minimum Number of Bits per Color                     |
| 24             | 4            | m_max_bits_per_color         | Maximum Number of Bits per Color                     |
| 28             | 4            | m_min_alpha_bits             | Minimum Number of Alpha Bits                         |
| 32             | 4            | m_max_alpha_bits             | Maximum Number of Alpha Bits                         |
| 36             | 4            | m_min_bits_per_pixel         | Minimum Number of Bits per Pixel                     |
| 40             | 4            | m_max_bits_per_pixel         | Maximum Number of Bits per Pixel                     |
| 44             | 4            | m_unknown_4                  | Unknown 4                                            |
| 48             | 4            | m_num_palettes               | Number of Palettes                                   |
| 52             | 4            | m_num_colors_per_palette     | Number of Colors per Palette                         |
| 56             | 4            | m_bit_depth                  | Bit Depth                                            |
| 60             | 4            | m_image_width                | Image Width                                          |
| 64             | 4            | m_image_height               | Image Height                                         |
| 68             | 4            | m_bytes_per_row              | Bytes per Row (usually ignored)                      |
| 72             | 4            | m_unknown_5                  | Unknown 5                                            |
| 76             | 4            | m_palette_flag               | Palette Flag (indicates the presence of a palette)   |
| 80             | 4            | m_bits_per_index             | Bits per Index (always 0 for non-paletted images)    |
| 84             | 4            | m_indexed_to_8_bit_flag      | Indexed_to_8bit Flag (never used in FF7)             |
| 88             | 4            | m_palette_size               | Palette Size                                         |
| 92             | 4            | m_num_colors_per_palette_dup | Number of Colors per Palette (duplicate)             |
| 96             | 4            | m_runtime_data_1             | Runtime Data 1 (ignored on load)                     |
| 100            | 4            | m_bits_per_pixel             | Number of Bits per Pixel                             |
| 104            | 4            | m_bytes_per_pixel            | Number of Bytes per Pixel                            |

```{eval-rst}
.. doxygenstruct:: open_viii::graphics::TexHeader
    :members:
```

### Part 2 - Pixel Format

Total Size: 80 bytes

| Offset (bytes) | Size (bytes) | Name                     | Description                        |
|----------------|--------------|--------------------------|------------------------------------|
| 0              | 4            | m_num_red_bits           | Number of Red Bits                 |
| 4              | 4            | m_num_green_bits         | Number of Green Bits               |
| 8              | 4            | m_num_blue_bits          | Number of Blue Bits                |
| 12             | 4            | m_num_alpha_bits         | Number of Alpha Bits               |
| 16             | 4            | m_red_bitmask            | Red Bitmask                        |
| 20             | 4            | m_green_bitmask          | Green Bitmask                      |
| 24             | 4            | m_blue_bitmask           | Blue Bitmask                       |
| 28             | 4            | m_alpha_bitmask          | Alpha Bitmask                      |
| 32             | 4            | m_red_shift              | Red Shift                          |
| 36             | 4            | m_green_shift            | Green Shift                        |
| 40             | 4            | m_blue_shift             | Blue Shift                         |
| 44             | 4            | m_alpha_shift            | Alpha Shift                        |
| 48             | 4            | m_minus_8_num_red_bits   | 8 - Number of Red Bits (ignored)   |
| 52             | 4            | m_minus_8_num_green_bits | 8 - Number of Green Bits (ignored) |
| 56             | 4            | m_minus_8_num_blue_bits  | 8 - Number of Blue Bits (ignored)  |
| 60             | 4            | m_minus_8_num_alpha_bits | 8 - Number of Alpha Bits (ignored) |
| 64             | 4            | m_red_max                | Red Max                            |
| 68             | 4            | m_green_max              | Green Max                          |
| 72             | 4            | m_blue_max               | Blue Max                           |
| 76             | 4            | m_alpha_max              | Alpha Max                          |

```{eval-rst}
.. doxygenstruct:: open_viii::graphics::TexPixelFormatHeader
    :members:
```

### Part 3

Total Size: 48 bytes

| Offset (bytes) | Size (bytes) | Name                   | Description                   |
|----------------|--------------|------------------------|-------------------------------|
| 0              | 4            | m_color_key_array_flag | Color Key Array Flag          |
| 4              | 4            | m_runtime_data_2       | Runtime Data 2                |
| 8              | 4            | m_reference_alpha      | Reference Alpha               |
| 12             | 4            | m_runtime_data_3       | Runtime Data 3                |
| 16             | 4            | m_unknown_6            | Unknown 6                     |
| 20             | 4            | m_palette_index        | Palette Index (Runtime Data)  |
| 24             | 4            | m_runtime_data_4       | Runtime Data 4                |
| 28             | 4            | m_runtime_data_5       | Runtime Data 5                |
| 32             | 4            | m_unknown_7            | Unknown 7                     |
| 36             | 4            | m_unknown_8            | Unknown 8                     |
| 40             | 4            | m_unknown_9            | Unknown 9                     |
| 44             | 4            | m_unknown_10           | Unknown 10                    |

```{eval-rst}
.. doxygenstruct:: open_viii::graphics::TexHeader2
    :members:
```

### Part 4 - Tex version 2 only

Total Size: 4 bytes

| Offset (bytes) | Size (bytes) | Name        | Description                       |
|----------------|--------------|-------------|-----------------------------------|
| 0              | 4            | m_unknown_11| Unknown 11 (TEX version 2 only)   |


```{eval-rst}
.. doxygenstruct:: open_viii::graphics::TexHeader2Version2
    :members:
```
## Palette

This contains the 32-bit BGRA values of colors organized in cols and rows.

## Image

#### 8 bpp

This contains the `8-bit` color keys in cols and rows. You reference the [Palette](#palette) to get the color. Max `256`
colors.

#### 16 bpp

This contains the `16-bit` ABGR values of colors organized in cols and rows.

#### 32 bpp

This contains the `32-bit` BGRA values of colors organized in cols and rows.
