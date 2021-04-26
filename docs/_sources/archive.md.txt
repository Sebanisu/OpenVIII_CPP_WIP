Archive Library
=====

The Archive Library supports reading and writing files from the PC releases of the game.

[File List](#file-list) contains one or more path strings. Each starting with `C:\ff8\Data\` ending with `\r\n`. These are in the same order as
the [File Index](#file-index). You scan the [File List](#file-list) to find the items you want, while doing this you would increment a counter. The counter is
used to know which entry to get from [File Index](#file-index). Lastly, you use the [File Index](#file-index) to read from the [File Source](#file-source).

[ZZZ](#zzz) contains multiple files including [FLFIFS](#fiflfs) archives of different languages. The [ZZZ](#zzz) header starts with the number of entries
followed by [FileData](#filedata) entries. [File Source](#file-source) functions can accept [FileData](#filedata) entries to extract from [ZZZ](#zzz) files.

## Archives

```{eval-rst}
.. doxygenstruct:: open_viii::archive::Archives
    :members:
```

### ArchiveTypeT

```{eval-rst}
.. doxygenenum:: open_viii::archive::ArchiveTypeT
```

### FIFLFS

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FIFLFS
```

#### Grouping

```{eval-rst}
.. doxygenstruct:: open_viii::archive::Grouping
    :members:
```

##### File List

```{eval-rst}
.. doxygennamespace:: open_viii::archive::fl
```

##### File Index

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FI
    :members: uncompressed_size, offset, compression_type, SIZE, EXT
```

##### File Source

```{eval-rst}
.. doxygennamespace:: open_viii::archive::FS
```

### ZZZ

```{eval-rst}
.. doxygenstruct:: open_viii::archive::ZZZ
    :members:
```

#### FileData

```{eval-rst}
.. doxygenstruct:: open_viii::archive::FileData
    :members:
```