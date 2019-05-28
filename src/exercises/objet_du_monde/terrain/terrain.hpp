#pragma once

#include "../commonLib/commonLib.hpp"

//Terrain
vcl::mesh create_terrain(const gui_scene_structure& gui_scene);
float evaluate_terrain_z(float u, float v, const gui_scene_structure& gui_scene);
float evaluate_terrain2_z(float x, float y, const gui_scene_structure& gui_scene);
vcl::vec3 evaluate_terrain(float u, float v,const gui_scene_structure& gui_scene);
vcl::mesh_drawable create_underTerrain(const gui_scene_structure& gui_scene);
size_t index_at_value(float t, const std::vector<float>& vt);
float evaluate_terrain_z2(float x, float y, float z);
float evaluate_terrain_z3(float x, float y, float z);

static float radious = 10;
