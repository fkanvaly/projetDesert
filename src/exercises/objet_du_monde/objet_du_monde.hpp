#pragma once

#include "../loadobj/loadobj.hpp"

//Arbre
vcl::mesh_drawable_hierarchy createArbre();

//Terrain
vcl::mesh create_grid(const gui_scene_structure& gui_scene);
float evalZ(float x, float y, float height, float noise);

//Cactus
vcl::mesh_drawable createCactus();
