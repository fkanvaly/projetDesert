
#include "oiseau.hpp"



vcl::mesh_drawable_hierarchy create_oiseau(){

    vcl::mesh_drawable_hierarchy bird;

    const float r_body = 0.9f;
    const float r_head = 0.55f;
    const float l_arm = 1.5f;

    vcl::mesh_drawable body = vcl::mesh_primitive_sphere(r_body,{0,0,0},40,40);
    body.uniform_parameter.scaling_axis={1,1,1.5};
    body.uniform_parameter.scaling_axis={1,0.8,1.5};

    vcl::mesh_drawable head = vcl::mesh_primitive_sphere(r_head,{0,0,0},40,40);

    vcl::mesh_drawable eye = vcl::mesh_primitive_sphere(0.08f,{0,0,0},20,20);
    eye.uniform_parameter.color = {0,0,0};

    vcl::mesh_drawable nose = vcl::mesh_primitive_cone(0.2f,{0,0,0},{0,0,0.5},20,10);
    nose.uniform_parameter.color = {1,0.1,0};

    vcl::mesh arm_top_left = vcl::mesh_primitive_quad({0,0,0},{l_arm,0,0},{l_arm,0,l_arm},{0,0,l_arm});
    vcl::mesh arm_top_right = vcl::mesh_primitive_quad({0,0,0},{-l_arm,0,0},{-l_arm,0,l_arm},{0,0,l_arm});

    vcl::mesh arm_bottom_left = vcl::mesh_primitive_quad({0,0,0},{l_arm,0,l_arm/3},{l_arm,0,2*l_arm/3},{0,0,l_arm});
    vcl::mesh arm_bottom_right = vcl::mesh_primitive_quad({0,0,0},{-l_arm,0,l_arm/3},{-l_arm,0,2*l_arm/3},{0,0,l_arm});


    bird.add_element(body, "body", "root");

    //Head
    bird.add_element(head, "head", "body",{0,0.5,1.7f*r_body});
    //bird.add_element(eye, "eye_left", "head",{r_head*std::sin(3.14*0.25)*std::cos(3.14*0.25),r_head*std::cos(3.14*0.25),r_head*cos(3.14*0.25)*cos(3.14*0.25)});
    //bird.add_element(eye, "eye_right", "head",{-r_head*sin(3.14*0.25)*std::cos(3.14*0.25),r_head*std::cos(3.14*0.25),r_head*cos(3.14*0.25)*cos(3.14*0.25)});
    //bird.add_element(nose,"nose","head",{0,0,r_head*0.9});
    //Arm
    //bird.add_element(arm_top_left, "arm_top_left", "body",{0.5*r_body,0,-0.7});
    //bird.add_element(arm_bottom_left, "arm_bottom_left", "arm_top_left",{l_arm,0,0});

    //bird.add_element(arm_top_right, "arm_top_right", "body",{-0.5*r_body,0,-0.7});
    //bird.add_element(arm_bottom_right, "arm_bottom_right", "arm_top_right",{-l_arm,0,0});

    return bird;
}


vcl::vec3 cardinal_spline_interpolation(float t, float t_1, float t0, float t1, float t2, const vcl::vec3& p_1, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2)
{
    float k=0.5;
    vcl::vec3 di= k*((t1-t0)/(t1-t_1))*(p1-p_1);
    vcl::vec3 di1= k*((t1-t0)/(t2-t0))*(p2-p0);
    float s=(t-t0)/(t1-t0);
    float s2=s*s;
    float s3=s*s*s;
    vcl::vec3 p=(2*s3-3*s2+1)*p0+(s3-2*s2+s)*di+(-2*s3+3*s2)*p1+(s3-s2)*di1;
    return p;
}

bird_settings init_birdSet(const float R){
    bird_settings bird_set;
    unsigned int N=100;
    bird_set.keyframe_position = circle_path(N,R,5.0f);
    //bird_set.keyframe_position = {{0,0,2}, {1,0,2}, {1,1,2}, {2,1,2}, {3,1,2}, {3,0,2}, {4,0,2}, {4,-1,2}, {0,-1,2}, {0,0,2}, {1,0,2}, {1,1,2}};
    bird_set.keyframe_time = create_keyTime(N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    bird_set.bird_timer.t_min = bird_set.keyframe_time[1];
    bird_set.bird_timer.t_max = bird_set.keyframe_time[bird_set.keyframe_time.size()-2];
    bird_set.bird_timer.t = bird_set.bird_timer.t_min;

    // Prepare the visual elements
    bird_set.surface = vcl::mesh_primitive_sphere();
    bird_set.surface.uniform_parameter.color   = {0,0,1};
    bird_set.surface.uniform_parameter.scaling = 0.08f;

    bird_set.sphere = vcl::mesh_primitive_sphere();
    bird_set.sphere.uniform_parameter.color = {1,1,1};
    bird_set.sphere.uniform_parameter.scaling = 0.05f;

    bird_set.segment_drawer.init();

    bird_set.trajectory = vcl::curve_dynamic_drawable(100); // number of steps stroed in the trajectory
    bird_set.trajectory.uniform_parameter.color = {0,0,1};

    bird_set.picked_object=-1;

    bird_set.bird_timer.scale=0.5;

    return bird_set;
}
std::vector<float> create_keyTime(unsigned int N){
    std::vector<float> keyTime;
    for (int i=0; i<N;i++) {
        keyTime.push_back(i);
    }
    return keyTime;
}

std::vector<vcl::vec3> circle_path(unsigned int N, const float R, float height){
    std::vector<vcl::vec3> path;
    float angle=0, toAdd=4*3.14/(N-1);
    for (int i=0; i<N ; i++) {
        path.push_back({R*std::cos(angle),R*std::sin(angle),height});
        angle+=toAdd;
        height+=0.1f;
    }

    return path;
}

