Archive Library
=====

```{toctree}
```

The Archive Library supports reading and writing files from the PC releases of the game.

[File List](#file-list) contains one or more path strings. Each starting with `C:\ff8\Data\` ending with `\r\n`. These are in the same order as
the [File Index](#file-index). You scan the [File List](#file-list) to find the items you want, while doing this you would increment a counter. The counter is
used to know which entry to get from [File Index](#file-index). Lastly, you use the [File Index](#file-index) to read from the [File Source](#file-source).

[ZZZ](#zzz) contains multiple files including [FLFIFS](#fiflfs) archives of different languages. The [ZZZ](#zzz) header starts with the number of entries
followed by [FileData](#filedata) entries. [File Source](#file-source) functions can accept [FileData](#filedata) entries to extract from [ZZZ](#zzz) files.

## [2000 CD release](https://www.pcgamingwiki.com/wiki/Final_Fantasy_VIII)

My copy only has [en](https://en.wikipedia.org/wiki/English_language) language. I think each copy is regional to what language(s) it has on it.

- Data
  - battle.fi
  - battle.fl
  - battle.fs
  - field.fi
  - field.fl
  - field.fs
  - magic.fi
  - magic.fl
  - magic.fs
  - main.fi
  - main.fl
  - main.fs
  - menu.fi
  - menu.fl
  - menu.fs
  - world.fi
  - world.fl
  - world.fs

## [2013 Steam release](https://store.steampowered.com/app/39150/FINAL_FANTASY_VIII/)

This version supports multiple languages [de](https://en.wikipedia.org/wiki/German_language), [en](https://en.wikipedia.org/wiki/English_language)
, [es](https://en.wikipedia.org/wiki/Spanish_language), [fr](https://en.wikipedia.org/wiki/French_language)
, [it](https://en.wikipedia.org/wiki/Italian_language). You need to manually install each one via steam. The lang.dat tells the game which one you are using. I
think there was a [jp](https://en.wikipedia.org/wiki/Japanese_language) release sold via square. There are mods to change one of the languages
to [ru](https://en.wikipedia.org/wiki/Russian_language) or [pl](https://en.wikipedia.org/wiki/Polish_language).

- Data
  - lang-en
    - battle.fi
    - battle.fl
    - battle.fs
    - field.fi
    - field.fl
    - field.fs
    - magic.fi
    - magic.fl
    - magic.fs
    - main.fi
    - main.fl
    - main.fs
    - menu.fi
    - menu.fl
    - menu.fs
    - world.fi
    - world.fl
    - world.fs
- lang.dat

## [2019 Steam Remaster release](https://store.steampowered.com/app/1026680/FINAL_FANTASY_VIII__REMASTERED/)

This version supports multiple languages [de](https://en.wikipedia.org/wiki/German_language), [en](https://en.wikipedia.org/wiki/English_language)
, [es](https://en.wikipedia.org/wiki/Spanish_language), [fr](https://en.wikipedia.org/wiki/French_language)
, [it](https://en.wikipedia.org/wiki/Italian_language), and [jp](https://en.wikipedia.org/wiki/Japanese_language). All the languages are included and need to be
selected in game.

- main.zzz
  - data
    - lang-en
      - battle.fi
      - battle.fl
      - battle.fs
      - main.fi
      - main.fl
      - main.fs
      - menu.fi
      - menu.fl
      - menu.fs
    - field.fi
    - field.fl
    - field.fs
    - magic.fi
    - magic.fl
    - magic.fs
    - world.fi
    - world.fl
    - world.fs
- other.zzz

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