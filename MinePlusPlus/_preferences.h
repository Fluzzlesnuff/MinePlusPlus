//##############
//# GENERATION #
//##############
//#define GENERATE_ON_START //Skip the menu and go right to generating a Default world
//#define PRESET_SEED 189 //Set a seed to generate from, or comment to create a seed
//#define RENDER_WHILE_GENERATING //Show the map while generating. Slows down generation a lot.

//############
//# GAMEPLAY #
//############
#define PLAYER_SPEED 200

//###############
//# COMPILATION #
//###############
#define COMMANDS_ENABLED

//###########
//# LOGGING #
//###########
//Group logging settings enable the chosen logs. So disabling BLOCK_LOGGING disables all block logs, and enabling it enables whichever specific block logs are already enabled.
//"ALL" logging settings enable all logs in the group. So enabling ALL_BLOCK_LOGGING enables all block logs, but disabling still allows the other chosen logs.

//#define BLOCK_LOGGING
//#define ALL_BLOCK_LOGGING
//#define BLOCK_SET_LOGGING
//#define BLOCK_GET_LOGGING
//#define BLOCK_NEAR_TOUCHING_LOGGING
//#define BLOCK_DB_CREATION_LOGGING

//#define RENDER_LOGGING
//#define ALL_RENDER_LOGGING
//#define RENDER_BLOCK_LOGGING
//#define RENDER_ITEM_LOGGIN
//#define RENDER_MOB_LOGGING

//#define PLAYER_LOGGING
//#define ALL_PLAYER_LOGGING
//#define PLAYER_MOVE_LOGGING

//#define SAVE_LOAD_LOGGING
//#define ALL_SAVE_LOAD_LOGGING
//#define SAVE_LOAD_BYTE_LOGGING





























//DON'T TOUCH THESE! The following preproccessor macros enable and disable the logs. Just edit the preferences above.

#ifdef ALL_BLOCK_LOGGING
#define BLOCK_LOGGING
#define BLOCK_SET_LOGGING
#define BLOCK_GET_LOGGING
#define BLOCK_NEAR_TOUCHING_LOGGING
#define BLOCK_DB_CREATION_LOGGING
#endif

#ifndef BLOCK_LOGGING
#undef BLOCK_SET_LOGGING
#undef BLOCK_GET_LOGGING
#undef BLOCK_NEAR_TOUCHING_LOGGING
#undef BLOCK_DB_CREATION_LOGGING
#endif

#ifdef ALL_RENDER_LOGGING
#define RENDER_LOGGING
#define RENDER_BLOCK_LOGGING
#define RENDER_ITEM_LOGGING
#define RENDER_MOB_LOGGING
#endif

#ifndef RENDER_LOGGING
#undef RENDER_BLOCK_LOGGING
#undef RENDER_ITEM_LOGGING
#undef RENDER_MOB_LOGGING
#endif

#ifdef ALL_PLAYER_LOGGING
#define PLAYER_LOGGING
#define PLAYER_MOVE_LOGGING
#endif

#ifndef PLAYER_LOGGING
#undef PLAYER_MOVE_LOGGING
#endif

#ifdef ALL_SAVE_LOAD_LOGGING
#define SAVE_LOAD_LOGGING
#define SAVE_LOAD_BYTE_LOGGING
#endif

#ifndef SAVE_LOAD_LOGGING
#undef SAVE_LOAD_BYTE_LOGGING
#endif
