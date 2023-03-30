X File
=====

[X](https://wiki.ffrtt.ru/index.php?title=FF8/FileFormat_X)

The filenames start with "a0stg" followed by a three-digit number, ranging from 000 to 162, and end with the ".x"
extension.
The regular expression `a0stg\d{3}\.x`{l=regex} matches filenames that start with "a0stg", followed by three digits, and
ending with ".x".

## Layout

| Name                                | 	Description                                                                                                                                      |
|-------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------------|
| PlayStation MIPS assembly           | Used only in PS version; skipped in PC.</br>Usually starts with `E8 FF BD 27 01 00 02`.                                                           |
| [Camera data + movement](camera.md) | Contains camera animations and movement (pre-keyed).</br>Usually starts with `02 00 08 00 20 00`.</br>Stage 147 has  `02 00 08 00 18 00` as well. |
| [Model section](geometry.md)        | Contains object (group) and its sub-objects, vertex/triangle grouping.</br>Usually starts with `06 00 00 00`.                                     |
| [Texture](../graphics/tim.md)	      | Contains one .TIM texture, 8 BPP.</br>Always starts with `10 00 00 00 09`.                                                                        |

Note that battle stages do not contain pointers to the next sections. All pointers
are [hardcoded](https://wiki.ffrtt.ru/index.php/FF8/BattleStage/Pointers) in `FF8.EXE`.

```{eval-rst}
.. doxygenstruct:: open_viii::battle::stage::X
    :members:
```

## File List

[Stage List](https://wiki.ffrtt.ru/index.php/FF8/BS_list)

|    Name    | Description                                      |
|:----------:|--------------------------------------------------|
| a0stg000.x | Balamb Garden Quad                               |
| a0stg001.x | Dollet Bridge                                    |
| a0stg002.x | Dollet Trasmission Tower path                    |
| a0stg003.x | Dollet Transmission Tower (Top)                  |
| a0stg004.x | Dollet Transmission Tower (Elevator)             |
| a0stg005.x | Dollet Transmission Tower (Elevator (UNUSED?))   |
| a0stg006.x | Dollet City                                      |
| a0stg007.x | Balamb Garden entrance gate                      |
| a0stg008.x | Balamb Garden 1st Floor Hall                     |
| a0stg009.x | Balamb Garden 2nd Floor Corridor                 |
| a0stg010.x | Balamb Garden Quad (Balamb raid version)         |
| a0stg011.x | Balamb Garden Outer Corridor                     |
| a0stg012.x | Balamb Garden Training Center (elevator zone)    |
| a0stg013.x | Balamb Garden Norg's Floor                       |
| a0stg014.x | Balamb Garden Underground Levels (Tube)          |
| a0stg015.x | Balamb Garden Underground levels                 |
| a0stg016.x | Balamb Garden Underground levels (OilBoil)       |
| a0stg017.x | Timber Pub Area                                  |
| a0stg018.x | Timber Maniacs square                            |
| a0stg019.x | Train (Deling Presidential Vagon)                |
| a0stg020.x | Deling City Sewers                               |
| a0stg021.x | Deling City (Caraway Residence secret exit)      |
| a0stg022.x | Balamb Garden Class Room                         |
| a0stg023.x | Galbadia Garden Corridor                         |
| a0stg024.x | Galbadia Garden Corridor 2                       |
| a0stg025.x | Galbadia Missile Base                            |
| a0stg026.x | Deep Sea Research Center (Bahamut)               |
| a0stg027.x | Balamb Town (Balamb Hotel road)                  |
| a0stg028.x | Balamb Town (Balamb Hotel Hall)                  |
| a0stg029.x | (UNUSED?)                                        |
| a0stg030.x | Fire Cavern (path)                               |
| a0stg031.x | Fire Cavern (Ifrit Lair)                         |
| a0stg032.x | Galbadia Garden Hall                             |
| a0stg033.x | Galbadia Garden Auditorium (Seifer battle CD2)   |
| a0stg034.x | Galbadia Garden Auditorium 2 (Seifer + Edea CD2) |
| a0stg035.x | Galbadia Garden Corridor                         |
| a0stg036.x | Galbadia Garden (Ice Hockey Field)               |
| a0stg037.x | Ultimecia - griever                              |
| a0stg038.x | Ultimecia - last form                            |
| a0stg039.x | Desert Prison (UNUSED?)                          |
| a0stg040.x | Desert Prison                                    |
| a0stg041.x | Eshtar City (road)                               |
| a0stg042.x | Desert Prison (Top)                              |
| a0stg043.x | Eshtar City (road2)                              |
| a0stg044.x | Missile Base Inside (UNUSED?)                    |
| a0stg045.x | Missile Base? Inside (UNUSED?)                   |
| a0stg046.x | Missile Base Control room                        |
| a0stg047.x | Winhill Village main square                      |
| a0stg048.x | Tomb of the Unknown King (Corridor)              |
| a0stg049.x | Eshtar City (road 3)                             |
| a0stg050.x | Tomb of the Unknown King (Boss Fight)            |
| a0stg051.x | Fisherman Horizon (Road)                         |
| a0stg052.x | Fisherman Horizon (Train Station Square)         |
| a0stg053.x | Desert Prison Cell (UNUSED?)                     |
| a0stg054.x | Salt Lake                                        |
| a0stg055.x | Ultima Weapon Stage                              |
| a0stg056.x | Salt Lake 2                                      |
| a0stg057.x | Eshtar Road                                      |
| a0stg058.x | Ultimecia's Castle (bridge)                      |
| a0stg059.x | Eshtar (road)                                    |
| a0stg060.x | Sorceress memorial                               |
| a0stg061.x | Lunatic Pandora laboratory                       |
| a0stg062.x | Lunatic Pandora (Ragnarok break-in)              |
| a0stg063.x | Lunatic Pandora                                  |
| a0stg064.x | Lunatic Pandora                                  |
| a0stg065.x | Lunatic Pandora                                  |
| a0stg066.x | Lunatic Pandora                                  |
| a0stg067.x | Lunatic Pandora (elevators)                      |
| a0stg068.x | Lunatic Pandora (Seifer battle)                  |
| a0stg069.x | Lunatic Pandora (Adel)                           |
| a0stg070.x | (Centra excavation site)                         |
| a0stg071.x | (Centra excavation site)                         |
| a0stg072.x | (Centra excavation site)                         |
| a0stg073.x | (Centra excavation site)                         |
| a0stg074.x | (Centra excavation site)                         |
| a0stg075.x | (Centra excavation site)                         |
| a0stg076.x | Centra Ruins (Lower Level)                       |
| a0stg077.x | Centra Ruins (Tower Level)                       |
| a0stg078.x | Centra Ruins (Tower Level)                       |
| a0stg079.x | Centra Ruins (Odin Room)                         |
| a0stg080.x | Centra excavation site (Entrance)                |
| a0stg081.x | Trabia Canyon                                    |
| a0stg082.x | Ragnarok                                         |
| a0stg083.x | Ragnarok                                         |
| a0stg084.x | Diabolous lair (Diablos)                         |
| a0stg085.x | Deep Sea Research Center (Entrance)              |
| a0stg086.x | Deep Sea Research Center                         |
| a0stg087.x | Deep Sea Research Center                         |
| a0stg088.x | Deep Sea Research Center                         |
| a0stg089.x | Deep Sea Research Center                         |
| a0stg090.x | Deep Sea Research Center                         |
| a0stg091.x | Deep Sea Research Center                         |
| a0stg092.x | Worm-like witch battle scene                     |
| a0stg093.x | Eshtar - Shop site                               |
| a0stg094.x | Tear's Point                                     |
| a0stg095.x | Eshtar                                           |
| a0stg096.x | Ultimecia's Castle                               |
| a0stg097.x | Ultimecia's Castle                               |
| a0stg098.x | Ultimecia's Castle                               |
| a0stg099.x | Ultimecia's Castle                               |
| a0stg100.x | Deling City (Edea's Room)                        |
| a0stg101.x | Balamb Plains                                    |
| a0stg102.x | Desert Canyon                                    |
| a0stg103.x | Desert                                           |
| a0stg104.x | Snow-Covered Plains                              |
| a0stg105.x | Forest                                           |
| a0stg106.x | Snow-Covered Forest                              |
| a0stg107.x | Beach                                            |
| a0stg108.x | Snow Beach                                       |
| a0stg109.x | Eshtar City                                      |
| a0stg110.x | Eshtar City                                      |
| a0stg111.x | Landscape- Dirt Ground                           |
| a0stg112.x | Landscape- Grass Ground                          |
| a0stg113.x | Landscape- Dirt Ground                           |
| a0stg114.x | Landscape- Snow Covered Mountains                |
| a0stg115.x | Eshtar City                                      |
| a0stg116.x | Eshtar City                                      |
| a0stg117.x | Landscape                                        |
| a0stg118.x | Eshtar City                                      |
| a0stg119.x | Eshtar City                                      |
| a0stg120.x | Ultimecia's Castle                               |
| a0stg121.x | Ultimecia's Castle                               |
| a0stg122.x | Ultimecia's Castle                               |
| a0stg123.x | Ultimecia's Castle                               |
| a0stg124.x | Ultimecia's Castle                               |
| a0stg125.x | Ultimecia's Castle                               |
| a0stg126.x | Ultimecia's Castle                               |
| a0stg127.x | Ultimecia's Castle                               |
| a0stg128.x | Landscape                                        |
| a0stg129.x | Landscape                                        |
| a0stg130.x | Landscape                                        |
| a0stg131.x | Landscape                                        |
| a0stg132.x | Landscape                                        |
| a0stg133.x | Landscape                                        |
| a0stg134.x | Eshtar City                                      |
| a0stg135.x | Landscape                                        |
| a0stg136.x | Landscape (beach night)                          |
| a0stg137.x | Deling City (Edea's Room)                        |
| a0stg138.x | Ultimecia's Castle                               |
| a0stg139.x | Ultimecia's Castle (Tiamat)                      |
| a0stg140.x | Ultimecia's Castle                               |
| a0stg141.x | Ultimecia's Castle                               |
| a0stg142.x | Ultimecia's Castle                               |
| a0stg143.x | Eshtar City                                      |
| a0stg144.x | Lunatic Pandora Lab                              |
| a0stg145.x | Lunatic Pandora Lab                              |
| a0stg146.x | Sorcerer memorial                                |
| a0stg147.x | Edea Dollet stage DISC 1 final battle            |
| a0stg148.x | Tomb of the Unknown King (Boss Fight)            |
| a0stg149.x | Prison control room                              |
| a0stg150.x | Landscape                                        |
| a0stg151.x | Landscape                                        |
| a0stg152.x | Balamb Garden (External Corridor)                |
| a0stg153.x | Balamb Garden (External Corridor)                |
| a0stg154.x | Balamb Garden (External Corridor)                |
| a0stg155.x | Balamb Garden (External Corridor)                |
| a0stg156.x | Balamb Garden (External Corridor)                |
| a0stg157.x | Landscape                                        |
| a0stg158.x | Landscape                                        |
| a0stg159.x | Landscape                                        |
| a0stg160.x | Debug UV tile stage (UNUSED)                     |
| a0stg161.x | Landscape                                        |
| a0stg162.x | Landscape                                        |