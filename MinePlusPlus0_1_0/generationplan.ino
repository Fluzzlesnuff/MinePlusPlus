/* Generation Process
    1a. Stone for bottom 1/2 of world
      b. Stone hills using random elevation deviances, working out from x=0
    2. Stone near (taxicab 3) air becomes B_DIRT *almost* always
    3a. Caves are seeded by setting GEN_AIR in random places to replace stone
      bi. Stone touching GEN_AIR becomes GEN_T_AIR (large chance)
        ii. GEN_T_AIR becomes GEN_AIR
      ci. Same, but smaller chance for touching 1, larger chances for touching multiple
        ii. GEN_T_AIR becomes GEN_AIR
      d. Repeat c multiple times
      ei. Stone touching 7 or 8 GEN_AIR becomes GEN_T_AIR 100%
        ii. GEN_T_AIR becomes GEN_AIR
    5a. Gravel veins are seeded randomly with GEN_GRAVEL
      bi. Stone directly touching GEN_GRAVEL has a small chance of becoming GEN_T_GRAVEL
        ii. GEN_T_GRAVEL becomes GEN_GRAVEL
      c. Repeat a few times (calibrate cycles and probabilities so veins are about 8-12 blocks)
      d. Stone directly touching 3 GEN_GRAVEL becomes GEN_T_GRAVEL
      e. GEN_GRAVEL and GEN_T_GRAVEL become B_GRAVEL
    6a. Dirt veins are seeded randomly above 1/4 depth using GEN_DIRT
      bi. Stone directly touching GEN_DIRT has a small chance of becoming GEN_T_DIRT
        ii. GEN_T_DIRT becomes GEN_DIRT
      c. Repeat a few times (calibrate cycles and probabilities so veins are about 6-9 blocks)
      d. GEN_DIRT becomes B_DIRT
    7a. Diamond veins are seeded in the the bottom 4 blocks of the world as B_DIA_ORE (Ores don't need GEN versions), with a better chance near GEN_AIR
      bi. Stone directly touching (isTouching()) B_DIA_ORE has a large chance of becoming GEN_T_DIA
        ii. GEN_T_DIA becomes B_DIA_ORE
    8a. Iron veins are seeded in the the bottom 1/3 of the world as B_IRON_ORE (Ores don't need GEN versions), with a better chance near GEN_AIR
      bi. Stone touching (isTouchingWide()) B_IRON_ORE has a large chance of becoming GEN_T_IRON
        ii. GEN_T_IRON becomes B_IRON_ORE
    9a. Gold veins are seeded in the the bottom 1/3 of the world as B_GOLD_ORE (Ores don't need GEN versions), with a better chance near GEN_AIR
      bi. Stone touching (isTouchingWide()) B_GOLD_ORE has a small chance of becoming GEN_T_GOLD
        ii. GEN_T_GOLD becomes B_GOLD_ORE
    10a. Coal veins are seeded in the the bottom 1/2 of the world as B_COAL_ORE (Ores don't need GEN versions), with a better chance near GEN_AIR
      bi. Stone touching (isTouchingWide()) B_COAL_ORE has a large chance of becoming GEN_T_COAL
        ii. GEN_T_IRON becomes B_IRON_ORE
      bi. Stone directly touching (isTouching()) B_COAL_ORE has a small chance of becoming GEN_T_COAL
        ii. GEN_T_IRON becomes B_IRON_ORE
    11a. GEN_AIR below y=7 has a tiny chance of becoming B_LAVA3
      b. GEN_AIR below y=7 touching B_LAVA3 becomes B_LAVA3
    12a. GEN_AIR in the bottom 1/3 of the world that has stone below it has a tiny chance of becoming B_WATER7
      b. GEN_AIR in the bottom 1/3 of the world touching B_WATER7 becomes B_WATER7
    13. GEN_AIR becomes B_AIR, now that underground generation is done (finally!)
    14a. Air below (inclusive) (1/2 world + 3 blocks) and above (1/2 world) has a tiny chance of becoming B_WATER7
      b. Air in that region touching B_WATER7 becomes B_WATER7
      c. Stone and dirt above (1/2 world - 2) and within 2 blocks (taxicab) of water becomes B_SAND
    15a. Dirt not within 4 blocks (wide) of water and above (1/2 world - 3) has a tiny chance of becoming GEN_SAND
      bi. Dirt touching (taxicab) GEN_SAND or B_SNDSTN becomes GEN_T_SAND and stone above (1/2 world -5) touching (taxicab) GEN_SAND or B_SNDSTN becomes GEN_T_SNDSTN
        ii. GEN_T_SAND becomes GEN_SAND and GEN_T_SNDSTN becomes B_SNDSTN
      c. Repeat (b) 2 more times
      di. Dirt touching (taxicab) GEN_SAND or B_SNDSTN has a small chance of becoming GEN_T_SAND and stone above (1/2 world -5) touching (taxicab) GEN_SAND or B_SNDSTN has a small chance of beoming GEN_T_SNDSTN
        ii. GEN_T_SAND becomes GEN_SAND and GEN_T_SNDSTN becomes B_SNDSTN
    16a. Air open to the sky with dirt below has a small chance of becoming B_WOOD
      bi. Air with B_WOOD below and B_DIRT within 2 blocks down turns into GEN_T_WOOD
        ii. GEN_T_WOOD becomes B_WOOD
      ci. Air with B_WOOD below and dirt within 3 blocks down turns into GEN_WOOD
        ii. GEN_T_WOOD becomes B_WOOD
      di. Air with B_WOOD below and dirt within 4 blocks down has a large chance of becoming GEN_WOOD
        ii. GEN_T_WOOD becomes B_WOOD
      ei. Air with B_WOOD below and dirt within 5 blocks down has a small chance of becoming GEN_WOOD
        ii. GEN_T_WOOD becomes B_WOOD
    17a. Air touching (isTouchingWide) B_WOOD and not touching (isTouchingWide) dirt becomes B_LEAVES
      bi. Air directly touching B_LEAVES and with at least 2 B_WOOD to its left or right becomes GEN_T_LEAVES
        ii. GEN_T_LEAVES becomes B_LEAVES
      ci. Air touching (isTouchingWide) at least 2 B_LEAVES and not touching (isTouchingWide) dirt has a small chance of becoming GEN_T_LEAVES
        ii. GEN_T_LEAVES becomes B_LEAVES
    18a. Air with dirt below it has a small chance of becoming B_GRASS.
        bi. Air with dirt below it touching (taxicab) B_GRASS has a chance of becoming GEN_T_GRASS
          ii. GEN_T_GRASS becomes B_GRASS
        c. B_GRASS has a small chance of becoming B_FLOWER
*/

/* GEN and GEN_T Blocks
   GEN (generation control) and GEN_T (temporary generation control) blocks are blocks used in generation to prevent steps acting on themselves.
   For example, when generating caves, more air is added to GEN_AIR. If it was added directly to B_AIR,
   the surface dirt  and stone would be eroded down to nothing, because there's air on the surface.
   Instead, caves are made up of GEN_AIR to distinguish between caves and surface air.
   An example of GEN_T: When air is added to caves, it is added as GEN_T_AIR. This is to prevent the
   line of generation going up the world from adding air on top of air that was already added on the
   current cycle.

   Usage: GEN_T is used for each cycle of generation and becomes the actual block or the GEN version
   at the ned of said cycle. GEN is used for the entire generation phase, and is converted to the actual
   block at the end of the phase.

   TL;DR: GEN and GEN_T blocks are temporary, and are used on generation to prevent the worldwide destruction.
*/
