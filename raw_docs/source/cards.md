Cards
======

https://finalfantasy.fandom.com/wiki/Triple_Triad_(Final_Fantasy_VIII)
https://finalfantasy.fandom.com/wiki/Final_Fantasy_VIII_Triple_Triad_cards

<div style="width: 100%; overflow-x: auto;">

| ID  | Level 1                                                                             | ID  | Level 2      | ID  | Level 3      | ID  | Level 4     | ID  | Level 5       | ID  | Level 6 (Boss) | ID  | Level 7 (Boss) | ID  | Level 8 (Guardian Force) | ID  | Level 9 (Guardian Force) | ID  | Level 10 (Player) |
|-----|-------------------------------------------------------------------------------------|-----|--------------|-----|--------------|-----|-------------|-----|---------------|-----|----------------|-----|----------------|-----|--------------------------|-----|------------------------------------|-----|-------------------|
| 0   | Geezard                                                                             | 11  | Grat         | 22  | Forbidden    | 33  | Turtapod    | 44  | Iron Giant    | 55  | Fujin Raijin   | 66  | Propagator     | 77  | Chubby Chocobo           | 88  | Carbuncle                          | 99  | Ward              |
| 1   | Funguar                                                                             | 12  | Buel         | 23  | Armadodo     | 34  | Vysage      | 45  | Behemoth      | 56  | Elvoret        | 67  | Jumbo Cactuar  | 78  | Angelo                   | 89  | Diablos                            | 100 | Kiros             |
| 2   | [Bite Bug](https://finalfantasy.fandom.com/wiki/Bite_Bug_(Final_Fantasy_VIII_card)) | 13  | Mesmerize    | 24  | Tri Face     | 35  | T Rexaur    | 46  | Chimera       | 57  | X ATM092       | 68  | Tri Point      | 79  | Gilgamesh                | 90  | Leviathan                          | 101 | Laguna            |
| 3   | [Red Bat](https://finalfantasy.fandom.com/wiki/Red_Bat_(card))                      | 14  | Glacial Eye  | 25  | Fastitocalon | 36  | Bomb        | 47  | PuPu          | 58  | Granaldo       | 69  | Gargantua      | 80  | MiniMog                  | 91  | Odin                               | 102 | Selphie           |
| 4   | [Blobra](https://finalfantasy.fandom.com/wiki/Blobra_(Final_Fantasy_VIII_card))     | 15  | Belhelmel    | 26  | Snow Lion    | 37  | Blitz       | 48  | Elastoid      | 59  | Gerogero       | 70  | Mobile Type 8  | 81  | Chicobo                  | 92  | Pandemona                          | 103 | Quistis           |
| 5   | Gayla                                                                               | 16  | Thrustaevis  | 27  | Ochu         | 38  | Wendigo     | 49  | GIM47N        | 60  | Iguion         | 71  | Sphinxara      | 82  | Quezacotl                | 93  | Cerberus                           | 104 | Irvine            |
| 6   | Gesper                                                                              | 17  | Anacondaur   | 28  | SAM08G       | 39  | Torama      | 50  | Malboro       | 61  | Abadon         | 72  | Tiamat         | 83  | Shiva                    | 94  | Alexander                          | 105 | Zell              |
| 7   | Fastitocalon F                                                                      | 18  | Creeps       | 29  | Death Claw   | 40  | Imp         | 51  | Ruby Dragon   | 62  | Trauma         | 73  | BGH251F2       | 84  | Ifrit                    | 95  | Phoenix                            | 106 | Rinoa             |
| 8   | Blood Soul                                                                          | 19  | Grendel      | 30  | Cactuar      | 41  | Blue Dragon | 52  | Elnoyle       | 63  | Oilboyle       | 74  | Red Giant      | 85  | Siren                    | 96  | Bahamut                            | 107 | Edea              |
| 9   | Caterchipillar                                                                      | 20  | Jelleye      | 31  | Tonberry     | 42  | Adamantoise | 53  | Tonberry King | 64  | Shumi Tribe    | 75  | Catoblepas     | 86  | Sacred                   | 97  | Doomtrain                          | 108 | Seifer            |
| 10  | Cockatrice                                                                          | 21  | Grand Mantis | 32  | Abyss Worm   | 43  | Hexadragon  | 54  | Biggs / Wedge | 65  | Krysta         | 76  | Ultima Weapon  | 87  | Minotaur                 | 98  | Eden                               | 109 | Squall            |

</div>

Each card mentioned above has its own 4-bit palette. Typically, a 4-bit palette contains 16 colors, with 240 unused
colors. In this file, the CLUT is arranged with 16 palettes per row. To read these palettes, I modify the TIM CLUT's
rows and columns to be 16x110. The card back is an exception, as it uses 8-bit color depth and utilizes the entire set
of 256 colors in its palette. The TIM file is embedded in the FF8.exe.

| ID  | Name      | Description                                                     |
|-----|-----------|-----------------------------------------------------------------|
| 110 | Card Back | 8 bit palette                                                   |
| 254 | Fail      |                                                                 |
| 255 | Immune    | For using in battle if a enemy is immune the value will be 0xff |


```{eval-rst}
.. doxygenenum::open viii::CardID
```