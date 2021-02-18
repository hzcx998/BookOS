#ifndef API_H
#define API_H
#if 0
#include "lib/lua52/lua.h"
#include "lib/lua52/lauxlib.h"
#include "lib/lua52/lualib.h"
#else
#include "lib/lua54/lua.h"
#include "lib/lua54/lauxlib.h"
#include "lib/lua54/lualib.h"
#endif
#define API_TYPE_FONT "Font"

void api_load_libs(lua_State *L);

#endif
