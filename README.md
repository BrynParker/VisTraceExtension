Key Features

    Accurate Sun Position: Calculates 4:00 PM sun position at 240° azimuth (west-southwest) and 45° elevation above horizon
    High-Performance Ray Tracing: Integrates with VisTrace's CPU-based AccelStruct for efficient mesh and entity intersection testing
    Shadow Detection: Performs ray tracing from any point toward the sun to determine if the point is in shadow
    Lua Integration: Exposes SimulateSunlight function globally in Garry's Mod scripts

Usage Example

-- Create acceleration structure with world geometry
local accel = vistrace.CreateAccel({}, true)

-- Simulate sunlight at a specific point
local origin = Vector(100, 200, 50)
local result = SimulateSunlight(origin, accel, 1.0)

-- Access lighting information
print("Sun direction:", result.sunDirection)
print("Light intensity:", result.intensity)  -- 0 if in shadow
print("In shadow:", result.inShadow)
if result.distance then
    print("Distance to obstruction:", result.distance)
end

Technical Implementation

The function performs the following operations:

    Sun Direction Calculation: Converts spherical coordinates (azimuth 240°, elevation 45°) to a unit vector in Source engine coordinate system
    Ray Tracing: Uses AccelStruct:Traverse to cast a ray from the origin point toward the sun direction
    Shadow Analysis: Determines lighting intensity based on whether the ray intersects with geometry before reaching the sky
    Result Packaging: Returns comprehensive lighting data including direction, intensity, shadow status, and obstruction distance

Validation

    Sun position calculation verified mathematically to produce correct unit vector
    Input validation ensures robust error handling for invalid parameters
    Duck typing approach for AccelStruct compatibility without breaking encapsulation
    Build system integration maintains compatibility with existing VisTrace module

This implementation provides game developers with an easy-to-use tool for realistic afternoon lighting calculations in Garry's Mod environments, optimized for real-time performance.
