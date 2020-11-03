# About IDs
Blocks and Item IDs are 8-bit values used to store data about blocks and items in the world. There are different types of IDs, and each has its own allocated number range. Some ranges may overlap, and some may not.

## Block IDs
Block IDs are used in the block database when a world is running, and are used to store the state of a world in persistent memory.
### Normal Blocks
These IDs exist in the range 0–191 (0x00–0xbf / B00000000–B101111111). This range is reserved, and no other block IDs can exist in this range. They represent normal blocks that can be seen, interacted with, and saved.
### Runtime Blocks
These IDs exist in the range 192–159 (0xc0–0x9f / B10000000–B10011111). This range can overlap with save-only (Compression) IDs, since a world will never be compressed and running simultaneously. They can also overlap with Generation IDs, because such trivial blocks are not used in world generation. They represent blocks which are derived from the environment, and can always be derived in the same way.
> Light blocks are used to prevent re-calculation of light values on every render, but they are saved as B_AIR, because the light system can be re-calculated on world load and update.
### Generation Blocks
These IDs exist in the range 192–239 (0xc0–0xef / B10000000–B10111111). This range can overlap with save-only (Compression) IDs, since a world will never be saved until generation is done, and these blocks are gone.

### Update Blocks

