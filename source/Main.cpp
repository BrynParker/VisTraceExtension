#include "vistrace.h"

using namespace GarrysMod::Lua;
using namespace VisTrace;

// Called by VisTrace when everything is loaded (Change the extension name to something unique)
VISTRACE_EXTENSION_OPEN(ExampleExtension)
{
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "print");
	LUA->PushString("Extension loaded");
	LUA->Call(1, 0);
}

// Called when the module is unloaded by GMod
VISTRACE_EXTENSION_CLOSE()
{
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "print");
	LUA->PushString("Extension unloaded");
	LUA->Call(1, 0);
}
