#ifndef rasterize_h
#define rasterize_h

#include <vector>

#include "Plane.h"
#include "Light.h"

//void draw_scene(std:":vector<Plane> scene, Uint32 *buffer, const Vec2f &dimensions, const Vec3f &transform, float *z_buffer);
void rasterize(Plane &plane, Uint32 *buffer, const Vec2f &dimensions, float *z_buffer, SDL_PixelFormat *pixel_format);
void draw_scene(std::vector<Plane> &scene, Uint32 *buffer, const Vec2f &dimensions, const Vec3f &transform, const Vec3f &rotate, float *z_buffer, SDL_PixelFormat *pixel_format, std::vector<Light> &lights);


#endif
