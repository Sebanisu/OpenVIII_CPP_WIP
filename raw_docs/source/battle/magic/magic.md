Magic Files
======
[MAG](https://wiki.ffrtt.ru/index.php/FF8/FileFormat_magfiles)

1. The prefix "mag" is common in all filenames, possibly an abbreviation for "magic."
1. A three-digit number follows the prefix, potentially indicating different categories or types of magic spells and
   summons.
1. An underscore connects the number with a single character, which could represent different properties or attributes
   of the spells and summons.
1. A dot separates the character from a combination of letters and numbers that might indicate variations, sequences, or
   levels of the respective spells and summons.

### Regular Expressions

1. All Files:  
   `^mag\d{3}_[a-z]\.([0-9a-z]+|dat)$`{l=regexp}
1. Basename:
   `^mag\d{3}_[a-z]`{l=regexp}
1. Extension with Leading letter followed by 1 digit (e.g., h.0):
   `\.[a-z]\d$`{l=regexp}
1. Extension with Leading letter followed by 2 digits (e.g., b.10):
   `\.[a-z]\d{2}$`{l=regexp}
1. Extension with Leading digit followed by a letter and 1 digit (e.g., 1t0):
   `\.\d[a-z]\d$`{l=regexp}
1. Extension with Leading digit followed by a letter and 2 digits (e.g., 2s10):
   `\.\d[a-z]\d{2}$`{l=regexp}
1. Extension with ".dat":
   `\.dat$`{l=regexp}
1. Files that don't match the above start with "ma8def_p" (magic files?):
   `^ma8def_p\.\d$`{l=regexp}
