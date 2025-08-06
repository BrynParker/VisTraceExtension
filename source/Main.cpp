#include "vistrace.h"
#include <cmath>

using namespace GarrysMod::Lua;
using namespace VisTrace;

// Constants for 4:00 PM sun position (western sky, moderate elevation)
constexpr float SUN_AZIMUTH_4PM = 240.0f;    // Degrees from north (west-southwest)
constexpr float SUN_ELEVATION_4PM = 45.0f;   // Degrees above horizon
constexpr float SUN_INTENSITY = 1.0f;        // Base sun intensity

// Convert degrees to radians
constexpr float DEG_TO_RAD = 3.14159265359f / 180.0f;

/*
	SimulateSunlight function
	
	Parameters:
	1. Vector origin - Point to trace sunlight from
	2. AccelStruct accel - Acceleration structure for ray tracing
	3. float intensity (optional) - Sun intensity multiplier (default 1.0)
	
	Returns:
	Table with:
	- sunDirection: Vector pointing towards the sun
	- intensity: Float representing light intensity (0 if in shadow, multiplied value if not)
	- inShadow: Boolean indicating if the point is in shadow
	- distance: Float indicating distance to first intersection (nil if no hit)
*/
LUA_FUNCTION(SimulateSunlight)
{
	// Check parameters
	LUA->CheckType(1, Type::Vector);
	
	Vector origin = LUA->GetVector(1);
	
	// Validate that second parameter has a Traverse method (duck typing approach)
	if (!LUA->IsType(2, Type::UserData)) {
		LUA->ArgError(2, "Expected AccelStruct");
	}
	
	// Check if the object has a Traverse method
	LUA->GetField(2, "Traverse");
	if (!LUA->IsType(-1, Type::Function)) {
		LUA->Pop(1);
		LUA->ArgError(2, "Expected AccelStruct with Traverse method");
	}
	LUA->Pop(1); // Pop the Traverse function
	
	// Get intensity parameter (optional)
	float intensity = SUN_INTENSITY;
	if (LUA->Top() > 2 && !LUA->IsType(3, Type::Nil)) {
		intensity = static_cast<float>(LUA->CheckNumber(3));
	}
	
	// Calculate sun direction for 4:00 PM
	float azimuthRad = SUN_AZIMUTH_4PM * DEG_TO_RAD;
	float elevationRad = SUN_ELEVATION_4PM * DEG_TO_RAD;
	
	// Convert spherical coordinates to Cartesian (sun direction)
	// In Source engine: X = forward, Y = left, Z = up
	float sunX = cos(elevationRad) * cos(azimuthRad);
	float sunY = cos(elevationRad) * sin(azimuthRad);
	float sunZ = sin(elevationRad);
	
	Vector sunDirection;
	sunDirection.x = sunX;
	sunDirection.y = sunY;
	sunDirection.z = sunZ;
	
	// Prepare stack for AccelStruct:Traverse call
	// Clear everything except the AccelStruct (parameter 2)
	while (LUA->Top() > 2) {
		LUA->Pop(1);
	}
	
	// Call AccelStruct:Traverse to check for shadows
	// Stack: origin, AccelStruct
	LUA->GetField(2, "Traverse");  // Stack: origin, AccelStruct, Traverse
	LUA->Push(2);                  // Stack: origin, AccelStruct, Traverse, AccelStruct
	LUA->PushVector(origin);       // Stack: origin, AccelStruct, Traverse, AccelStruct, origin
	LUA->PushVector(sunDirection); // Stack: origin, AccelStruct, Traverse, AccelStruct, origin, sunDirection
	LUA->PushNumber(0.01);        // Stack: origin, AccelStruct, Traverse, AccelStruct, origin, sunDirection, tMin
	LUA->PushNumber(10000.0);     // Stack: origin, AccelStruct, Traverse, AccelStruct, origin, sunDirection, tMin, tMax
	
	// Call Traverse function (self, origin, direction, tMin, tMax) -> returns TraceResult or nil
	LUA->Call(5, 1);              // Stack: origin, AccelStruct, result
	
	bool inShadow = !LUA->IsType(-1, Type::Nil);
	float finalIntensity = inShadow ? 0.0f : intensity;
	float hitDistance = 0.0f;
	
	if (inShadow) {
		// Get distance from TraceResult if we hit something
		LUA->GetField(-1, "GetPos");  // Stack: origin, AccelStruct, result, GetPos
		LUA->Push(-2);                // Stack: origin, AccelStruct, result, GetPos, result
		LUA->Call(1, 1);              // Stack: origin, AccelStruct, result, position
		
		if (LUA->IsType(-1, Type::Vector)) {
			Vector hitPos = LUA->GetVector(-1);
			float dx = hitPos.x - origin.x;
			float dy = hitPos.y - origin.y;
			float dz = hitPos.z - origin.z;
			hitDistance = sqrt(dx*dx + dy*dy + dz*dz);
		}
		LUA->Pop(1); // Pop position result  // Stack: origin, AccelStruct, result
	}
	
	LUA->Pop(3); // Pop result, AccelStruct, and origin  // Stack: empty
	
	// Create return table
	LUA->CreateTable();
	
	// sunDirection
	LUA->PushVector(sunDirection);
	LUA->SetField(-2, "sunDirection");
	
	// intensity
	LUA->PushNumber(finalIntensity);
	LUA->SetField(-2, "intensity");
	
	// inShadow
	LUA->PushBool(inShadow);
	LUA->SetField(-2, "inShadow");
	
	// distance (only if we hit something)
	if (inShadow) {
		LUA->PushNumber(hitDistance);
		LUA->SetField(-2, "distance");
	}
	
	return 1;
}

// Called by VisTrace when everything is loaded (Change the extension name to something unique)
VISTRACE_EXTENSION_OPEN(SunlightExtension)
{
	LUA->PushSpecial(SPECIAL_GLOB);
	
	// Register SimulateSunlight in the global namespace
	LUA->PushCFunction(SimulateSunlight);
	LUA->SetField(-2, "SimulateSunlight");
	
	LUA->GetField(-1, "print");
	LUA->PushString("VisTrace Sunlight Extension loaded - SimulateSunlight function available");
	LUA->Call(1, 0);
	LUA->Pop(1); // Pop global table
}

// Called when the module is unloaded by GMod
VISTRACE_EXTENSION_CLOSE()
{
	LUA->PushSpecial(SPECIAL_GLOB);
	LUA->GetField(-1, "print");
	LUA->PushString("Extension unloaded");
	LUA->Call(1, 0);
}
