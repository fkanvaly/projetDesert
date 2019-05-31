#pragma once

#include "../commonLib/commonLib.hpp"
#include "../terrain/terrain.hpp"
#include "../eau/eau.hpp"
#include "../../base_exercise/base_exercise.hpp"


struct animal_setting{
    // Data (p_i,t_i)
    std::vector<vcl::vec3> keyframe_position; // Given positions
    std::vector<float> keyframe_time;         // Time at given positions

    vcl::mesh_drawable surface;                            // moving point
    vcl::mesh_drawable sphere;                             // keyframe samples
    vcl::segment_drawable_immediate_mode segment_drawer;   // used to draw segments between keyframe samples
    vcl::curve_dynamic_drawable trajectory;

    unsigned int N;

    // Store the index of a selected sphere
    int picked_object;

    vcl::timer_interval animal_timer;
};




vcl::mesh_drawable_hierarchy create_oiseau();
vcl::mesh_drawable_hierarchy create_goat();
vcl::mesh_drawable_hierarchy create_croc();
vcl::mesh_drawable_hierarchy create_boat();
vcl::mesh_drawable_hierarchy create_snake_head();
vcl::mesh_drawable create_sprite();

animal_setting init_birdSet(gui_scene_structure& gui_scene);
animal_setting init_birdSet(const vcl::vec3 p, gui_scene_structure& gui_scene);
animal_setting init_goatSet(const vcl::vec3 p, gui_scene_structure& gui);
animal_setting init_snakeSet(const vcl::vec3 p, gui_scene_structure& gui);
animal_setting init_boatSet(const vcl::vec2 p, gui_scene_structure& gui_scene);
animal_setting init_sprite();


std::vector<vcl::vec3> circle_path(unsigned int N, const float R, float height);
std::vector<vcl::vec3> bird_path(gui_scene_structure& gui_scene, vcl::vec3 trans, unsigned int N, const float R);
std::vector<vcl::vec3> goat_path(unsigned int N, const float R , vcl::vec3 p, gui_scene_structure& gui);
std::vector<vcl::vec3> snake_path(unsigned int N, const float R , vcl::vec3 p, gui_scene_structure& gui);
std::vector<vcl::vec3> sprite_path(unsigned int N, vcl::vec3 p);
std::vector<vcl::vec3> boat_path(gui_scene_structure& gui_scene, vcl::vec2 pos, unsigned int N, const float R);

std::vector<float> create_keyTime(unsigned int N);
vcl::vec3 cardinal_spline_interpolation(float t, float t_1, float t0, float t1, float t2, const vcl::vec3& p_1, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2);
vcl::vec3 cardinal_spline_derivative_interpolation(float t, float t0, float t1, float t2, float t3, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2, const vcl::vec3& p3);
vcl::vec3 sinus_spline_interpolation(float t, float t0, float t1, float t2, float t3, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2, const vcl::vec3& p3);
vcl::vec3 cardinal_spline_interpolation(const animal_setting& birdSet, float t);
vcl::vec3 cardinal_spline_derivative_interpolation(const animal_setting& birdSet, float t);
vcl::vec3 sinus_spline_interpolation(const animal_setting& birdSet, float t);


