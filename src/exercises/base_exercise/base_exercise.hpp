#pragma once

#include "../../vcl/vcl.hpp"
#include "../../helper_scene.hpp"
#include "../../exercise_current.hpp"


struct gui_scene_structure
{
    bool wireframe   = false;
    bool skybox = true;
    bool camera_ownpath = false;

    //terrain
    float height = 0.67f;
    float scaling = 8.06f;
    int octave = 6;
    float persistency = 0.16f;

    bool path   = false;

    //bird path
    float path_height = 3.87f;
    float path_scaling = 3.8f;
    int path_octave = 6;
    float path_persistency = 0.0f;

    //eau
    vcl::timer_interval eau_time;
    float eau_height = 0.225f;
    float eau_scaling = 2.68f;
    int eau_octave = 6;
    float eau_persistency = 0.0f;

    vcl::timer_interval eau2_time;
    float eau2_height = 0.37f;
    float eau2_scaling = 3.8f;
    int eau2_octave = 6;
    float eau2_persistency = 0.0f;

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
    float t=0.0f;
    float R=0.0f;
    float size=0.2f;
};
struct particle_structure
{
    vcl::vec3 p0;
    vcl::vec3 v0;
    float t0=0.0f;
    float size=0.2f;
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
