# VisTrace Sunlight Extension

This extension adds sunlight simulation functionality to VisTrace for Garry's Mod, representing a 4:00 PM sun position.

## Features

- Accurate sun position calculation for 4:00 PM (western sky, 45-degree elevation)
- High-speed CPU-based ray tracing using VisTrace's AccelStruct
- Shadow detection through mesh and entity intersection
- Optimized performance for real-time use in Garry's Mod

## Usage

```lua
-- Load the extension in Garry's Mod
-- The extension automatically registers the SimulateSunlight function globally

-- Create an acceleration structure with world geometry
local accel = vistrace.CreateAccel({}, true) -- Include world geometry

-- Simulate sunlight at a point
local origin = Vector(100, 200, 50)
local result = SimulateSunlight(origin, accel, 1.0)

-- Check results
print("Sun direction:", result.sunDirection)
print("Light intensity:", result.intensity)
print("In shadow:", result.inShadow)
if result.distance then
    print("Distance to obstruction:", result.distance)
end
```

## Function Reference

### SimulateSunlight(origin, accel, [intensity])

Simulates sunlight at 4:00 PM hitting a specific point.

**Parameters:**
- `origin` (Vector): Point to trace sunlight from
- `accel` (AccelStruct): VisTrace acceleration structure for ray tracing
- `intensity` (number, optional): Sun intensity multiplier (default: 1.0)

**Returns:**
Table containing:
- `sunDirection` (Vector): Direction vector pointing towards the sun
- `intensity` (number): Light intensity (0 if in shadow, multiplied value if not)
- `inShadow` (boolean): Whether the point is in shadow
- `distance` (number, optional): Distance to first intersection if in shadow

## Sun Position Details

The 4:00 PM sun position is calculated as:
- **Azimuth**: 240° from north (west-southwest direction)
- **Elevation**: 45° above the horizon
- **Coordinates**: Converted to Source engine coordinate system (X=forward, Y=left, Z=up)

This represents a typical late afternoon sun position suitable for lighting calculations in Garry's Mod environments.

## Performance

The extension integrates with VisTrace's high-performance BVH acceleration structures for optimal ray tracing performance, making it suitable for real-time applications in Garry's Mod.