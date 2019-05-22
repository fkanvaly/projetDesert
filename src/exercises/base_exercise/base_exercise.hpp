#pragma once

#include "../../vcl/vcl.hpp"
#include "../../helper_scene.hpp"
#include "../../exercise_current.hpp"

struct gui_scene_structure
{
    bool wireframe   = false;
    bool skybox = false;

    //terrain
    float height = 0.67f;
    float scaling = 8.06f;
    int octave = 6;
    float persistency = 0.16f;

    //eau
    vcl::timer_interval eau_time;
    float eau_height = 0.37f;
    float eau_scaling = 3.8f;
    int eau_octave = 6;
    float eau_persistency = 0.0f;

    bool cactus   = true;
    bool rock   = true;
    bool flower   = true;
    bool texture   = true;

    bool enable_sprite = true;
    vcl::timer_event fire_timer;
};

struct fire_particle
{
    vcl::vec3 p;
    vcl::vec3 v;
    float t;
    std::vector<float> size;
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
