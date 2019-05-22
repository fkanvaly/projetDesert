
#include "oiseau.hpp"

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

std::vector<float> create_keyTime(unsigned int N){
    std::vector<float> keyTime;
    for (int i=0; i<N+2;i++) {
        keyTime.push_back(i);
    }
    return keyTime;
}
//CREATER

vcl::mesh_drawable_hierarchy create_oiseau(){

    vcl::mesh_drawable_hierarchy bird;

    const float r_body = 0.9f;
    const float r_head = 0.55f;
    const float l_arm = 1.5f;

    //vcl::mesh_drawable body=load_obj("data/oiseau/corps_oiseau.obj","data/oiseau/corps_oiseau.mtl");
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
    bird.add_element(eye, "eye_left", "head",{r_head*std::sin(3.14*0.25)*std::cos(3.14*0.25),r_head*std::cos(3.14*0.25),r_head*cos(3.14*0.25)*cos(3.14*0.25)});
    bird.add_element(eye, "eye_right", "head",{-r_head*sin(3.14*0.25)*std::cos(3.14*0.25),r_head*std::cos(3.14*0.25),r_head*cos(3.14*0.25)*cos(3.14*0.25)});
    bird.add_element(nose,"nose","head",{0,0,r_head*0.9});
    //Arm
    bird.add_element(arm_top_left, "arm_top_left", "body",{0.5*r_body,0,-0.7});
    bird.add_element(arm_bottom_left, "arm_bottom_left", "arm_top_left",{l_arm,0,0});

    bird.add_element(arm_top_right, "arm_top_right", "body",{-0.5*r_body,0,-0.7});
    bird.add_element(arm_bottom_right, "arm_bottom_right", "arm_top_right",{-l_arm,0,0});

    return bird;
}

vcl::mesh_drawable_hierarchy create_goat(){
    vcl::mesh_drawable_hierarchy goat;
    vcl::mesh_drawable goat_body=load_obj("data/animals/goat.obj","data/animals/goat.mtl");
    vcl::mesh_drawable goat_eyes=load_obj("data/animals/goat_eyes.obj","data/animals/goat_eyes.mtl");
    goat.add_element(goat_body,"body","root");
    return goat;
}

vcl::mesh_drawable create_sprite(){
    vcl::mesh_drawable sprite = vcl::mesh_drawable(vcl::mesh_primitive_quad({-1,1,0},{-1,-1,0},{1,-1,0},{1,1,0}));
    sprite.uniform_parameter.scaling = 5.0f;

    return sprite;
}


//INIT

animal_setting init_birdSet(const float R){
    animal_setting bird_set;
    unsigned int N=100;
    //bird_set.keyframe_position = circle_path(N, 8.0, 5.0);
    bird_set.keyframe_position = sprite_path(N, {0,0,0});
    bird_set.keyframe_time = create_keyTime(N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    bird_set.animal_timer.t_min = bird_set.keyframe_time[1];
    bird_set.animal_timer.t_max = bird_set.keyframe_time[bird_set.keyframe_time.size()-2];
    bird_set.animal_timer.t = bird_set.animal_timer.t_min;

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

    bird_set.animal_timer.scale=2.3;

    return bird_set;
}

animal_setting init_goatSet(const vcl::vec3 p, gui_scene_structure& gui){
    animal_setting goat_set;
    unsigned int N=100;
    goat_set.keyframe_position = goat_path(N, 8.0, p, gui);
    goat_set.keyframe_time = create_keyTime(N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    goat_set.animal_timer.t_min = goat_set.keyframe_time[1];
    goat_set.animal_timer.t_max = goat_set.keyframe_time[goat_set.keyframe_time.size()-2];
    goat_set.animal_timer.t = goat_set.animal_timer.t_min;

    // Prepare the visual elements
    goat_set.surface = vcl::mesh_primitive_sphere();
    goat_set.surface.uniform_parameter.color   = {0,0,1};
    goat_set.surface.uniform_parameter.scaling = 0.08f;

    goat_set.sphere = vcl::mesh_primitive_sphere();
    goat_set.sphere.uniform_parameter.color = {1,1,1};
    goat_set.sphere.uniform_parameter.scaling = 0.05f;

    goat_set.segment_drawer.init();

    goat_set.trajectory = vcl::curve_dynamic_drawable(100); // number of steps stroed in the trajectory
    goat_set.trajectory.uniform_parameter.color = {0,0,1};

    goat_set.picked_object=-1;

    goat_set.animal_timer.scale=2.3;

    return goat_set;
}

animal_setting init_sprite(){
    animal_setting sprite_set;
    unsigned int N=100;
    sprite_set.keyframe_position = sprite_path(N, {0,0,0});
    sprite_set.keyframe_time = create_keyTime(N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    sprite_set.animal_timer.t_min = sprite_set.keyframe_time[1];
    sprite_set.animal_timer.t_max = sprite_set.keyframe_time[sprite_set.keyframe_time.size()-2];
    sprite_set.animal_timer.t = sprite_set.animal_timer.t_min;

    // Prepare the visual elements
    sprite_set.surface = vcl::mesh_primitive_sphere();
    sprite_set.surface.uniform_parameter.color   = {0,0,1};
    sprite_set.surface.uniform_parameter.scaling = 0.08f;

    sprite_set.sphere = vcl::mesh_primitive_sphere();
    sprite_set.sphere.uniform_parameter.color = {1,1,1};
    sprite_set.sphere.uniform_parameter.scaling = 0.05f;

    sprite_set.segment_drawer.init();

    sprite_set.trajectory = vcl::curve_dynamic_drawable(100); // number of steps stroed in the trajectory
    sprite_set.trajectory.uniform_parameter.color = {0,0,1};

    sprite_set.picked_object=-1;

    sprite_set.animal_timer.scale=2.3;

    return sprite_set;
}




//SOME PATH

std::vector<vcl::vec3> circle_path(unsigned int N, const float R, float height){
    std::vector<vcl::vec3> path;
    float angle=0, toAdd=2*M_PI/(N-1);
    for (int i=0; i<N+2 ; i++) {
        path.push_back({R*std::cos(angle),R*std::sin(angle),height});
        angle+=toAdd;
    }

    return path;
}

std::vector<vcl::vec3> goat_path(unsigned int N, const float R, vcl::vec3 p,gui_scene_structure& gui){
    std::vector<vcl::vec3> path;

    float angle=0,  offset=0.6;
    float toAdd=2*3.14/(N-1);
    for (int i=0; i<N+2 ; i++) {
        const float x =p.x + R*std::cos(angle);
        const float y =p.y + R*std::sin(angle);
        const float z =evaluate_terrain2_z(x,y,gui)+offset;
        path.push_back(vcl::vec3{x,y,z});
        angle+=toAdd;
    }


    return path;
}


std::vector<vcl::vec3> sprite_path(unsigned int N, vcl::vec3 p){
    std::vector<vcl::vec3> path;
    float angle=0, toAdd=20*M_PI/(N-1) , dz=0.2 , R=0, dr=0.1 , height=0.0f;
    for (int i=0; i<N+2 ; i++) {
        const float x =p.x + R*std::cos(angle);
        const float y =p.y + R*std::sin(angle);
        const float z =height;
        path.push_back({x,y,z});
        angle+=toAdd;
        R+=dr;
        height+=dz;
    }

    return path;
}

