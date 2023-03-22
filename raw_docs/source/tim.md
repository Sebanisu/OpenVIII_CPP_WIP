TIM file
=====
Pretty much everything else is some variation on
[Playstation TIM](http://fileformats.archiveteam.org/wiki/TIM_(PlayStation_graphics)). Many files have TIM images
embedded into them.

## 16-bit

All of these images use [15-bit high color](https://en.wikipedia.org/wiki/High_color#15-bit_high_color) with 1 bit for
the [STP](#stp) flag.

| 1         | 5    | 5     | 5   |
|-----------|------|-------|-----|
| STP/Alpha | Blue | Green | Red |

[Endianness](https://en.wikipedia.org/wiki/Endianness) could affect the order of the bits.

### STP

STP stands for Special Transparency Processing. You might have to ignore the STP unless the image doesn't look correct.
The playstation has modes for how to
blend things.

| STP Bit	     | Black Color                        | Other Colors     |
|--------------|------------------------------------|------------------|
| Disabled (0) | Treated as a normal color (opaque) | 	Normal blending |
| Enabled (1)  | Treated as transparent	            | Special blending |

### Blend Modes

| Mode | Name            | Formula                                             | Description                                                                                 |
|------|-----------------|-----------------------------------------------------|---------------------------------------------------------------------------------------------|
| 0    | Normal Blending | $0.5 \times \text{Back} + 0.50 \times \text{Front}$ | Equal blending of the back and front colors, producing a 50% transparency effect.           |
| 1    | Additive        | $1.0 \times \text{Back} + 1.00 \times \text{Front}$ | Adds the color values of the back and front, resulting in a brighter output color.          |
| 2    | Subtractive     | $1.0 \times \text{Back} - 1.00 \times \text{Front}$ | Subtracts the front color values from the back, resulting in a darker output color.         |
| 3    | Screen          | $1.0 \times \text{Back} + 0.25 \times \text{Front}$ | Adds a portion (25%) of the front color values to the back, creating a softer light effect. |

Source:
http://www.psxdev.net/forum/viewtopic.php?t=953

## 5 bit to 8 bit

```{math}
   \begin{align}
      f(x)    = & \frac{255x}{31}  \\
                   or\\   
      f(x)    = & (x << 3) | (x >> 2)
   \end{align}   
```

## Header

| Offset (bits) | Size (bits) |            Fixed Value            | Description                               |
|:-------------:|:-----------:|:---------------------------------:|:------------------------------------------|
|       0       |      8      |              `0x10`               | Tag                                       |
|       8       |      8      |                `0`                | Version                                   |
|      32       |      2      | `0b00`, `0b01`, `0b10`, or `0b11` | BPP: Bits Per Pixel                       |
|      35       |      1      |            `0` or `1`             | CLP: [Color Look-Up Table](#clut) Present |

| BPP Value | Bits Per Pixel | CLP: [Color Look-Up Table](#clut) Present |
|:---------:|:--------------:|:-----------------------------------------:|
|  `0b00`   |     4-bit      |                     1                     |
|  `0b01`   |     8-bit      |                     1                     |
|  `0b10`   |     16-bit     |                     0                     |
|  `0b11`   |     24-bit     |                     0                     |

## Sub Header

Total Size: 12 bytes

| Offset (bytes) | Size (bytes) | Description |
|----------------|--------------|-------------|
| 0              | 4            | Length      |
| 4              | 2            | X           |
| 6              | 2            | Y           |
| 8              | 2            | Width       |
| 10             | 2            | Height      |

## CLUT

Contains a Sub Header at the start of this section. Total Size: `Sub_Header.length` bytes. This section might not exist
in the file. This contains the `16-bit` values of colors organized in cols and rows. In some cases, game developers and
graphic artists exploited the unused space in the Color Look-Up Table ([CLUT](#clut))
to store additional palette colors. For example, while a standard `4-bit` color palette would only require `16` colors,
they could utilize all `256` available colors by storing multiple `16-color` palettes in a row. To properly read and
display these additional colors, you would need to override the default [CLUT](#clut) row and column settings.

## Image Data

Contains a Sub Header at the start of this section. Total Size: `Sub_Header.length` bytes

### 4 bpp

This contains the `4-bit` color keys in cols and rows. You reference the [CLUT](#clut) to get the color. Max `16`
colors.

### 8 bpp

This contains the `8-bit` color keys in cols and rows. You reference the [CLUT](#clut) to get the color. Max `256`
colors.

### 16 bpp

This contains the `16-bit` values of colors organized in cols and rows.

### 24 bpp

This contains the `24-bit` values of colors organized in cols and rows.

By extending the [CLUT](#clut) and overriding the default row and column settings, developers could achieve more varied
and visually appealing graphics, even with the limited color depth of the PlayStation One hardware.

Sources: http://www.psxdev.net/forum/viewtopic.php?t=109
