#include "vistrace.h"

using namespace GarrysMod::Lua;
using namespace VisTrace;

VISTRACE_EXTENSION_OPEN(ExampleExtension)
{
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "print");
	LUA->PushString("Extension loaded");
	LUA->Call(1, 0);
}

VISTRACE_EXTENSION_CLOSE() {
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "print");
	LUA->PushString("Extension unloaded");
	LUA->Call(1, 0);
}
