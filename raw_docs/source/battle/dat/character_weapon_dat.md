Character and Weapons DAT
======
[DAT](https://wiki.ffrtt.ru/index.php/FF8/FileFormat_DAT),
[Qhimm](https://forums.qhimm.com/index.php?topic=11137.msg154834)

1) The first character (d) represents a common prefix for all the files.
1) The following number (0-(9), a) represents a group identifier character id.
1) The next character (c or w) represents the type of model file, where "c" stands for character and "w" stands for
   weapon.
1) The three-digit number following the "c" or "w" represents a unique identifier for each model within its group, which
   could include different costumes or variations of a character.

The regular expression `^d[0-9aA]([cw]\d{3}\.dat)$`{l=regexp} matches filenames that start with "d", followed by 1
hexadecimal digits, followed by 'c' or 'w', followed by three digits, and
ending with ".dat"

Here is a summary of the groups, characters, and weapons:

* Group __0__ (Squall Leonhart):
    * Character Model: Default (000), Uniform (001)
    * Weapon Model: Revolver (000), Shear Trigger (001), Cutting Trigger (002), Flame Saber (003), Twin Lance (004),
      Punishment (005), Lion Heart (006)
* Group __1__ (Zell Dincht):
    * Character Model: Default (003), Uniform (004)
    * Weapon Model: Metal Knuckle (008), Maverick (009), Gauntlet (010), Ehrgeiz (011)
* Group __2__ (Irvine Kinneas):
    * Character Model: Default (006)
    * Weapon Model: Valiant (013), Ulysses (014), Bismarck (015), Exeter (016)
* Group __3__ (Quistis Trepe):
    * Character Model: Default (007)
    * Weapon Model: Chain Whip (018), Slaying Tail (019), Red Scorpion (020), Save the Queen (021)
* Group __4__ (Rinoa Heartilly):
    * Character Model: Default (009)
    * Weapon Model: Pinwheel (023), Valkyrie (024), Rising Sun (025), Cardinal (026), Shooting Star (027)
* Group __5__ (Selphie Tilmitt):
    * Character Model: Default (011), Uniform (012)
    * Weapon Model: Flail (028), Morning Star (029), Crescent Wish (030), Strange Vision (031)
* Group __6__ (Seifer Almasy):
    * Character Model: Default (014)
    * Weapon Model: (Hyperion) 033
* Group __7__ (Edea Kramer):
    * Character Model: Default (016)
    * Weapon Model: None
* Group __8__ (Laguna Loire):
    * Character Model: Default (017), Uniform (018)
    * Weapon Model: Machine Gun (035)
* Group __9__ (Kiros Seagill):
    * Character Model: Default (019), Uniform (020)
    * Weapon Model: Katal (037)
* Group __A__ _(10)_ (Ward Zabac):
    * Character Model: Default (021), Uniform (022)
    * Weapon Model: Harpoon (039)

```{eval-rst}
.. doxygenenum:: open_viii::CharacterID
    :project:
```

```{eval-rst}
.. doxygenenum:: open_viii::CharacterModelID
    :project:
```

```{eval-rst}
.. doxygenenum:: open_viii::WeaponModelID
    :project:
```