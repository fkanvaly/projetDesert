#pragma once

#include "../../vcl/vcl.hpp"
#include "../../helper_scene.hpp"
#include "../../exercise_current.hpp"

struct gui_scene_structure
{
    bool wireframe   = false;
    bool skybox = false;

    float height = 1.4f;
    float scaling = 4.02f;
    int octave = 6;
    float persistency = 0.16f;

    bool cactus   = true;
    bool rock   = true;
    bool flower   = true;
    bool texture   = true;
};



#include <string>
#include <map>
#include <vector>


struct base_scene_exercise
{
    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);

    void mouse_click(scene_structure& scene, GLFWwindow* window, int button, int action, int mods);
    void mouse_move(scene_structure& scene, GLFWwindow* window);

};
