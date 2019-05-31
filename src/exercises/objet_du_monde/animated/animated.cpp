
#include "animated.hpp"

vcl::vec3 rotate(const vcl::vec3& p, const vcl::vec3& k, float theta);

vcl::vec3 cardinal_spline_interpolation(float t, float t0, float t1, float t2, float t3, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2, const vcl::vec3& p3)
{
    const float sigma = t2-t1;

    const vcl::vec3 d1 = (p2-p0)/(t2-t0) * sigma;
    const vcl::vec3 d2 = (p3-p1)/(t3-t1) * sigma;

    const float s = (t-t1)/sigma;
    const float s2 = s*s;
    const float s3 = s2*s;

    const vcl::vec3 p = (2*s3-3*s2+1)*p1 + (s3-2*s2+s)*d1 + (-2*s3+3*s2)*p2 + (s3-s2)*d2;

    return p;
}

vcl::vec3 cardinal_spline_derivative_interpolation(float t, float t0, float t1, float t2, float t3, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2, const vcl::vec3& p3)
{
    const float sigma = t2-t1;

    const vcl::vec3 d1 = (p2-p0)/(t2-t0) * sigma;
    const vcl::vec3 d2 = (p3-p1)/(t3-t1) * sigma;

    const float s = (t-t1)/sigma;
    const float s2 = s*s;

    const vcl::vec3 p = (6*s2-6*s)*p1 + (3*s2-4*s+1)*d1 + (-6*s2+6*s)*p2 + (3*s2-2*s)*d2;

    return p;
}

vcl::vec3 sinus_spline_interpolation(float t, float t0, float t1, float t2, float t3, const vcl::vec3& p0, const vcl::vec3& p1, const vcl::vec3& p2, const vcl::vec3& p3)
{
    float pi = 3.1415f;
    float A = ((4*pi)/(t2-t1));
    float B = -A*t1;
    float amplitude = 0.1f;
    vcl::vec3 oscillation = {0,amplitude*sin(A*t+B),0};
    float costeta = (p2-p1).x/norm(p2-p1);
    float sinteta = (p2-p1).y/norm(p2-p1);
    vcl::mat3 rot {costeta,-sinteta,0,sinteta,costeta,0,0,0,0};

    const float sigma = t2-t1;

    const vcl::vec3 d1 = (p2-p0)/(t2-t0) * sigma;
    const vcl::vec3 d2 = (p3-p1)/(t3-t1) * sigma;

    const float s = (t-t1)/sigma;
    const float s2 = s*s;
    const float s3 = s2*s;

    vcl::vec3 p = (2*s3-3*s2+1)*p1 + (s3-2*s2+s)*d1 + (-2*s3+3*s2)*p2 + (s3-s2)*d2;
    p += rot*oscillation;

    return p;
}

vcl::vec3 cardinal_spline_interpolation(const animal_setting& birdSet, float t)
{

    const size_t idx = index_at_value(t, birdSet.keyframe_time);

    const float t0 = birdSet.keyframe_time[idx-1];
    const float t1 = birdSet.keyframe_time[idx];
    const float t2 = birdSet.keyframe_time[idx+1];
    const float t3 = birdSet.keyframe_time[idx+2];

    const vcl::vec3& p0 = birdSet.keyframe_position[idx-1];
    const vcl::vec3& p1 = birdSet.keyframe_position[idx];
    const vcl::vec3& p2 = birdSet.keyframe_position[idx+1];
    const vcl::vec3& p3 = birdSet.keyframe_position[idx+2];

    const vcl::vec3 p = cardinal_spline_interpolation(t,t0,t1,t2,t3,p0,p1,p2,p3);
    return p;
}

vcl::vec3 cardinal_spline_derivative_interpolation(const animal_setting& birdSet, float t)
{
    const size_t idx = index_at_value(t, birdSet.keyframe_time);

    const float t0 = birdSet.keyframe_time[idx-1];
    const float t1 = birdSet.keyframe_time[idx];
    const float t2 = birdSet.keyframe_time[idx+1];
    const float t3 = birdSet.keyframe_time[idx+2];

    const vcl::vec3& p0 = birdSet.keyframe_position[idx-1];
    const vcl::vec3& p1 = birdSet.keyframe_position[idx];
    const vcl::vec3& p2 = birdSet.keyframe_position[idx+1];
    const vcl::vec3& p3 = birdSet.keyframe_position[idx+2];

    const vcl::vec3 p = cardinal_spline_derivative_interpolation(t,t0,t1,t2,t3,p0,p1,p2,p3);
    return p;
}

vcl::vec3 sinus_spline_interpolation(const animal_setting& birdSet, float t)
{

    const size_t idx = index_at_value(t, birdSet.keyframe_time);

    const float t0 = birdSet.keyframe_time[idx-1];
    const float t1 = birdSet.keyframe_time[idx];
    const float t2 = birdSet.keyframe_time[idx+1];
    const float t3 = birdSet.keyframe_time[idx+2];

    const vcl::vec3& p0 = birdSet.keyframe_position[idx-1];
    const vcl::vec3& p1 = birdSet.keyframe_position[idx];
    const vcl::vec3& p2 = birdSet.keyframe_position[idx+1];
    const vcl::vec3& p3 = birdSet.keyframe_position[idx+2];

    const vcl::vec3 p = sinus_spline_interpolation(t,t0,t1,t2,t3,p0,p1,p2,p3);
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


    vcl::mesh_drawable body=load_obj("data/animals/bird_Body.obj");
    //body.uniform_parameter.shading={1,1,0.2};
    body.uniform_parameter.color={0.8,0.8,0.8};

    vcl::mesh_drawable arm_top_left=load_obj("data/animals/bird_lArm.obj");
    arm_top_left.uniform_parameter.color={0.4,0.4,0.4};
    arm_top_left.uniform_parameter.shading.specular=0;
    vcl::mesh_drawable arm_top_right=load_obj("data/animals/bird_rArm.obj");
    arm_top_right.uniform_parameter.color={0.4,0.4,0.4};
    arm_top_right.uniform_parameter.shading.specular=0;
    vcl::mesh_drawable eyes=load_obj("data/animals/bird_eyes.obj","data/animals/bird_eyes.mtl");

    bird.add_element(body, "body", "root");
    //Head
    bird.add_element(eyes, "eyes", "body",{0,0,0});
    //Arm
    bird.add_element(arm_top_left, "arm_top_left", "body",{0,0,0});

    bird.add_element(arm_top_right, "arm_top_right", "body",{0,0,0});

    return bird;
}

vcl::mesh_drawable_hierarchy create_croc(){

    vcl::mesh_drawable_hierarchy croc;

    vcl::mesh_drawable body=load_obj("data/animals/croc/body.obj", "data/animals/croc/body.mtl" );

    vcl::mesh_drawable foot=load_obj("data/animals/croc/foot.obj", "data/animals/croc/foot.mtl");

    vcl::mesh_drawable eyes=load_obj("data/animals/croc/eye.obj", "data/animals/croc/eye.mtl");

    vcl::mesh_drawable dents=load_obj("data/animals/croc/dent.obj");


    croc.add_element(body, "body", "root");
    croc.add_element(eyes, "eyes", "body",{0,0,0});
    croc.add_element(foot, "foot", "body",{0,0,0});
    croc.add_element(dents, "dents", "body",{0,0,0});

    croc.scaling=0.4;

    return croc;
}

vcl::mesh_drawable_hierarchy create_boat(){

    vcl::mesh_drawable_hierarchy boat;

    vcl::mesh_drawable body=load_obj("data/boat/voile.obj", "data/boat/voile.mtl" );

    vcl::mesh_drawable mat=load_obj("data/boat/mat.obj", "data/boat/mat.mtl");

    vcl::mesh_drawable foc=load_obj("data/boat/foc.obj", "data/boat/foc.mtl");

    vcl::mesh_drawable corde=load_obj("data/boat/cordeobj.obj", "data/boat/cordeobj.mtl");


    boat.add_element(body, "body", "root");
    boat.add_element(mat, "eyes", "body");
    boat.add_element(foc, "foot", "body");
    boat.add_element(corde, "dents", "body");

    boat.scaling=0.5;

    return boat;
}

vcl::mesh_drawable_hierarchy create_goat(){
    vcl::mesh_drawable_hierarchy goat;
    vcl::mesh_drawable goat_body=load_obj("data/animals/goat.obj","data/animals/goat.mtl");
    vcl::mesh_drawable goat_eyes=load_obj("data/animals/goat_eyes.obj","data/animals/goat_eyes.mtl");
    goat.add_element(goat_body,"body","root");
    return goat;
}

vcl::mesh_drawable_hierarchy create_snake_head(){
    vcl::mesh_drawable_hierarchy snake_head;
    vcl::mesh_drawable head = load_obj("data/snake/head.obj");
    head.uniform_parameter.color ={147./255, 94./255, 0./255};
    head.uniform_parameter.shading.specular=0;
    vcl::mesh_drawable eyes = load_obj("data/snake/eyes.obj","data/snake/eyes.mtl");
    snake_head.add_element(head, "head", "root");
    snake_head.add_element(eyes, "eyes","head");
    snake_head.scaling = 1.6;

    return snake_head;
}

vcl::mesh_drawable create_sprite(){
    vcl::mesh_drawable sprite = vcl::mesh_drawable(vcl::mesh_primitive_quad({-1,1,0},{-1,-1,0},{1,-1,0},{1,1,0}));
    sprite.uniform_parameter.scaling = 5.0f;

    return sprite;
}


//INIT

animal_setting init_birdSet(gui_scene_structure& gui_scene){
    animal_setting bird_set;
    bird_set.N=100;
    bird_set.keyframe_position = bird_path(gui_scene, {0,0,0}, bird_set.N, 8.0);
    bird_set.keyframe_time = create_keyTime(bird_set.N);

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

    bird_set.animal_timer.scale=1;

    return bird_set;
}

animal_setting init_goatSet(const vcl::vec3 p, gui_scene_structure& gui){
    animal_setting goat_set;
    goat_set.N=100;
    goat_set.keyframe_position = goat_path(goat_set.N, 8.0, p, gui);
    goat_set.keyframe_time = create_keyTime(goat_set.N);

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

//    goat_set.trajectory = vcl::curve_dynamic_drawable(100); // number of steps stroed in the trajectory
//    goat_set.trajectory.uniform_parameter.color = {0,0,1};

    goat_set.picked_object=-1;

    goat_set.animal_timer.scale=2.3;

    return goat_set;
}

animal_setting init_snakeSet(const vcl::vec3 p, gui_scene_structure& gui){
    animal_setting snake_set;
    snake_set.N=25;
    snake_set.keyframe_position = snake_path(snake_set.N, 8.0, p, gui);
    snake_set.keyframe_time = create_keyTime(snake_set.N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    snake_set.animal_timer.t_min = snake_set.keyframe_time[1];
    snake_set.animal_timer.t_max = snake_set.keyframe_time[snake_set.keyframe_time.size()-2];
    snake_set.animal_timer.t = snake_set.animal_timer.t_min;

    // Prepare the visual elements
    snake_set.surface = vcl::mesh_primitive_sphere();
    snake_set.surface.uniform_parameter.color   = {0,0,1};
    snake_set.surface.uniform_parameter.scaling = 0.08f;

    snake_set.sphere = vcl::mesh_primitive_sphere();
    snake_set.sphere.uniform_parameter.color = {1,1,1};
    snake_set.sphere.uniform_parameter.scaling = 0.05f;

    snake_set.segment_drawer.init();

    snake_set.picked_object=-1;

    snake_set.animal_timer.scale=0.2f;

    return snake_set;
}

animal_setting init_boatSet(const vcl::vec2 p, gui_scene_structure& gui_scene){
    animal_setting boat_set;
    boat_set.N=100;
    boat_set.keyframe_position = boat_path(gui_scene, p, boat_set.N, 4.0);
    boat_set.keyframe_time = create_keyTime(boat_set.N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    boat_set.animal_timer.t_min = boat_set.keyframe_time[1];
    boat_set.animal_timer.t_max = boat_set.keyframe_time[boat_set.keyframe_time.size()-2];
    boat_set.animal_timer.t = boat_set.animal_timer.t_min;

    // Prepare the visual elements
    boat_set.surface = vcl::mesh_primitive_sphere();
    boat_set.surface.uniform_parameter.color   = {0,0,1};
    boat_set.surface.uniform_parameter.scaling = 0.08f;

    boat_set.sphere = vcl::mesh_primitive_sphere();
    boat_set.sphere.uniform_parameter.color = {1,1,1};
    boat_set.sphere.uniform_parameter.scaling = 0.05f;

    boat_set.segment_drawer.init();

    boat_set.trajectory = vcl::curve_dynamic_drawable(100); // number of steps stroed in the trajectory
    boat_set.trajectory.uniform_parameter.color = {0,0,1};

    boat_set.picked_object=-1;

    boat_set.animal_timer.scale=1;

    return boat_set;
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

std::vector<vcl::vec3> goat_path(unsigned int N, const float R, vcl::vec3 p, gui_scene_structure& gui){
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

std::vector<vcl::vec3> snake_path(unsigned int N, const float R, vcl::vec3 p, gui_scene_structure& gui){
    std::vector<vcl::vec3> path;

    float angle=0,  offset=0.08;
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

std::vector<vcl::vec3> bird_path(gui_scene_structure& gui_scene, vcl::vec3 pos, unsigned int N, const float R){
    std::vector<vcl::vec3> path;
    float height = pos.z;

    //const float scaling = gui_scene.eau_scaling;
    const float scaling = gui_scene.path_scaling;
    const int octave = gui_scene.path_octave;
    const float persistency = gui_scene.path_persistency;
    const float h = gui_scene.path_height;

    // Evaluate Perlin noise



    float angle=0, toAdd=4*M_PI/(N-1);
    for (int i=0; i<N/4 ; i++) {
        vcl::vec3 p = {R*std::cos(angle),R*std::sin(angle),height};

        const float noise = vcl::perlin(scaling*p.x, scaling*p.y, octave, persistency);
        p.z=p.z+noise*h;
        path.push_back(p);
        angle+=toAdd;
    }

    vcl::vec3 p00 = path[path.size()-1];
    float dy=5.0f;
    for (int i=0; i<N/4; i++) {
        p00 = vcl::vec3{p00.x,p00.y,height} + vcl::vec3(0,-dy,0);

        const float noise = vcl::perlin(scaling*p00.x, scaling*p00.y, octave, persistency);
        p00.z=p00.z+noise*h;

        path.push_back(p00);
    }

    for (int i=0; i<N/4 ; i++) {
        vcl::vec3 p = {R*std::cos(angle), p00.y + R*std::sin(angle),height};

        const float noise = vcl::perlin(scaling*p.x, scaling*p.y, octave, persistency);
        p.z=p.z+noise*h;
        path.push_back(p);
        angle+=toAdd;
    }

    p00 = path[path.size()-1];
    for (int i=0; i<N/4-1 ; i++) {
        p00 = vcl::vec3{p00.x,p00.y,height} + vcl::vec3(0,dy,0);

        const float noise = vcl::perlin(scaling*p00.x, scaling*p00.y, octave, persistency);
        p00.z=p00.z+noise*h;

        path.push_back(p00);
    }

    path.push_back(path[0]);path.push_back(path[1]);

    for (vcl::vec3& p: path) {
        p = p + vcl::vec3(pos.x, pos.y,0);
    }


    return path;
}

std::vector<vcl::vec3> boat_path(gui_scene_structure& gui_scene, vcl::vec2 pos, unsigned int N, const float R){
    std::vector<vcl::vec3> path;


    float angle=0, toAdd=4*M_PI/(N-1);
    for (int i=0; i<N/4 ; i++) {
        vcl::vec3 p = {R*std::cos(angle),R*std::sin(angle),0};

        path.push_back(rotate(p, {0,0,1}, M_PI/2));
        angle+=toAdd;
    }

    vcl::vec3 p00 = path[path.size()-1];
    float dy=0.5f;
    for (int i=0; i<N/4; i++) {
        p00 = vcl::vec3{p00.x,p00.y,0} + vcl::vec3(+dy,0,0);

        path.push_back(p00);
    }

    for (int i=0; i<N/4 ; i++) {
        vcl::vec3 p = {R*std::cos(angle),  R*std::sin(angle),0};

        p=vcl::vec3(p00.x,0,0) + rotate(p, {0,0,1}, M_PI/2);

        path.push_back(p);
        angle+=toAdd;
    }

    p00 = path[path.size()-1];
    for (int i=0; i<N/4-1 ; i++) {
        p00 = vcl::vec3{p00.x,p00.y,0} + vcl::vec3(-dy,0,0);

        path.push_back(p00);
    }

    path.push_back(path[0]);path.push_back(path[1]);path.push_back(path[2]);

    for (vcl::vec3& p: path) {
        p = p + vcl::vec3(pos.x, pos.y, 0) ;
        p = p + vcl::vec3(0, 0, z_eau2({p.x,p.y},gui_scene)-0.2);

    }

    return path;
}

vcl::vec3 rotate(const vcl::vec3& p, const vcl::vec3& k, float theta)
{
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    vcl::vec3 rotated = (p * cos_theta) + (cross(k, p) * sin_theta) + (k * vcl::dot(k, p)) * (1 - cos_theta);

    return rotated;
}



