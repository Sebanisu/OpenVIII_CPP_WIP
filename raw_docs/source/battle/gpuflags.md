GPU flags
=====
[FileFormatX](https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X)

Total Size: 1 byte

| Offset (bits) | Size (bits) | Name | Description                               |
|---------------|-------------|------|-------------------------------------------|
|      0        |      1      |  v0  | Bit 0 (0x1)                               |
|      1        |      1      |  add | Bit 1 (0x2): Affects fire/light blending  |
|      2        |      1      |  v2  | Bit 2 (0x4)                               |
|      3        |      1      |  v3  | Bit 3 (0x8)                               |
|      4        |      1      |  v4  | Bit 4 (0x10)                              |
|      5        |      1      |  v5  | Bit 5 (0x20)                              |
|      6        |      1      |  v6  | Bit 6 (0x40)                              |
|      7        |      1      |  v7  | Bit 7 (0x80)                              |


```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::GpuFlags
    :members:
```