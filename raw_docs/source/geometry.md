Geometry
=====
[Geometry](https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X#Geometry)

## Geometries

| Offset (bytes) | Size (bytes) | Name         | Description                                                |
|:--------------:|:------------:|--------------|------------------------------------------------------------|
|       0        |    varies    | m_geometries | Vector of Geometry objects (usually one, but can be more). |

```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::Geometries
    :members:
```

### Geometry

| Offset (bytes) | Size (bytes) | Name               | Description                                                                                     |
|:--------------:|:------------:|--------------------|-------------------------------------------------------------------------------------------------|
|       0        |      6       | m_geometry_header1 | First header of the geometry object.                                                            |
|       6        |    varies    | m_vertices         | Vector of vertices.                                                                             |
|     varies     |     4-7      | padding            | Padding formula. </br>Position when reading the vertices </br>{math}`(position \; \% \; 4) + 4` |
|     varies     |      8       | m_geometry_header2 | second header of the geometry object.                                                           |
|     varies     |    varies    | m_triangles        | Vector of triangles.                                                                            |
|     varies     |    varies    | m_quads            | Vector of quads.                                                                                |

```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::Geometry
    :members:
```

#### First Header

Total Size: 6 bytes

| Offset (bytes) | Size (bytes) | Name                | Description           |
|----------------|--------------|---------------------|-----------------------|
| 0              | 2            | m_always_1_0_1      | Always 0x0100         |
| 2              | 2            | m_always_1_0_2      | Always 0x0100         |
| 4              | 2            | m_number_vertices   | Number of vertices    |


```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::GeometryHeader1
    :members:
```

#### Second Header

Total Size: 8 bytes

| Offset (bytes) | Size (bytes) | Name             | Description                 |
|----------------|--------------|------------------|-----------------------------|
| 0              | 2            | m_triangle_count | Number of triangles         |
| 2              | 2            | m_quad_count     | Number of quads             |
| 4              | 4            | m_padding        | Padding to maintain alignment |


```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::GeometryHeader2
    :members:
```