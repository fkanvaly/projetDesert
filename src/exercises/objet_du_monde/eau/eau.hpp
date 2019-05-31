#pragma once

#include "../commonLib/commonLib.hpp"

vcl::mesh create_eau(gui_scene_structure& gui_scene);
vcl::mesh create_eau2(gui_scene_structure& gui_scene);
float	z_eau (const vcl::vec2 p, gui_scene_structure& gui_scene);
float	z_eau2 (const vcl::vec2 p, gui_scene_structure& gui_scene);
float h(const float x, const float y, const float t);
vcl::mesh create_sousEau();
