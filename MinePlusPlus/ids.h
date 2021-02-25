#ifndef ids_h
#define ids_h

#include "datatypes.h"
// #############
// # BLOCK IDS #
// #############

const id_t B_AIR = 0x00;
const id_t B_FIRE = 0x01;
const id_t B_DIRT = 0x02;
const id_t B_COBBLE = 0x03;
const id_t B_LAVA0 = 0x04;
const id_t B_LAVA1 = 0x05;
const id_t B_LAVA2 = 0x06;
const id_t B_LAVA3 = 0x07;
const id_t B_WATER0 = 0x08;
const id_t B_WATER1 = 0x09;
const id_t B_WATER2 = 0x0a;
const id_t B_WATER3 = 0x0b;
const id_t B_WATER4 = 0x0c;
const id_t B_WATER5 = 0x0d;
const id_t B_WATER6 = 0x0e;
const id_t B_WATER7 = 0x0f;
const id_t B_FARM0 = 0x10;
const id_t B_FARM1 = 0x11;
const id_t B_FARM2 = 0x12;
const id_t B_FARM3 = 0x13;
const id_t B_WHEAT0 = 0x14;
const id_t B_WHEAT1 = 0x15;
const id_t B_WHEAT2 = 0x16;
const id_t B_WHEAT3 = 0x17;
const id_t B_CARROT0 = 0x18;
const id_t B_CARROT1 = 0x19;
const id_t B_CARROT2 = 0x1a;
const id_t B_CARROT3 = 0x1b;
const id_t B_POTATO0 = 0x1c;
const id_t B_POTATO1 = 0x1d;
const id_t B_POTATO2 = 0x1e;
const id_t B_POTATO3 = 0x1f;
const id_t B_COAL_ORE = 0x20;
const id_t B_IRON_ORE = 0x21;
const id_t B_GOLD_ORE = 0x22;
const id_t B_DIA_ORE = 0x23;
const id_t B_DOOR_C = 0x24;
const id_t B_DOOR_O = 0x25;
const id_t B_TRAP_C = 0x26;
const id_t B_TRAP_O = 0x27;
const id_t B_STONE = 0x28;
const id_t B_SNDSTN = 0x29;
const id_t B_GRAVEL = 0x2a;
const id_t B_SAND = 0x2b;
const id_t B_WOOD = 0x2c;
const id_t B_PLANKS = 0x2d;
const id_t B_LEAVES = 0x2e;
const id_t B_OBSIDIAN = 0x2f;
const id_t B_GLASS = 0x30;
const id_t B_BRICKS = 0x31;
const id_t B_WOOL = 0x32;
const id_t B_GOLD_BLOCK = 0x33;
const id_t B_BED = 0x34;
const id_t B_LADDER = 0x35;
const id_t B_TORCH = 0x36;
const id_t B_SPONGE = 0x37;
const id_t B_CRAFT = 0x38;
const id_t B_FURNACE = 0x39;
const id_t B_CHEST = 0x3a;
const id_t B_GRASS = 0x3b;
const id_t B_SAPLING = 0x3c;
const id_t B_FLOWER = 0x3d;
const id_t B_TNT_U = 0x3e;
const id_t B_TNT_L = 0x3f;
const id_t B_WATER_SOURCE = 0x40;

// RUNTIME BLOCKS (Exist during regular world operation, but are not saved) [Overlap: Generation, Compression]
const id_t R_LIGHT0 = 0xc0;
const id_t R_LIGHT1 = 0xc1;
const id_t R_LIGHT2 = 0xc2;
const id_t R_LIGHT3 = 0xc3;
const id_t R_LIGHT4 = 0xc4;
const id_t R_LIGHT5 = 0xc5;
const id_t R_LIGHT6 = 0xc6;
const id_t R_LIGHT7 = 0xc7;
const id_t R_VOID = 0xc8;


// GENERATION BLOCKS (Only exist during generation stage) [Overlap: Update, Compression]
const id_t G_AIR = 0xc0;
const id_t G_T_AIR = 0xc1;
const id_t G_DIRT = 0xc2;
const id_t G_T_DIRT = 0xc3;
const id_t G_GRAVEL = 0xc4;
const id_t G_SAND = 0xc5;
const id_t G_T_SAND = 0xc6;
const id_t G_T_COAL= 0xc7;
const id_t G_T_IRON = 0xc8;
const id_t G_T_GOLD = 0xc9;
const id_t G_T_DIA = 0xca;
const id_t G_T_WOOD = 0xcb;
const id_t G_T_LEAVES = 0xcc;
const id_t G_T_SNDSTN = 0xcd;
const id_t G_T_GRASS = 0xce;

// UPDATE BLOCKS (Only exist during updating) [Overlap: Generation, Compression]
const id_t U_DEL_WATER0 = 0xe0;
const id_t U_DEL_WATER1= 0xe1;
const id_t U_DEL_WATER2= 0xe2;
const id_t U_DEL_WATER3= 0xe3;
const id_t U_DEL_WATER4 = 0xe4;
const id_t U_DEL_WATER5 = 0xe5;
const id_t U_DEL_WATER6 = 0xe6;
const id_t U_DEL_WATER7 = 0xe7;

// COMPRESSION IDS
const id_t C_3_FLAG= 0xfa; //Indicates that the next 2 bytes are compressed
const id_t C_4_FLAG= 0xfb; //Indicates that the next 3 bytes are compressed
const id_t C_COMMA = 0xfc; //Separates two similar saved items, such as two chests or two mobs
const id_t C_SEMICOLON = 0xfd; //Separates two saving sections, such as pigs and zombies
const id_t C_PERIOD = 0xfe; //Separates two world save sections, such as between blocks and items
const id_t C_END= 0xff; //Will be deprecated once a map of the filesystem is added

// ############
// # ITEM IDS #
// ############

const id_t I_NONE=B00000000;
const id_t I_GUNPOWDER= B00000001;
const id_t I_DIRT=B00000010;
const id_t I_COBBLE = B00000011;
const id_t I_WHEAT =B00010100;
const id_t I_SEEDS= B00010101;
const id_t I_CARROT=B00011000;
const id_t I_POTATO= B00011100;
const id_t I_FLESH = B00011101;
const id_t I_IRON_ORE = B00100001;
const id_t I_GOLD_ORE = B00100010;
const id_t I_DOOR = B00100100;
const id_t I_TRAP = B00100110;
const id_t I_STONE = B00101000;
const id_t I_SNDSTN = B00101001;
const id_t I_GRAVEL = B00101010;
const id_t I_SAND = B00101011;
const id_t I_WOOD = B00101100;
const id_t I_PLANKS = B00101101;
const id_t I_OBSIDIAN = B00101111;
const id_t I_GLASS = B00110000;
const id_t I_BRICKS = B00110001;
const id_t I_WOOL = B00110010;
const id_t I_GOLD_BLOCK = B00110011;
const id_t I_BED = B00110100;
const id_t I_LADDER = B00110101;
const id_t I_TORCH = B00110110;
const id_t I_SPONGE = B00110111;
const id_t I_CRAFT = B00111000;
const id_t I_FURNACE = B00111001;
const id_t I_CHEST = B00111010;
const id_t I_SAPLING = B00111100;
const id_t I_FLOWER = B00111101;
const id_t I_TNT = B00111110;
const id_t I_SWORD_WOOD = B01000000;
const id_t I_PICK_WOOD = B01000001;
const id_t I_SHOVEL_WOOD = B01000010;
const id_t I_AXE_WOOD = B01000011;
const id_t I_HOE_WOOD = B01000100;
const id_t I_SWORD_STONE = B01000101;
const id_t I_PICK_STONE = B01000110;
const id_t I_SHOVEL_STONE = B01000111;
const id_t I_AXE_STONE = B01001000;
const id_t I_HOE_STONE = B01001001;
const id_t I_SWORD_GOLD = B01001010;
const id_t I_PICK_GOLD = B01001011;
const id_t I_SHOVEL_GOLD = B01001100;
const id_t I_AXE_GOLD = B01001101;
const id_t I_HOE_GOLD = B01001110;
const id_t I_SWORD_IRON = B01001111;
const id_t I_PICK_IRON = B01010000;
const id_t I_SHOVEL_IRON = B01010001;
const id_t I_AXE_IRON = B01010010;
const id_t I_HOE_IRON = B01010011;
const id_t I_SWORD_DIA = B01010100;
const id_t I_PICK_DIA = B01010101;
const id_t I_SHOVEL_DIA = B01010110;
const id_t I_AXE_DIA = B01010111;
const id_t I_HOE_DIA = B01011000;
const id_t I_BOW = B01011001;
const id_t I_ARROW = B01011010;
const id_t I_FLINT = B01011011;
const id_t I_COAL = B01011100;
const id_t I_GOLD_INGOT =B01011101;
const id_t I_IRON_INGOT =B01011110;
const id_t I_DIAMOND =B01011111;
const id_t I_HELMET_LEATHER =B01100000;
const id_t I_CHESTPLATE_LEATHER =B01100001;
const id_t I_LEG_LEATHER =B01100010;
const id_t I_BOOT_LEATHER =B01100011;
const id_t I_HELMET_GOLD =B01100100;
const id_t I_CHESTPLATE_GOLD =B01100101;
const id_t I_LEG_GOLD =B01100110;
const id_t I_BOOT_GOLD =B01100111;
const id_t I_HELMET_IRON =B01101000;
const id_t I_CHESTPLATE_IRON =B01101001;
const id_t I_LEG_IRON =B01101010;
const id_t I_BOOT_IRON =B01101011;
const id_t I_HELMET_DIA = B01101100;
const id_t I_CHESTPLATE_DIA = B01101101;
const id_t I_LEG_DIA = B01101110;
const id_t I_BOOT_DIA = B01101111;
const id_t I_STICK = B01110000;
const id_t I_LEATHER = B01110001;
const id_t I_STRING = B01110010;
const id_t I_BUCKET_E = B01110011;
const id_t I_BUCKET_W = B01110100;
const id_t I_BUCKET_L = B01110101;
const id_t I_CHICKEN_R = B01110110;
const id_t I_CHICKEN_C = B01110111;
const id_t I_PORK_R = B01111000;
const id_t I_PORK_C = B01111001;
const id_t I_BEEF_R = B01111010;
const id_t I_BEEF_C = B01111011;
const id_t I_F_S = B01111100;
const id_t I_CHARCOAL = B01111101;
const id_t I_BREAD =B01111110;
const id_t I_FEATHER =B01111111;

#endif
