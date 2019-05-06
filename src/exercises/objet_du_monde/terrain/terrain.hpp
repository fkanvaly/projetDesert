#pragma once

#include "../commonLib/commonLib.hpp"

//Terrain
vcl::mesh create_terrain(const gui_scene_structure& gui_scene);
float evaluate_terrain_z(float u, float v, const gui_scene_structure& gui_scene);
vcl::vec3 evaluate_terrain(float u, float v,const gui_scene_structure& gui_scene);
vcl::mesh_drawable create_underTerrain(const gui_scene_structure& gui_scene);
size_t index_at_value(float t, const std::vector<float>& vt);

static float radious = 8;
