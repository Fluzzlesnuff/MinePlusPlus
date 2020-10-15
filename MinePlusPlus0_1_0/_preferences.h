//Group logging settings enable the chosen logs. So disabling BLOCK_LOGGING disables all block logs, and enabling it enables whichever specific block logs are already enabled.
//"ALL" logging settings enable all logs in the group. So enabling ALL_BLOCK_LOGGING enables all block logs, but disabling still allows the other chosen logs.

//#define BLOCK_LOGGING
//#define ALL_BLOCK_LOGGING
//#define BLOCK_SET_LOGGING
//#define BLOCK_GET_LOGGING
//#define BLOCK_NEAR_TOUCHING_LOGGING

//#define RENDER_LOGGING
//#define ALL_RENDER_LOGGING
//#define RENDER_BLOCK_LOGGING
//#define RENDER_ITEM_LOGGIN
//#define RENDER_MOB_LOGGING

//#define PLAYER_LOGGING
//#define ALL_PLAYER_LOGGING
//#define PLAYER_MOVE_LOGGING





























//DON'T TOUCH THESE! The following preproccessor macros enable and disable the logs. Just edit the preferences above.

#ifdef ALL_BLOCK_LOGGING
#define BLOCK_LOGGING
#define BLOCK_SET_LOGGING
#define BLOCK_GET_LOGGING
#define BLOCK_NEAR_TOUCHING_LOGGING
#endif

#ifndef BLOCK_LOGGING
#undef BLOCK_SET_LOGGING
#undef BLOCK_GET_LOGGING
#undef BLOCK_NEAR_TOUCHING_LOGGING
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
