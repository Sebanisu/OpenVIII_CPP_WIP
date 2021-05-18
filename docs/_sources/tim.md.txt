TIM file
=====
Pretty much everything else is some variation on
[Playstation TIM](http://fileformats.archiveteam.org/wiki/TIM_(PlayStation_graphics)). Many files have TIM images embedded into them.

## 16-bit

All of these images use [15-bit high color](https://en.wikipedia.org/wiki/High_color#15-bit_high_color) with 1 bit for the [STP](#stp) flag.

| 1 | 5 | 5 | 5 |
| --- | --- | --- | --- |
| STP/Alpha | Blue | Green | Red |

[Endianness](https://en.wikipedia.org/wiki/Endianness) could affect the order of the bits.

### STP

STP stands for Special Transparency Processing. You might have to ignore the STP unless the image doesn't look correct. The playstation has modes for how to
blend things.

### Blend Modes

|Mode|Formula|
|---|---|
|0|0.5 * Back + 0.50 * Front|
|1|1.0 * Back + 1.00 * Front|
|2|1.0 * Back - 1.00 * Front|
|3|1.0 * Back + 0.25 * Front|

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

Total Size: 8 bytes

|Offset (bits)|Size (bits)|Fixed Value|Description|
|---|---|---|---|
|0|8|0x10|Tag|
|8|8|0|Version|
|32|2|0b00, 0b10, 0b11, or 0b10|BPP: Bits Per Pixel|
|35|1|0,1|CLP: Color Lookup Table Present|

## Sub Header

Total Size: 12 bytes

|Offset (bytes)|Size (bytes)|Description|
|---|---|---|
|0|4|Length|
|4|2|X|
|6|2|Y|
|8|2|Width|
|10|2|Height|

## CLUT

Contains a Sub Header at the start of this section. Total Size: Sub_Header.length bytes This section might not exist in the file. This contains the 16 bit
values of colors organized in cols and rows.

## Image Data

Contains a Sub Header at the start of this section. Total Size: Sub_Header.length bytes

### 4 bpp

This contains the 4-bit color keys in cols and rows. You reference the CLUT to get the color. Max 16 colors.

### 8 bpp

This contains the 8-bit color keys in cols and rows. You reference the CLUT to get the color. Max 256 colors.

### 16 bpp

This contains the 16-bit values of colors organized in cols and rows.

### 24 bpp

This contains the 24-bit values of colors organized in cols and rows.

Sources: http://www.psxdev.net/forum/viewtopic.php?t=109
