#ifndef ids_h
#define ids_h

#include "datatypes.h"
namespace Blocks {
const id_t air = 0x00;
const id_t fire = 0x01;
const id_t dirt = 0x02;
const id_t cobblestone = 0x03;
const id_t lava0 = 0x04;
const id_t lava1 = 0x05;
const id_t lava2 = 0x06;
const id_t lava3 = 0x07;
const id_t water0 = 0x08;
const id_t water1 = 0x09;
const id_t water2 = 0x0a;
const id_t water3 = 0x0b;
const id_t water4 = 0x0c;
const id_t water5 = 0x0d;
const id_t water6 = 0x0e;
const id_t water7 = 0x0f;
const id_t farmland0 = 0x10;
const id_t farmland1 = 0x11;
const id_t farmland2 = 0x12;
const id_t farmland3 = 0x13;
const id_t wheat0 = 0x14;
const id_t wheat1 = 0x15;
const id_t wheat2 = 0x16;
const id_t wheat3 = 0x17;
const id_t carrot0 = 0x18;
const id_t carrot1 = 0x19;
const id_t carrot2 = 0x1a;
const id_t carrot3 = 0x1b;
const id_t potato0 = 0x1c;
const id_t potato1 = 0x1d;
const id_t potato2 = 0x1e;
const id_t potato3 = 0x1f;
const id_t coalOre = 0x20;
const id_t ironOre = 0x21;
const id_t goldOre = 0x22;
const id_t diamondOre = 0x23;
const id_t closedDoor = 0x24;
const id_t openDoor = 0x25;
const id_t closedTrapdoor = 0x26;
const id_t openTrapdoor = 0x27;
const id_t stone = 0x28;
const id_t sandstone = 0x29;
const id_t gravel = 0x2a;
const id_t sand = 0x2b;
const id_t log = 0x2c;
const id_t planks = 0x2d;
const id_t leaves = 0x2e;
const id_t obsidian = 0x2f;
const id_t glass = 0x30;
const id_t stoneBricks = 0x31;
const id_t wool = 0x32;
const id_t goldBlock = 0x33;
const id_t bed = 0x34;
const id_t ladder = 0x35;
const id_t torch = 0x36;
const id_t sponge = 0x37;
const id_t craftingTable = 0x38;
const id_t furnace = 0x39;
const id_t chest = 0x3a;
const id_t grass = 0x3b;
const id_t sapling = 0x3c;
const id_t flower = 0x3d;
const id_t tnt = 0x3e;
const id_t litTnt = 0x3f;
const id_t lavaSource = 0x40;
const id_t waterSource = 0x41;

namespace Runtime {
const id_t light0 = 0xc0;
const id_t light1 = 0xc1;
const id_t light2 = 0xc2;
const id_t light3 = 0xc3;
const id_t light4 = 0xc4;
const id_t light5 = 0xc5;
const id_t light6 = 0xc6;
const id_t light7 = 0xc7;
const id_t void = 0xc8;
}

namespace Generation {
const id_t air = 0xc0;
const id_t tempAir = 0xc1;
const id_t dirt = 0xc2;
const id_t tempDirt = 0xc3;
const id_t gravel = 0xc4;
const id_t sand = 0xc5;
const id_t tempSand = 0xc6;
const id_t coalOre = 0xc7;
const id_t ironOre = 0xc8;
const id_t goldOre = 0xc9;
const id_t diamondOre = 0xca;
const id_t log = 0xcb;
const id_t leaves = 0xcc;
const id_t sandstone = 0xcd;
const id_t grass = 0xce;
}

namespace Update {
const id_t deletedWater0 = 0xe0;
const id_t deletedWater1 = 0xe1;
const id_t deletedWater2 = 0xe2;
const id_t deletedWater3 = 0xe3;
const id_t deletedWater4 = 0xe4;
const id_t deletedWater5 = 0xe5;
const id_t deletedWater6 = 0xe6;
const id_t deletedWater7 = 0xe7;
}
namespace Storage { 
const id_t threeByteFlag = 0xfa; //Indicates that the next 2 bytes are compressed
const id_t fourByteFlag  = 0xfb; //Indicates that the next 3 bytes are compressed
const id_t comma = 0xfc; //Separates two similar saved items, such as two chests or two mobs
const id_t semicolon = 0xfd; //Separates two saving sections, such as pigs and zombies
const id_t period = 0xfe; //Separates two world save sections, such as between blocks and items
const id_t end = 0xff; //Will be deprecated once a map of the filesystem is added
}
}

namespace Items {
const id_t none = 0x00;
const id_t gunpowder = 0x01;
const id_t dirt = 0x02;
const id_t cobblestone = 0x03;
const id_t wheat = 0x14;
const id_t seeds = 0x15;
const id_t carrot = 0x18;
const id_t potato = 0x1c;
const id_t rottenFlesh = 0x1d;
const id_t ironOre = 0x21;
const id_t goldOre = 0x22;
const id_t door = 0x24;
const id_t trapdoor = 0x26;
const id_t stone = 0x28;
const id_t sandstone = 0x29;
const id_t gravel = 0x2a;
const id_t sand = 0x2b;
const id_t log = 0x2c;
const id_t planks = 0x2d;
const id_t obsidian = 0x2f;
const id_t glass = 0x30;
const id_t stoneBricks = 0x31;
const id_t wool = 0x32;
const id_t goldBlock = 0x33;
const id_t bed = 0x34;
const id_t ladder = 0x35;
const id_t torch = 0x36;
const id_t sponge = 0x37;
const id_t craftingTable = 0x38;
const id_t furnace = 0x39;
const id_t chest = 0x3a;
const id_t sapling = 0x3c;
const id_t flower = 0x3d;
const id_t tnt = 0x3e;
const id_t woodSword = 0x40;
const id_t woodPickaxe = 0x41;
const id_t woodShovel = 0x42;
const id_t woodAxe = 0x43;
const id_t woodHoe = 0x44;
const id_t stoneSword = 0x45;
const id_t stonePickaxe = 0x46;
const id_t stoneShovel = 0x47;
const id_t stoneAxe = 0x48;
const id_t stoneHoe = 0x49;
const id_t goldSword = 0x4a;
const id_t goldPickaxe = 0x4b;
const id_t goldShovel = 0x4c;
const id_t goldAxe = 0x4d;
const id_t goldHoe = 0x4e;
const id_t ironSword = 0x4f;
const id_t ironPickaxe = 0x50;
const id_t ironShovel = 0x51;
const id_t ironAxe = 0x52;
const id_t ironHoe = 0x53;
const id_t diamondSword = 0x54;
const id_t diamondPickaxe = 0x55;
const id_t diamondShovel = 0x56;
const id_t diamondAxe = 0x57;
const id_t diamondHoe = 0x58;
const id_t bow = 0x59;
const id_t arrow = 0x5a;
const id_t charcoal = 0x5b;
const id_t coal = 0x5c;
const id_t goldIngot = 0x5d;
const id_t ironIngot = 0x5e;
const id_t diamond = 0x5f;
const id_t leatherHelmet = 0x60;
const id_t leatherChestplate = 0x61;
const id_t leatherLeggings = 0x62;
const id_t leatherBoots = 0x63;
const id_t goldHelmet = 0x64;
const id_t goldChestplate = 0x65;
const id_t goldLeggings = 0x66;
const id_t goldBoots = 0x67;
const id_t ironHelmet = 0x68;
const id_t ironChestplate = 0x69;
const id_t ironLeggings = 0x6a;
const id_t ironBoots = 0x6b;
const id_t diamondHelmet = 0x6c;
const id_t diamondChestplate = 0x6d;
const id_t diamondLeggings = 0x6e;
const id_t diamondBoots = 0x6f;
const id_t stick = 0x70;
const id_t leather = 0x71;
const id_t string = 0x72;
const id_t bucket = 0x73;
const id_t waterBucket = 0x74;
const id_t lavaBucket = 0x75;
const id_t rawChicken = 0x76;
const id_t cookedChicken = 0x77;
const id_t rawPork = 0x78;
const id_t cookedPorkchop = 0x79;
const id_t rawBeef = 0x7a;
const id_t steak = 0x7b;
const id_t bread = 0x7c;
const id_t feather = 0x7d;
const id_t flint = 0x7e;
const id_t flintAndSteel = 0x7f;
}

#endif
