X File
=====
[X](https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X)

## Layout

| Name                      | 	Description                                                                                              |
|---------------------------|-----------------------------------------------------------------------------------------------------------|
| PlayStation MIPS assembly | Used only in PS version; skipped in PC. Usually starts with `E8 FF BD 27 01 00 02`.                       |
| Camera data + movement    | Contains camera animations and movement (pre-keyed). Usually starts with `02 00 08 00 20 00`.             |
| Model section             | Contains object (group) and its sub-objects, vertex/triangle grouping. Usually starts with `06 00 00 00`. |
| Texture	                  | Contains one .TIM texture, 8 BPP. Always starts with `10 00 00 00 09`.                                    |

Note that battle stages do not contain pointers to the next sections. All pointers
are [hardcoded](https://wiki.ffrtt.ru/index.php/FF8/BattleStage/Pointers) in `FF8.EXE`.

```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::X
    :members:
```