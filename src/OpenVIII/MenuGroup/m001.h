//
// Created by pcvii on 7/6/2020.
//

#ifndef VIIIARCHIVE_M001_H
#define VIIIARCHIVE_M001_H
#include <cstdint>
#include "OpenVIII/Strings/EncodedStringOffset.h"
struct m001 // Item to Item
{
  /*
   * Ability	    # of Entries  mngrp.bin Location	        Offset  Description
   * Recov Med-RF   9 Entries	  (mngrp.bin loc: 0x21F800)	0x0	Item to Recovery Items
   * ST Med-RF      12 Entries	  (mngrp.bin loc: 0x21F848)	0x48	Item to Status Removal Items
   * Ammo-RF	    16 Entries	  (mngrp.bin loc: 0x21F8A8)	0xA8	Item to Ammo Item
   * Forbid Med-RF  20 Entries	  (mngrp.bin loc: 0x21F928)	0x128	Item to Forbidden Medicine
   * GFRecov Med-RF 12 Entries	  (mngrp.bin loc: 0x21F9C8)	0x1C8	Item to GF Recovery Items
   * GFAbl Med-RF   42 Entries	  (mngrp.bin loc: 0x21FA28)	0x228	Item to GF Ability Medicine Items
   * Tool-RF	    32 Entries	  (mngrp.bin loc: 0x21FB78)	0x378	Item to Tool Items
   * */
};
#endif// VIIIARCHIVE_M001_H
