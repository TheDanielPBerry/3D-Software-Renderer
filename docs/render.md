Friday November 8, 2024
Calculate projections for all 3 points of a plane and store alongside their 3d counterpart instead of just mapping to 2d space.

When rasterizing the triangle on the 2d screen calculate the z position of the pixel.
If in frustum, then compare with the zbuffer. If it's smaller than the current pixel, then replace it.

Don't scale to the width/height of the buffer until as late as possible. This should give us some room when dealing with buffers
This should give us some more wiggle room when we try to sample textures.

- [x] Update colors down to Uint32 across the board as a struct. 

- [x] Probably when calculating each pixels z-buffer only use the squared distance to save time on the sqrt function

```c++
typedef struct Vec2f {
    float x;
    float y;
} Vec2f;

typedef struct Vec3f {
    float x;
    float y;
    float z;
} Vec3f;

typedef struct Plane {
    Vec3f points[3];
    Vec3f buffer[3];
    Uint32 color;
    Vec2f texture_coords;
} Plane;
```


Saturday November 9, 2024
- [ ] Calculate luminosity at each vertex on the triangle
- [ ] Break color back down into separate rgb values - Should be more efficient than breaking down on every draw
- [ ] Build out a function for generating coefficients of functions based on set of 2-d points

3 3-d vertices per vertex
1 for model space
1 for transform space
1 for 2d/buffer space
Pipeline goes from top to bottom

1 2d vertex per vertice for texture coordinates
We can interpolate textures the same way we did with the z buffer

1 float for luminosity


Ways this is better so far than the 3d renderer I built in high school
- optimizations regarding cache efficiency and lowering dynamic memory allocations
- Ability to do per pixel z-distance calculations and calculate per pixel lighting
- Vertices behind frustum are being cut, so no rubber banding around camera (Big Win)
