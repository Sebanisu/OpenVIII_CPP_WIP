Quad file
=====
[Quad](https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Quad)

Total Size: 24 bytes

| Offset (bytes) | Size (bytes) | Name               | Description </br>Formula                                                                         |
|:--------------:|:------------:|--------------------|--------------------------------------------------------------------------------------------------|
|       0        |      2       | m_face_indice_a    | Face index A                                                                                     |
|       2        |      2       | m_face_indice_b    | Face index B                                                                                     |
|       4        |      2       | m_face_indice_c    | Face index C                                                                                     |
|       6        |      2       | m_face_indice_d    | Face index D                                                                                     |
|       8        |      2       | m_uv1              | UV coordinate 1                                                                                  |
|       10       |      2       | m_raw_clut         | Raw CLUT value </br>{math}`((\text{raw_clut} \ll 2) ∣ (\text{raw_clut} \gg (16 - 2))) \;\&\; 15` |
|       12       |      2       | m_uv2              | UV coordinate 2                                                                                  |
|       14       |      1       | m_raw_texture_page | Raw texture page value </br>{math}`(\text{raw_texture_page} \gg 4) \;\text{&}\; 15`              |
|       15       |      1       | m_raw_hide         | Raw hide value </br>{math}`\text{raw_hide} \neq 0`                                                 |
|       16       |      2       | m_uv3              | UV coordinate 3                                                                                  |
|       18       |      2       | m_uv4              | UV coordinate 4                                                                                  |
|       20       |      3       | m_color            | Color value </br>(graphics::Color24RGB)                                                          |
|       23       |      1       | m_raw_gpu          | Raw GPU flags                                                                                    |

```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::Quad
    :members:
```