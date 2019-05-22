#pragma once

#include "../commonLib/commonLib.hpp"

vcl::mesh create_eau(const gui_scene_structure& gui_scene);
float	z_eau (const float x, const float y, const gui_scene_structure& gui_scene);
float h(const float x, const float y, const float t);

