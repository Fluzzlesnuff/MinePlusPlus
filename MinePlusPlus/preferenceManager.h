#ifndef prefmanager_h
#define prefmanager_h
#include "_preferences.h"

//DON'T TOUCH THESE! The following preproccessor macros enable and disable the logs. Just edit the preferences in _preferences.h.

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

#endif //Header guard
