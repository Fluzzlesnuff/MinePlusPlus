#ifndef ids_h
#define ids_h

// #############
// # BLOCK IDS #
// #############

#define B_AIR           0x00
#define B_FIRE          0x01
#define B_DIRT          0x02
#define B_COBBLE        0x03
#define B_LAVA0         0x04
#define B_LAVA1         0x05
#define B_LAVA2         0x06
#define B_LAVA3         0x07
#define B_WATER0        0x08
#define B_WATER1        0x09
#define B_WATER2        0x0a
#define B_WATER3        0x0b
#define B_WATER4        0x0c
#define B_WATER5        0x0d
#define B_WATER6        0x0e
#define B_WATER7        0x0f
#define B_FARM0         0x10
#define B_FARM1         0x11
#define B_FARM2         0x12
#define B_FARM3         0x13
#define B_WHEAT0        0x14
#define B_WHEAT1        0x15
#define B_WHEAT2        0x16
#define B_WHEAT3        0x17
#define B_CARROT0       0x18
#define B_CARROT1       0x19
#define B_CARROT2       0x1a
#define B_CARROT3       0x1b
#define B_POTATO0       0x1c
#define B_POTATO1       0x1d
#define B_POTATO2       0x1e
#define B_POTATO3       0x1f
#define B_COAL_ORE      0x20
#define B_IRON_ORE      0x21
#define B_GOLD_ORE      0x22
#define B_DIA_ORE       0x23
#define B_DOOR_C        0x24
#define B_DOOR_O        0x25
#define B_TRAP_C        0x26
#define B_TRAP_O        0x27
#define B_STONE         0x28
#define B_SNDSTN        0x29
#define B_GRAVEL        0x2a
#define B_SAND          0x2b
#define B_WOOD          0x2c
#define B_PLANKS        0x2d
#define B_LEAVES        0x2e
#define B_OBSIDIAN      0x2f
#define B_GLASS         0x30
#define B_BRICKS        0x31
#define B_WOOL          0x32
#define B_GOLD_BLOCK    0x33
#define B_BED           0x34
#define B_LADDER        0x35
#define B_TORCH         0x36
#define B_SPONGE        0x37
#define B_CRAFT         0x38
#define B_FURNACE       0x39
#define B_CHEST         0x3a
#define B_GRASS         0x3b
#define B_SAPLING       0x3c
#define B_FLOWER        0x3d
#define B_TNT_U         0x3e
#define B_TNT_L         0x3f
#define B_WATER_SOURCE  0x40

// RUNTIME BLOCKS (Exist during regular world operation, but are not saved) [Overlap: Generation, Compression]
#define R_LIGHT0        0xc0
#define R_LIGHT1        0xc1
#define R_LIGHT2        0xc2
#define R_LIGHT3        0xc3
#define R_LIGHT4        0xc4
#define R_LIGHT5        0xc5
#define R_LIGHT6        0xc6
#define R_LIGHT7        0xc7
#define R_VOID          0xc8


// GENERATION BLOCKS (Only exist during generation stage) [Overlap: Update, Compression]
#define G_AIR       0xc0
#define G_T_AIR     0xc1
#define G_DIRT      0xc2
#define G_T_DIRT    0xc3
#define G_GRAVEL    0xc4
#define G_SAND      0xc5
#define G_T_SAND    0xc6
#define G_T_COAL    0xc7
#define G_T_IRON    0xc8
#define G_T_GOLD    0xc9
#define G_T_DIA     0xca
#define G_T_WOOD    0xcb
#define G_T_LEAVES  0xcc
#define G_T_SNDSTN  0xcd
#define G_T_GRASS   0xce

// UPDATE BLOCKS (Only exist during updating) [Overlap: Generation, Compression]
#define U_DEL_WATER0 0xe0
#define U_DEL_WATER1 0xe1
#define U_DEL_WATER2 0xe2
#define U_DEL_WATER3 0xe3
#define U_DEL_WATER4 0xe4
#define U_DEL_WATER5 0xe5
#define U_DEL_WATER6 0xe6
#define U_DEL_WATER7 0xe7

// COMPRESSION IDS
#define C_3_FLAG    0xfa //Indicates that the next 2 bytes are compressed
#define C_4_FLAG    0xfb //Indicates that the next 3 bytes are compressed
#define C_COMMA     0xfc //Separates two similar saved items, such as two chests or two mobs
#define C_SEMICOLON 0xfd //Separates two saving sections, such as pigs and zombies
#define C_PERIOD    0xfe //Separates two world save sections, such as between blocks and items
#define C_END       0xff //Will be deprecated once a map of the filesystem is added

// ############
// # ITEM IDS #
// ############

#define I_NONE                B00000000
#define I_GUNPOWDER           B00000001
#define I_DIRT                B00000010
#define I_COBBLE              B00000011
#define I_WHEAT               B00010100
#define I_SEEDS               B00010101
#define I_CARROT              B00011000
#define I_POTATO              B00011100
#define I_FLESH               B00011101
#define I_IRON_ORE            B00100001
#define I_GOLD_ORE            B00100010
#define I_DOOR                B00100100
#define I_TRAP                B00100110
#define I_STONE               B00101000
#define I_SNDSTN              B00101001
#define I_GRAVEL              B00101010
#define I_SAND                B00101011
#define I_WOOD                B00101100
#define I_PLANKS              B00101101
#define I_OBSIDIAN            B00101111
#define I_GLASS               B00110000
#define I_BRICKS              B00110001
#define I_WOOL                B00110010
#define I_GOLD_BLOCK          B00110011
#define I_BED                 B00110100
#define I_LADDER              B00110101
#define I_TORCH               B00110110
#define I_SPONGE              B00110111
#define I_CRAFT               B00111000
#define I_FURNACE             B00111001
#define I_CHEST               B00111010
#define I_SAPLING             B00111100
#define I_FLOWER              B00111101
#define I_TNT                 B00111110
#define I_SWORD_WOOD          B01000000
#define I_PICK_WOOD           B01000001
#define I_SHOVEL_WOOD         B01000010
#define I_AXE_WOOD            B01000011
#define I_HOE_WOOD            B01000100
#define I_SWORD_STONE         B01000101
#define I_PICK_STONE          B01000110
#define I_SHOVEL_STONE        B01000111
#define I_AXE_STONE           B01001000
#define I_HOE_STONE           B01001001
#define I_SWORD_GOLD          B01001010
#define I_PICK_GOLD           B01001011
#define I_SHOVEL_GOLD         B01001100
#define I_AXE_GOLD            B01001101
#define I_HOE_GOLD            B01001110
#define I_SWORD_IRON          B01001111
#define I_PICK_IRON           B01010000
#define I_SHOVEL_IRON         B01010001
#define I_AXE_IRON            B01010010
#define I_HOE_IRON            B01010011
#define I_SWORD_DIA           B01010100
#define I_PICK_DIA            B01010101
#define I_SHOVEL_DIA          B01010110
#define I_AXE_DIA             B01010111
#define I_HOE_DIA             B01011000
#define I_BOW                 B01011001
#define I_ARROW               B01011010
#define I_FLINT               B01011011
#define I_COAL                B01011100
#define I_GOLD_INGOT          B01011101
#define I_IRON_INGOT          B01011110
#define I_DIAMOND             B01011111
#define I_HELMET_LEATHER      B01100000
#define I_CHESTPLATE_LEATHER  B01100001
#define I_LEG_LEATHER         B01100010
#define I_BOOT_LEATHER        B01100011
#define I_HELMET_GOLD         B01100100
#define I_CHESTPLATE_GOLD     B01100101
#define I_LEG_GOLD            B01100110
#define I_BOOT_GOLD           B01100111
#define I_HELMET_IRON         B01101000
#define I_CHESTPLATE_IRON     B01101001
#define I_LEG_IRON            B01101010
#define I_BOOT_IRON           B01101011
#define I_HELMET_DIA          B01101100
#define I_CHESTPLATE_DIA      B01101101
#define I_LEG_DIA             B01101110
#define I_BOOT_DIA            B001101111
#define I_STICK               B01110000
#define I_LEATHER             B01110001
#define I_STRING              B01110010
#define I_BUCKET_E            B01110011
#define I_BUCKET_W            B01110100
#define I_BUCKET_L            B01110101
#define I_CHICKEN_R           B01110110
#define I_CHICKEN_C           B01110111
#define I_PORK_R              B01111000
#define I_PORK_C              B01111001
#define I_BEEF_R              B01111010
#define I_BEEF_C              B01111011
#define I_F_S                 B01111100
#define I_CHARCOAL            B01111101
#define I_BREAD               B01111110
#define I_FEATHER             B01111111

#endif
