#include <vector>

#include "Plane.h"

void draw_scene(std::vector<Plane> scene, Uint32 *buffer, const Vec2f &dimensions, const Vec3f &transform, float *z_buffer);
void rasterize(Plane &plane, Uint32 *buffer, const Vec2f &dimensions, float *z_buffer);
