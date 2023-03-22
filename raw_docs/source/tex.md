TEX file
=====

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