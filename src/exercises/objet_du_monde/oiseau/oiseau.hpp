#pragma once

#include "../commonLib/commonLib.hpp"

struct bird_settings{

    // Data (p_i,t_i)
    std::vector<vcl::vec3> keyframe_position; // Given positions
    std::vector<float> keyframe_time;         // Time at given positions

    vcl::mesh_drawable surface;                            // moving point
    vcl::mesh_drawable sphere;                             // keyframe samples
    vcl::segment_drawable_immediate_mode segment_drawer;   // used to draw segments between keyframe samples
    vcl::curve_dynamic_drawable trajectory;                // Draw the trajectory of the moving point as a curve

    // Store the index of a selected sphere
    int picked_object;

    vcl::timer_interval bird_timer;

};

vcl::mesh_drawable_hierarchy create_oiseau();

bird_settings init_birdSet(const float R);

std::vector<float> create_keyTime(unsigned int N);
std::vector<vcl::vec3> circle_path(unsigned int N, const float R, float height);

vcl::vec3 cardinal_spline_interpolation(float t, float t_1, float t0, float t1, float t2, const vcl::vec3& p_1, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2);

//static size_t index_at_value(float t, const std::vector<float>& vt);
