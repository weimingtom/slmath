/*
NOTE:

1) This Lua test assumes <lua.h> can be found from include path and lua.lib from lib path

2) slmath must be compiled with SLMATH_LUA_SUPPORT (see slmath/slmath_configure.h)
*/

#include <stdio.h>
#include <slmath/slmath_lua.h>
#ifdef _MSC_VER
#include <direct.h>
#endif

extern "C" 
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}


int main( int argc, char* argv[] )
{
#ifdef _MSC_VER
	char buf[512];
	_getcwd( buf, sizeof(buf) );
	printf( "cwd: %s\n", buf );
#endif

	lua_State* lua = lua_open();
	luaL_openlibs( lua );

	luaopen_slmath( lua );

	int err = luaL_loadfile( lua, "runme.lua" );
	if ( err )
	{
		printf( "err: lua_loadfile failed\n" );
		return 1;
	}
	lua_call( lua, 0, 0 );

	lua_close( lua );
	return 0;
}
