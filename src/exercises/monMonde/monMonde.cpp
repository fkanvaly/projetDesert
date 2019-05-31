#include "monMonde.hpp"
#include <random>

#ifdef MON_MONDE

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

// Generator for uniform random number
std::default_random_engine generator;
std::uniform_real_distribution<float> distrib(0.0,1.0);


//les fonctions
mesh create_grid(const gui_scene_structure& gui_scene);
vcl::mesh create_skybox();
bool onGround(vec3 p);
bool onGround2(vec3 p);
vcl::vec3 rotateFrom(const vec3& p, const vec3& k, float theta);
std::vector<vec3> camera_path(unsigned int N, const float R, float height);
void add_pos(vcl::vec3 p, snake_tail& snake_tail);


/** ****** SETUP_DATA ******** **/
void scene_exercise::setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{

    //initialisation du terrain-----------------------------// *
    update_terrain();
    terrain_tex = texture_gpu( image_load_png("data/terrain/sabl.png") );
    raccord = create_underTerrain(gui_scene);
    raccord.uniform_parameter.color={0.9,0.7,0.6};
    raccord.uniform_parameter.shading.ambiant=0.5;

    //eau-----------------------------// *
    eau= create_eau(gui_scene);
    gui_scene.eau_time.t_min=0;
    gui_scene.eau_time.t_max=100;
    gui_scene.eau_time.scale=0.5f;
    gui_scene.eau_time.update();
    gui_scene.eau_time.scale=0.1;
    eau_tex=texture_gpu( image_load_png("data/textures/water.png") );
    update_eau();

    eau2 = create_eau2(gui_scene);
    gui_scene.eau2_time.t_min=0;
    gui_scene.eau2_time.t_max=20;
    gui_scene.eau2_time.update();
    gui_scene.eau2_time.scale=0.1;
    update_eau2();



    sousEau = create_sousEau();
    sousEau.uniform_parameter.color = {0.83,0.69,0.43};
    sousEau.uniform_parameter.shading.specular = 0.0f;
    sousEau.uniform_parameter.shading.ambiant=1.0f;

    //statue-----------------------------// *
    statue=load_obj("data/statue/statue.obj");
    statue.uniform_parameter.color={0.56,0.40,0.16};
    statue.uniform_parameter.shading.specular=0;
    statue.uniform_parameter.shading.ambiant=0.8;
    statue.uniform_parameter.translation={-24,20,10};
    statue.uniform_parameter.scaling=0.04;
    statue.uniform_parameter.rotation=rotation_from_axis_angle_mat3({0,1,0}, 3.14/3.5);

    //maison-----------------------------// *
    maison=load_obj("data/maison/maison.obj","data/maison/maison.mtl");
    maison.uniform_parameter.rotation=rotation_from_axis_angle_mat3({0,0,1}, -3.14f*0.5);
    maison.uniform_parameter.translation={-8,-20,0};
    maison.uniform_parameter.scaling=1.2;
    maison.uniform_parameter.shading.diffuse=1;
    maison.uniform_parameter.shading.specular=0;


    //Tree-----------------------------// *
    tree=createTree(); tree.scaling=0.15; //palm tree
    createTreePos();

    update_vegetation_type();

    //Oiseau-----------------------------// *
    birds.resize(N_bird);
    birdSet.resize(N_bird);

    bird_R={8.0, 4.0 , 5.0, 4.0, 3.0, 8.0, 4.0 , 5.0, 4.0, 3.0,8.0};
    bird_pos = {{10,-10,5},{-20,12,9}, {-13,10,5}, {13,10,15}, {-9,12,9}, {15,-22,5},{-20,20,9}, {-04,-40,5}, {-23,-12,5}, {9,-12,9}, {-20,-40,7}};

    for (int k=0; k<N_bird; k++) {
        birds[k] = create_oiseau();
        birds[k].translation("body")={0,0,2};
        birds[k].scaling=0.2f;

        birdSet[k]= init_birdSet(gui_scene);
        birdSet[k].animal_timer.t= birdSet[k].animal_timer.t_min + distrib(generator)*10;
        birdSet[k].N=20;

        //choose direction in with we will rotate the path
        if(distrib(generator)>0.5)
            bird_rotation.push_back(M_PI*distrib(generator));
        else {
            bird_rotation.push_back(-M_PI*distrib(generator));
        }

        update_path(gui_scene, k); //create bird k path
    }

    //goat-----------------------------// *
    goats.resize(N_goat);
    goatSets.resize(N_goat);
    goatP_inits.resize(N_goat);

    goat_pos = {{20,20,0}, {-20,20,0}, {0,40,0}, {0,0,0}};

    for (int k=0; k<N_goat; k++) {
        goats[k] = create_goat();
        //goats[k].rotation("body")=rotation_from_axis_angle_mat3({0,0,1}, M_PI/2);
        goats[k].scaling=0.2;
        goatSets[k] = init_goatSet(goat_pos[k],gui_scene);
        goatP_inits[k]={0,0,0};
    }

    //snake-----------------------------// *
    texture_snake = texture_gpu(image_load_png("data/snake/snake2.png"));

    snake_sphere=mesh_primitive_sphere();
    snake_sphere.uniform_parameter.color={1,1,1};
    snake_sphere.uniform_parameter.scaling=0.05f;

    snakes.resize(N_snake);
    snakeSets.resize(N_snake);
    snakeP_inits.resize(N_snake);
    queue.resize(N_snake);

    snake_pos = {{10,-10,5},{-20,12,9}, {-13,10,5}, {13,10,15}};

    for (int k=0; k<N_snake; k++) {
        snakes[k] = create_snake_head();
        snakes[k].scaling=1.6;
        snakeSets[k] = init_snakeSet(snake_pos[k],gui_scene);
        snakeP_inits[k]={0,0,0};
    }

    //crocodile-----------------------------// *
    croc = create_croc();

    //boat-----------------------------// *
    boat = create_boat();
    boat2 = create_boat();
    boat3 = create_boat();
    boat2Set = init_boatSet(boat2_xy, gui_scene);
    boat3Set = init_boatSet(boat3_xy, gui_scene);
    boat2Set.animal_timer.t = boat2Set.animal_timer.t_min + distrib(generator)*120;


    //flower-----------------------------// *
    flower=createFlower(0.1f);
    update_flower_position();

    //cactus-----------------------------// *
    cactus=createCactus(); cactus.scaling=0.8; //cactus type 1
    cactus.translation("tronc")={0,0,10};

    cactus2=load_obj("data/cactus/cac2.obj");  //cactus type 2
    cactus2.uniform_parameter.color={0.162093, 0.319995, 0.093024};
    cactus2.uniform_parameter.shading.ambiant=1;
    cactus2.uniform_parameter.shading.specular=0;
    cactus2.uniform_parameter.scaling=4;

    //rock-----------------------------// *
    caillou=load_obj("data/arbre/caillou.obj");
    caillou.uniform_parameter.color={0.6 ,0.6, 0.6};
    caillouTex=texture_gpu(image_load_png("data/arbre/caillou.png"));
    update_rock_position();
    update_rock_size();

    rock2 = load_obj("data/rock/rock2.obj");
    rock2.uniform_parameter.scaling = 0.5; rock2.uniform_parameter.shading.specular=0;
    rock2.uniform_parameter.color = {84.1/255.,66.0/255. , 34./255.};
    rock2.uniform_parameter.shading.ambiant=0.8;
    rock3 = load_obj("data/rock/rock3.obj","data/rock/rock3.mtl");
    rock3.uniform_parameter.scaling = 0.5; rock3.uniform_parameter.shading.specular=0;
    rock3.uniform_parameter.color = {84.1/255.,66.0/255. , 34./255.};
    rock3.uniform_parameter.shading.ambiant=0.2;

    //tour-----------------------------// *
    tour = load_obj("data/tour.obj","data/tour.mtl");
    tour.uniform_parameter.scaling = 3;
    tour.uniform_parameter.translation = {-20,-40,0};

    //bateau bord
    boat4 = load_obj("data/boat2.obj");
    boat4.uniform_parameter.scaling = 5; boat4.uniform_parameter.shading.specular=0;
    boat4.uniform_parameter.color = {84.1/255.,66.0/255. , 34./255.};
    boat4.uniform_parameter.shading.ambiant=0.8;
    boat4.uniform_parameter.translation = {-20,-50,0};

    //tente-----------------------------// *
    tente=createTente();
    tente.rotation("drap")=rotation_from_axis_angle_mat3({0,0,1}, -3.14*0.5);
    tente.translation("drap")={0,-4,0};
    //chameau
    chameau=load_obj("data/chameau/camel.obj","data/chameau/camel.mtl");
    chameau.uniform_parameter.scaling=0.5;
    chameau.uniform_parameter.rotation=rotation_from_axis_angle_mat3({0,0,1}, -3.14*0.5);
    chameau.uniform_parameter.translation={-6,-4,0};
    chameau.uniform_parameter.shading.specular=0;
    //pot
    pot_position = {-1.8,-5.4,0.1};
    pot=load_obj("data/tente/pot.obj","data/tente/pot.mtl");
    pot.uniform_parameter.scaling=0.4;
    pot.uniform_parameter.translation=pot_position;
    //fire
    setup_sprite(pot_position);
    //sauce
    sauce= mesh_primitive_disc(0.2);
    sauce.uniform_parameter.color={0.8, 0.2, 0.1};
    sauce.uniform_parameter.translation=pot_position +vec3(0,0,0.16);



    //cloud-----------------------------// *
    cloud = load_obj("data/cloud.obj");
    cloud.uniform_parameter.scaling=1.2;
    cloud.uniform_parameter.shading.ambiant=0.2;
    cloud.uniform_parameter.translation = {0,0,20};
    for (size_t k = 0; k < N_cloud; ++k) {
        const float x = 70* ( 0.025f+0.95f*distrib(generator) -0.5f);
        const float y = 100* (0.025f+0.95f*distrib(generator) - 0.5f);
        cloud_position.push_back({x,y,cloud_height});
    }

    //pluie-----------------------------// *
    goute = mesh_drawable(mesh_primitive_sphere(0.03f));
    goute.uniform_parameter.color = vec3(0.6f, 0.6f, 1.0f);
    goute_timer.periodic_event_time_step = 0.01f;
    pluie_timer.periodic_event_time_step = 20;
    goute_timer.scale=1.45;
    for (int i = 0; i < 3; ++i) {
        size_t k = distrib(generator)*cloud_position.size();
        pluie_position.push_back(cloud_position[k]);
    }


    //skybox-----------------------------// *
    skybox = create_skybox();
    skybox.uniform_parameter.shading = {1,0,0};
    skybox.uniform_parameter.rotation = rotation_from_axis_angle_mat3({1,0,0},-3.014f/2.0f);
    texture_skybox = texture_gpu(image_load_png("data/skybox/desertbox.png"));


    /************************************    *          DRAPEAU         *    ************************************/

    poteau = mesh_primitive_cylinder(0.1,{0,0,0},{0,0,10}); // le poteau

    // Load shaders
    shaders["open_surface"] = create_shader_program("shaders/mesh_back_illumination/mesh.vert.glsl","shaders/mesh_back_illumination/mesh.frag.glsl");
    shaders["wireframe_quads"] = create_shader_program("shaders/wireframe_quads/shader.vert.glsl","shaders/wireframe_quads/shader.geom.glsl","shaders/wireframe_quads/shader.frag.glsl");

    // Load textures
    texture_cloth = texture_gpu(image_load_png("data/cloth/X.png"));

    // Initialize cloth geometry and particles
    initialize();

    // Default value for simulation parameters
    user_parameters.K = 250.0f;
    user_parameters.m = 2.5f;
    user_parameters.wind = 0.02f;
    user_parameters.mu = 0.003f;

    //re-pos
    cloth.uniform_parameter.rotation= rotation_from_axis_angle_mat3({1,0,0}, M_PI);
    cloth.uniform_parameter.rotation= rotation_from_axis_angle_mat3({0,1,0}, M_PI/2);
    cloth.uniform_parameter.translation = {0,-1,9.5};

    // Init visual models
    sphere = mesh_drawable(mesh_primitive_sphere(1.0f,{0,0,0},60,60));

    //timer
    flag_timer.scale=1;
    flag_timer1.scale=1;
    flag_timer1.t_min = 0;
    flag_timer1.t_max = 15;

    /*******************************************************************************************************/


    // Camera
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    scene.camera.apply_rotation(0,0,0,1.5);
    setup_camera_path();



}

/** ****** FRAME_DATA ******** **/

void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui(scene);


    if(gui_scene.camera_ownpath)
        move_camera(scene, shaders);

    /************************************
    *          OBJETS STATIQUES         *
    ************************************/
    displayTerrain(shaders,scene);
    raccord.draw(shaders["mesh"], scene.camera); //dessous du terrain
    sousEau.draw(shaders["mesh"], scene.camera); //dessous de l'eau

    displayTree(shaders,scene);

    displayRock(shaders,scene); 

    statue.draw(shaders["mesh"], scene.camera);

    croc.translation("body")={croc_xy.x, croc_xy.y, z_eau(croc_xy, gui_scene) + 4.8};
    croc.draw(shaders["mesh"], scene.camera);

    boat.translation("body")={boat_xy.x, boat_xy.y, z_eau(boat_xy, gui_scene) + 5.1};
    boat.draw(shaders["mesh"], scene.camera);

    displayMaison(shaders,scene);

    tente.draw(shaders["mesh"], scene.camera);
    chameau.draw(shaders["mesh"], scene.camera);
    pot.draw(shaders["mesh"], scene.camera);
    sauce.draw(shaders["mesh"], scene.camera);

    poteau.draw(shaders["mesh"], scene.camera);

    tour.draw(shaders["mesh"], scene.camera);
    boat4.draw(shaders["mesh"], scene.camera);

    for (vec3 p : cloud_position) {
        cloud.uniform_parameter.translation = p;
        cloud.draw(shaders["mesh"], scene.camera);
    }

    //arbre mort - vegetation progressive
    for (size_t k=0; k<N_veg_rock; ++k) {
        if(!tree_type[k]) {
            if(choixRock[k]>0.5){
                rock2.uniform_parameter.translation = tree2_position[k];
                rock2.draw(shaders["mesh"], scene.camera);}
            else {
                rock3.uniform_parameter.translation = tree2_position[k];
                rock3.draw(shaders["mesh"], scene.camera);
            }
        }else {
            if(choixCac[k]>0.5){
                cactus.translation("tronc") = tree2_position[k] + vec3{0,0,0.5};
                cactus.draw(shaders["mesh"], scene.camera);
            }
            else {
                cactus2.uniform_parameter.translation = tree2_position[k] + vec3{0,0,0.5};
                cactus2.draw(shaders["mesh"], scene.camera);
            }}
    }

    /************************************
    *          OBJETS ANIMES            *
    ************************************/

    for (int k=0; k<N_bird; ++k) { //oiseau
        displayOiseau(k, birds[k], birdSet[k], shaders,scene);
    }

    for (int k=0; k<N_goat; ++k) { //chevre
        displayGoat(goats[k], goatSets[k], shaders,scene);
    }

    for (int k=0; k<N_snake; ++k) { //serpent
        displaySnake(k, snakes[k], snakeSets[k], shaders,scene);
    }

    update_boat(boat2Set, boat2_xy); //bateau en mvt
    displayBoat(boat2, boat2Set, shaders,scene);

    update_boat(boat3Set, boat3_xy);//bateau en mvt
    displayBoat(boat3, boat3Set, shaders,scene);

    display_drapeau(shaders,scene);

    displayPluie(shaders,scene);


    /************************************
    *          OBJETS TRANSPARENT       *
    ************************************/

    displayFlower(shaders,scene);
    displaySkybox(shaders,scene);
    displaySprite(shaders,scene); //fumée annimé
    displayEau(shaders,scene);
    displayEau2(shaders,scene);



}


/** ***********************************             fonctions            *********************************** **/

void scene_exercise::move_camera( scene_structure& scene , std::map<std::string,GLuint>& shaders)
{
    size_t N = camera_set.keyframe_time.size();

    camera_set.animal_timer.update();
    const float t = camera_set.animal_timer.t;
    vec3 p = cardinal_spline_interpolation(camera_set, t);
    const vec3 d = normalize( cardinal_spline_derivative_interpolation(camera_set, t) );

    mat3 R = rotation_between_vector_mat3({1,0,0},d);

    // up vector
    const vec3 up = {0,0,1};
    const vec3 up_proj = up-dot(up,d)*d;
    const vec3 new_up = R*vec3{0,0,1};

    const mat3 twist = rotation_between_vector_mat3(new_up,up_proj);
    R = twist*R;

    //scene.camera.orientation = R;
    scene.camera.translation = p;
    //scene.camera.apply_translation_in_screen_plane(p.x,p.y);

    if(gui_scene.path){
    // Draw birdSet.sphere at each keyframe position
    for(size_t k=0; k<N; ++k)
    {
        const vec3& p_keyframe = camera_set.keyframe_position[k];
        camera_set.sphere.uniform_parameter.translation = p_keyframe;
        camera_set.sphere.draw(shaders["mesh"],scene.camera);
    }


    // Draw segments between each keyframe
    for(size_t k=0; k<camera_set.keyframe_position.size()-1; ++k)
    {
        const vec3& pa = camera_set.keyframe_position[k];
        const vec3& pb = camera_set.keyframe_position[k+1];

        camera_set.segment_drawer.uniform_parameter.p1 = pa;
        camera_set.segment_drawer.uniform_parameter.p2 = pb;
        camera_set.segment_drawer.draw(shaders["segment_im"], scene.camera);
    }
    }
}


/************************************
*             SETUPER               *
************************************/
void scene_exercise::setup_sprite(vec3 p)
{
    sprite = vcl::mesh_drawable(vcl::mesh_primitive_quad({-1,1,0},{-1,-1,0},{1,-1,0},{1,1,0}));
    sprite.uniform_parameter.scaling = 5.0f;

    // The sprite texture .png file with alpha channel used as transparency parameter
    sprite.texture_pos = texture_gpu(image_load_png("data/sprite_smoke/smoke.png"));
    gui_scene.fire_timer.periodic_event_time_step=0.5;

    //segment_drawer.init();

    // Set initial position of several particles
    fire_particle particle;
    particle.p = p;
    particles.push_back(particle);
}


/************************************
*             UPDATER               *
************************************/


void scene_exercise::update_path(gui_scene_structure& gui, const int k)
{
    vec3 p = bird_pos[k];
    animal_setting& bird=birdSet[k];
    bird.keyframe_position=bird_path(gui, p, bird.N, bird_R[k]);
    bird.keyframe_time = create_keyTime(bird.N);

    bird.animal_timer.t_min = bird.keyframe_time[1];
    bird.animal_timer.t_max = bird.keyframe_time[bird.keyframe_time.size()-2];
}

void scene_exercise::update_terrain()
{
    // Clear memory in case of pre-existing terrain
    terrain.data_gpu.clear();

    // Create visual terrain surface
    terrain = create_terrain(gui_scene);
    terrain.uniform_parameter.color = {0.83,0.69,0.43};
    terrain.uniform_parameter.shading.specular = 0.0f;
    terrain.uniform_parameter.shading.ambiant=1.0f;
}

void scene_exercise::update_eau()
{
    // Clear memory in case of pre-existing terrain

    // Create visual terrain surface
    const size_t N=40;
    std::vector<vec3> newEauPos ; std::vector<index3> newEauCon;
    newEauPos.resize(N*N);

    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // get gui parameters
            //const float scaling = gui_scene.eau_scaling;
            const float scaling = gui_scene.eau_scaling*sin(gui_scene.eau_time.t);
            const int octave = gui_scene.eau_octave;
            const float persistency = gui_scene.eau_persistency;
            const float height = gui_scene.eau_height;

            // Evaluate Perlin noise
            const float noise = vcl::perlin(scaling*u, scaling*v, octave, persistency);

            const float x = 5*(u-0.5f);
            const float y = 45*(v-0.5f)-27.5;

            //const float z = h(x,y,gui_scene.eau_time.t)*height;
            float z = height*noise;

            const float norm= sqrt(pow(std::max(x-30,0.0f),2)+pow(y+51, 2));

            const float radious =1.0f;

            float d= std::max(norm-radious,0.0f)/1.5;
            d=1-exp(-pow(d,4));
            d=(d>1)?1:d;
            z= (d!=0)? z*d -5.5: -5.2f;

            newEauPos[kv+N*ku] = {x,y,z};

        }
    }
    unsigned int Ns = N;
    for(size_t ku=0; ku<Ns-1; ++ku)
    {
        for(size_t kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + Ns*ku; // current vertex offset

            const vcl::index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const vcl::index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            newEauCon.push_back(triangle_1);
            newEauCon.push_back(triangle_2);
        }
    }

    eau.data_gpu.update_normal(normal(newEauPos, newEauCon));
    eau.data_gpu.update_position(newEauPos);
    eau.uniform_parameter.shading.specular=0.5;
    eau.uniform_parameter.shading.ambiant=0.6;
    eau.uniform_parameter.translation={0,0,5};
}

void scene_exercise::update_eau2()
{
    // Clear memory in case of pre-existing terrain

    // Create visual terrain surface
    const size_t N=70;
    std::vector<vec3> newEauPos ; std::vector<index3> newEauCon;
    newEauPos.resize(N*N);

    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // get gui parameters
            //const float scaling = gui_scene.eau_scaling;
            const float scaling = gui_scene.eau2_scaling*std::sin(gui_scene.eau2_time.t);
            const int octave = gui_scene.eau2_octave;
            const float persistency = gui_scene.eau2_persistency;
            const float height = gui_scene.eau2_height;

            // Evaluate Perlin noise
            const float noise = vcl::perlin(scaling*u, scaling*v, octave, persistency);

            const float x = 70*(u-0.5f);
            const float y = 20*(v-0.5f)-59.5;

            //const float z = h(x,y,gui_scene.eau_time.t)*height;
            float z = height*noise;

            const float norm= sqrt(pow(x,2)+pow(std::max(-y-47.0,0.0),2));

            const float radious =2.0f;

            float d= std::max(norm-radious,0.0f)/1.5;
            d=1-exp(-pow(d,4));
            d=(d>1)?1:d;
            z= (d!=0)? z*d -5.40: -5.2f;

            newEauPos[kv+N*ku] = {x,y,z};

        }
    }
    unsigned int Ns = N;
    for(size_t ku=0; ku<Ns-1; ++ku)
    {
        for(size_t kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + Ns*ku; // current vertex offset

            const vcl::index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const vcl::index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            newEauCon.push_back(triangle_1);
            newEauCon.push_back(triangle_2);
        }
    }

    eau2.data_gpu.update_normal(normal(newEauPos, newEauCon));
    eau2.data_gpu.update_position(newEauPos);
    eau2.uniform_parameter.shading.specular=0.5;
    eau2.uniform_parameter.shading.ambiant=0.6;
    eau2.uniform_parameter.translation={0,0,5};
}

void scene_exercise::update_flower_position()
{
    const size_t N_flower = 25;

    for(size_t k=0; k<N_flower; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v,gui_scene);
        flower_position.push_back(p);
    }
}

void scene_exercise::update_rock_position()
{

    for(size_t k=0; k<N_rock; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v,gui_scene);
        rock_position.push_back(p);
    }
}

void scene_exercise::update_rock_size()
{

    for(size_t k=0; k<N_rock; ++k)
    {
        const float size = distrib(generator)/3;
        rock_size.push_back(size);
    }
}

void scene_exercise::update_vegetation_type()
{
    while(tree2_position.size()<N_veg_rock)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v,gui_scene);
        if(!onGround(p) && !onGround2(p))
            tree2_position.push_back(p);
    }

    for(size_t k=0; k<N_veg_rock; ++k)
    { //choix de cactus ou gros caillou
        vec3 p =tree2_position[k];
        const float d= std::sqrt(p.x*p.x + p.y*p.y);
        const float prob = -d/80.0f  + 1.0f;
        std::bernoulli_distribution distribution(prob);
        bool a=distribution(generator);
        tree_type.push_back(a);
    }

    choixCac.resize(N_veg_rock);
    choixRock.resize(N_veg_rock);

    for(size_t k=0; k<N_veg_rock; ++k)
    {
        if(!tree_type[k]){ //choix tu type de gros caillou
            choixRock[k] = distrib(generator);
            std::cout<<choixRock[k]<<std::endl;}
        if(tree_type[k]){//choix du type de cactus
            choixCac[k] = distrib(generator);
            std::cout<<choixCac[k]<<std::endl;}
    }


}

void scene_exercise::update_boat(animal_setting& Set, vec2 p)
{
    Set.keyframe_position = boat_path(gui_scene, p, Set.N, 4.0);
}

/************************************
*           DISPLAYER               *
************************************/
void scene_exercise::displayTerrain(std::map<std::string,GLuint>& shaders, scene_structure& scene){

    // Display terrain *************************************
    glPolygonOffset( 1.0, 1.0 );
    glBindTexture(GL_TEXTURE_2D, terrain_tex);

    terrain.draw(shaders["mesh"], scene.camera);

    //  Avoids to use the previous texture for another object
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    if(gui_scene.wireframe){
        terrain.draw(shaders["wireframe"], scene.camera);
    }
    //End terrain-------------------------------------------
}

void scene_exercise::displayMaison(std::map<std::string, GLuint> &shaders, scene_structure &scene){
    maison.uniform_parameter.rotation = rotation_from_axis_angle_mat3({0,0,1}, -3.14*0.5 -3.14);

    //left side
    maison.uniform_parameter.translation={-8,-20,0};
    maison.draw(shaders["mesh"], scene.camera);
    maison.uniform_parameter.translation={-8,-30,0};
    maison.draw(shaders["mesh"], scene.camera);
    maison.uniform_parameter.translation={-8,-40,0};
    maison.draw(shaders["mesh"], scene.camera);

    //right side

    maison.uniform_parameter.translation={8,-20,0};
    maison.draw(shaders["mesh"], scene.camera);
    maison.uniform_parameter.translation={8,-30,0};
    maison.draw(shaders["mesh"], scene.camera);
    maison.uniform_parameter.translation={8,-40,0};
    maison.draw(shaders["mesh"], scene.camera);

}

void scene_exercise::displaySprite(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    const float dt = gui_scene.fire_timer.update();

    const bool is_new_particle = gui_scene.fire_timer.event;

    if( is_new_particle )
    {
        fire_particle new_particle;
        const vec3 p0 = pot_position;
        new_particle.p=p0;
        particles.push_back(new_particle);
    }


    // Evolve position of particles
    const vec3 g = {0.0f,0.0f,-9.81f};
    const float dz=0.01 , dr=0.001, dsize=0.0003;
    for(fire_particle& particle : particles)
    {
        vec3& p = particle.p;
        float& t = particle.t;
        float& R = particle.R;
        float& size=particle.size;


        //Numerical integration
        t = t + dt;
        R = R + dr;
        size = size - dsize;
        p = vec3(pot_position.x + R*cos(t) , pot_position.y + R*sin(t) , p.z+dz);
    }


    // Remove particles that are too low
    int i=0;
    for(fire_particle par: particles){
        if(par.p.z > 5){
            particles.erase(particles.begin()+i);
        }i++;
    }


    // **************************************** //
    // Sprites display
    // **************************************** //

    // Activate transparency - allow blend based on alpha channel value
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Set texture ID to be the one of the sprite
    glBindTexture(GL_TEXTURE_2D,sprite.texture_pos);

    // Desactivate writing in the Z-buffer (semi-transparent object will not hide other semi-transparent objects)
    glDepthMask(false);

    // Set color of the sprite
    sprite.uniform_parameter.color = {1,1,1};
    // Sprite should not be shaded with lighting - Set ambiant value to 1 and 0 for diffuse and specular
    sprite.uniform_parameter.shading = {1,0,0};

    for(fire_particle particle : particles)
    {
        // Sprite should always face the camera
        sprite.uniform_parameter.rotation = scene.camera.orientation;
        sprite.uniform_parameter.translation = particle.p;
        sprite.uniform_parameter.scaling =particle.size;
        if( gui_scene.enable_sprite )
            sprite.draw(shaders["mesh"], scene.camera);
    }


    // In case we want to show the quads on which sprites are displayed
    if( !gui_scene.enable_sprite )
    {
        // return to standard mesh display
        glDepthMask(true);
        sprite.uniform_parameter.color = {0.1f,0.1f,0.5f};
        sprite.uniform_parameter.shading = {0.2f,0.8f,0.3f};
        glBindTexture(GL_TEXTURE_2D,scene.texture_white);

        for(size_t k=0; k<particles.size(); ++k)
        {
            sprite.uniform_parameter.translation = particles[k].p;
            sprite.uniform_parameter.scaling = 0.2;
            sprite.uniform_parameter.rotation = scene.camera.orientation;
            sprite.draw(shaders["mesh"], scene.camera);

            // avoids z-fighting between mesh and wireframe
            glEnable( GL_POLYGON_OFFSET_FILL ); glPolygonOffset( 1.0, 1.0 );

            sprite.draw(shaders["wireframe"], scene.camera);
        }

    }

    // Set the default settings for future drawings
    glDepthMask(true);
    glBindTexture(GL_TEXTURE_2D,scene.texture_white);
}

void scene_exercise::displayEau(std::map<std::string,GLuint>& shaders, scene_structure& scene){

    // Display terrain *************************************
    //glPolygonOffset( 1.0, 1.0 );
    update_eau();
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, eau_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    gui_scene.eau_time.update();

    eau.draw(shaders["mesh"], scene.camera);
    glDisable(GL_BLEND);
    //  Avoids to use the previous texture for another object
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    if(gui_scene.wireframe){
        eau.draw(shaders["wireframe"], scene.camera);
    }
    //End terrain-------------------------------------------
}

void scene_exercise::displayEau2(std::map<std::string,GLuint>& shaders, scene_structure& scene){

    // Display terrain *************************************
    //glPolygonOffset( 1.0, 1.0 );
    update_eau2();
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, eau_tex);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    gui_scene.eau2_time.update();

    eau2.draw(shaders["mesh"], scene.camera);
    glDisable(GL_BLEND);
    //  Avoids to use the previous texture for another object
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    if(gui_scene.wireframe){
        eau2.draw(shaders["wireframe"], scene.camera);
    }
    //End terrain-------------------------------------------
}

void scene_exercise::displayTree(std::map<std::string,GLuint>& shaders, scene_structure& scene){
    for (vec3 u: tree_pos) {
        tree.translation("tronc")=u;
        tree.draw(shaders["mesh"], scene.camera);
    }
}

void scene_exercise::displaySkybox(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(gui_scene.skybox)
    {
        glBindTexture(GL_TEXTURE_2D,texture_skybox);
        skybox.uniform_parameter.scaling = 150.0f;
        skybox.uniform_parameter.translation = scene.camera.camera_position() + vec3(0,0,-50.0f);
        skybox.draw(shaders["mesh"], scene.camera);
        glBindTexture(GL_TEXTURE_2D,scene.texture_white);
     }

}

void scene_exercise::displayOiseau(int k , vcl::mesh_drawable_hierarchy& oiseau, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    Set.animal_timer.update();
    const float t = Set.animal_timer.t;
    vec3 p = cardinal_spline_interpolation(Set, t);
    p = rotateFrom( p , {0,0,1} , bird_rotation[k]); //randomize trajectory direction
    const vec3 d = normalize( rotateFrom (cardinal_spline_derivative_interpolation(Set, t), {0,0,1}, bird_rotation[k]) );

    mat3 R = rotation_between_vector_mat3({1,0,0},d);

    // up vector
    const vec3 up = {0,0,1};
    const vec3 up_proj = up-dot(up,d)*d;
    const vec3 new_up = R*vec3{0,0,1};

    const mat3 twist = rotation_between_vector_mat3(new_up,up_proj);
    R = twist*R;

    //wings move
    oiseau.rotation("arm_top_left") = rotation_from_axis_angle_mat3({1,0,0}, std::sin(2*3.14f*(t-0.4f)) );
    oiseau.rotation("arm_top_right") = rotation_from_axis_angle_mat3({-1,0,0}, std::sin(2*3.14f*(t-0.4f)) );


    // Draw current position
    oiseau.translation("body") = p ;
    oiseau.rotation("body") = R;
    oiseau.draw(shaders["mesh"],scene.camera);

}

void scene_exercise::displayGoat(vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene)
{

    Set.animal_timer.update();
    const float t = Set.animal_timer.t;
    vec3 p = cardinal_spline_interpolation(Set, t);
    const vec3 d = normalize( cardinal_spline_derivative_interpolation(Set, t) );

    mat3 R = rotation_between_vector_mat3({1,0,0},d);

    // up vector
    const vec3 up = {0,0,1};
    const vec3 up_proj = up-dot(up,d)*d;
    const vec3 new_up = R*vec3{0,0,1};

    const mat3 twist = rotation_between_vector_mat3(new_up,up_proj);
    R = twist*R;

    // Draw current position
    goat.translation("body") = p;
    goat.rotation("body") = R * rotation_from_axis_angle_mat3({0,0,1}, M_PI);
    goat.draw(shaders["mesh"],scene.camera);
}

void scene_exercise::displaySnake(int k, vcl::mesh_drawable_hierarchy& snake, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene){

    Set.animal_timer.update();
    const float t = Set.animal_timer.t;
    const vec3 p = sinus_spline_interpolation(Set, t);
    const vec3 d = normalize( cardinal_spline_derivative_interpolation(Set, t) );

    mat3 R = rotation_between_vector_mat3({1,0,0},d);

    // up vector
    const vec3 up = {0,0,1};
    const vec3 up_proj = up-dot(up,d)*d;
    const vec3 new_up = R*vec3{0,0,1};

    const mat3 twist = rotation_between_vector_mat3(new_up,up_proj);
    R = twist*R;

    snake.translation("head")= p - 0.1*d - vec3(0,0,0.08);
    snake.rotation("head") = R * rotation_from_axis_angle_mat3({0,0,-1}, M_PI/2);
    snake.draw(shaders["mesh"],scene.camera);

    //rajouter les positions à la queue du serpent
    add_pos(p, queue[k]);

    //draw tale of snake

    for (size_t i=0;i<queue[k].tail.size();i++) {
        vec3 p11=queue[k].tail[i];
        snake_sphere.uniform_parameter.translation = p11;
        snake_sphere.uniform_parameter.scaling = 0.1/(1+exp(-0.015f*i));
        glBindTexture(GL_TEXTURE_2D, texture_snake);
        snake_sphere.draw(shaders["mesh"],scene.camera);
        glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    }


}

void scene_exercise::displayBoat(vcl::mesh_drawable_hierarchy& boat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    Set.animal_timer.update();
    const float t = Set.animal_timer.t;
    vec3 p = cardinal_spline_interpolation(Set, t);
    const vec3 d = normalize( cardinal_spline_derivative_interpolation(Set, t) );

    Set.trajectory.add_point(p);

    mat3 R = rotation_between_vector_mat3({0,1,0},d);

    // up vector
    const vec3 up = {0,0,1};
    const vec3 up_proj = up-dot(up,d)*d;
    const vec3 new_up = R*vec3{0,0,1};

    const mat3 twist = rotation_between_vector_mat3(new_up,up_proj);
    R = twist*R;


    // Draw current position
    boat.translation("body") = p ;
    boat.rotation("body") = R ;
    boat.draw(shaders["mesh"],scene.camera);


}

void scene_exercise::displayFlower(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.flower)
        return ;

    glEnable(GL_BLEND);
    glDepthMask(false);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const size_t N_grass = flower_position.size();
    if(gui_scene.texture)
        glBindTexture(GL_TEXTURE_2D, flower.texture_pos);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    flower.uniform_parameter.rotation = scene.camera.orientation;
    flower.uniform_parameter.scaling = 3.0f;
    for(size_t k=0; k<N_grass; ++k)
    {

        const vec3& p = flower_position[k];
        if(!onGround(p)){
        flower.uniform_parameter.translation = p;

        glPolygonOffset( 1.0, 1.0 );
        flower.draw(shaders["mesh"], scene.camera);}
    }
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    glDepthMask(true);
    glDisable(GL_BLEND);


    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_grass; ++k)
        {
            const vec3& p = flower_position[k];
            if(!onGround(p)){
            flower.uniform_parameter.translation = p;
            glPolygonOffset( 1.0, 1.0 );
            flower.draw(shaders["wireframe"], scene.camera);}
        }
    }


}

void scene_exercise::displayRock(std::map<std::string, GLuint> &shaders, scene_structure &scene)
{
    if(!gui_scene.rock)
        return ;

    const size_t N_grass = rock_position.size();

    for(size_t k=0; k<N_grass; ++k)
    {
        const vec3& p = rock_position[k];
        const float size=rock_size[k];
        if(!onGround(p)){
            caillou.uniform_parameter.translation = p + vec3{0,0,0.05};
            caillou.uniform_parameter.scaling=size;
            glPolygonOffset( 1.0, 1.0 );
            // Display terrain *************************************
            glPolygonOffset( 1.0, 1.0 );
            glBindTexture(GL_TEXTURE_2D, caillouTex);

            caillou.draw(shaders["mesh"], scene.camera);
            //rock.draw(shaders["mesh"], scene.camera);

            //  Avoids to use the previous texture for another object
            glBindTexture(GL_TEXTURE_2D, scene.texture_white);

        }
    }


    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_grass; ++k)
        {
            const vec3& p = rock_position[k];
            if(!onGround(p)){
            caillou.uniform_parameter.translation = p;
            glPolygonOffset( 1.0, 1.0 );
            caillou.draw(shaders["wireframe"], scene.camera);
            }
        }
    }


}


void scene_exercise::displayPluie(std::map<std::string, GLuint> &shaders, scene_structure &scene){

    goute_timer.update();
    pluie_timer.update();
    const float t_current = goute_timer.t;

    // Emission of new particle if needed
    const bool is_new_pluie = pluie_timer.event;
    const bool is_new_particle = goute_timer.event;

    if(is_new_pluie){//gere les transision entre deux pluie
        cond=!cond;
        if(!cond){
            for (int i = 0; i < 3; ++i) {
                size_t k = distrib(generator)*cloud_position.size(); //choix les prochains nuages
                pluie_position[i]=cloud_position[k];
            }
        }
    }

    if( cond )
    {
    if( is_new_particle )
    {
        for (vec3 p : pluie_position) {

            const float dx=1;
            particle_structure new_particle;
            new_particle.t0 = t_current;
            new_particle.p0 = p - vec3(dx,0,0);

            // Initial speed is random. (x,z) components are uniformly distributed along a circle.
            const float theta = 2*float(M_PI)*distrib(generator);
            new_particle.v0 = vec3( std::cos(theta), std::sin(theta), 1.0f);

            particle_structure new_particle2;
            new_particle2.t0 = t_current;
            new_particle2.p0 = p + vec3(dx,0,0);

            // Initial speed is random. (x,z) components are uniformly distributed along a circle.
            const float theta2 = 2*float(M_PI)*distrib(generator);
            new_particle2.v0 = vec3( std::cos(theta2), std::sin(theta2), 1.0f);

            goute_particules.push_back(new_particle); goute_particules.push_back(new_particle2);
        }
    }}

    // Compute position of particles at current time
    const vec3 g = {0,0,-9.81f};
    for(particle_structure& particle : goute_particules)
    {
        const float t = t_current-particle.t0; // local time of the current particle since its creation
        const vec3 p = g*t*t/2.0f + particle.v0*t + particle.p0;

        goute.uniform_parameter.translation = p;
        goute.draw(shaders["mesh"], scene.camera);
    }

    // Remove particles that have exceeded they lifetime
    for(auto it = goute_particules.begin(); it!=goute_particules.end(); ++it)
        if( t_current-it->t0 > 2)
            it = goute_particules.erase(it);
}


/************************************
*           DRAPEAU SIMULATION      *
************************************/

vec3 spring_force(const vec3& pi, const vec3& pj, float L0, float K)
{
    float L = sqrt((pi.x-pj.x)*(pi.x-pj.x)+(pi.y-pj.y)*(pi.y-pj.y)+(pi.z-pj.z)*(pi.z-pj.z));
    vec3 u = (pi-pj)/L;
    vec3 F = -K*(L-L0)*u;
    // TO DO: correct the computation of this force value
    return F;
}

vec3 wind_force(const float wind,const vec3& normal, float h){
    vec3 direction {0,0,1};
    return wind*sin(h)*dot(direction,normal)*direction;
}

void scene_exercise::compute_forces(float h)
{
    const size_t NN = force.size();      // Total number of particles
    const int N = size_t(std::sqrt(NN)); // Dimension of the grid in u/v direction

    // Update simulation parameters
    // Adapt parameter to scale with the size of the mesh
    simulation_parameters.m = user_parameters.m / float(NN);


    // Gravity
    const float m = simulation_parameters.m;   // mass of the particle
    const vec3 g = {0,-9.81f,0};
    for(size_t k=0; k<NN; ++k)
        force[k] = m*g;
    // Add other forces ...
    // You have access to the following parameters
    const float K = user_parameters.K;   // spring stiffness
    const float L0 = simulation_parameters.L0; // edge length in u and v direction at rest
    const float mu = user_parameters.mu;
    const float wind = user_parameters.wind;
    // Note: It can be convenient to navigate through the 2D (u,v) parameters of the grid
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // the index in the 1D at parameter (ku,kv) is given by kv+N*ku
            // ex. position[kv+N*ku]
            // ...
           force[kv+N*ku]+=-mu*speed[kv+N*ku];
           if(kv<N-1){force[kv+N*ku]+=spring_force(position[kv+N*ku],position[kv+1+N*ku],L0,K);};
           if(kv>0){force[kv+N*ku]+=spring_force(position[kv+N*ku],position[kv-1+N*ku],L0,K);};
           if(ku<N-1){force[kv+N*ku]+=spring_force(position[kv+N*ku],position[kv+N*(ku+1)],L0,K);}
           if(ku>0){force[kv+N*ku]+=spring_force(position[kv+N*ku],position[kv+N*(ku-1)],L0,K);}
           force[kv+N*ku]+=wind_force(wind,normals[kv+N*ku],h);
        }
    }


}

void scene_exercise::initialize()
{
    // Number of samples of the model (total number of particles is N_cloth x N_cloth)
    const size_t N_cloth = 51;

    // Rest length (length of an edge)
    simulation_parameters.L0 = 2.0f/float(N_cloth-1);

    // Create cloth mesh in its initial position
    // Horizontal grid of length 1 x 1
    const mesh base_cloth = mesh_primitive_grid(N_cloth,N_cloth,2.0f,2.0f,{-0.5,1,0},{0,1,0});

    // Set particle position from cloth geometry
    position = base_cloth.position;


    // Set hard positional constraints
    for (int ku=0; ku<N_cloth; ++ku) {
    positional_constraints[N_cloth*ku] = position[N_cloth*ku];
    ku++;
    }

    // Init particles data (speed, force)
    speed.resize(0);
    speed.resize(position.size());
    force.resize(0);
    force.resize(position.size());


    // Store connectivity and normals
    connectivity = base_cloth.connectivity;
    normals = normal(position,connectivity);

    // Send data to GPU
    cloth.data_gpu.clear();
    cloth = mesh_drawable(base_cloth);
    cloth.uniform_parameter.shading.specular = 0.0f;
}

void scene_exercise::display_drapeau(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    flag_timer1.update();
    const float dt = flag_timer.update();
    const float t = flag_timer1.t;

    // Force constant simulation time step
    float h = dt<=1e-6f? 0.0f : flag_timer.scale*0.001f;

    if(h>0)
    {
        compute_forces(0.5*t*cos(0.5*t));
        numerical_integration(h);

        hard_constraints();                      // Enforce hard positional constraints

        normal(position, connectivity, normals); // Update normals of the cloth
    }


    cloth.data_gpu.update_position(position);
    cloth.data_gpu.update_normal(normals);

    glEnable( GL_POLYGON_OFFSET_FILL );

    // Display cloth
    if(gui_display_texture)
        glBindTexture(GL_TEXTURE_2D, texture_cloth);
    glPolygonOffset( 1.0, 1.0 );
    cloth.draw(shaders["open_surface"],scene.camera);
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);

    if(gui_display_wireframe)
    {
        glPolygonOffset( 1.0, 1.0 );
        cloth.draw(shaders["wireframe_quads"],scene.camera);
    }


}

void scene_exercise::hard_constraints()
{
    // Fixed positions of the cloth
    for(const auto& constraints : positional_constraints)
        position[constraints.first] = constraints.second;
}

void scene_exercise::numerical_integration(float h)
{
    const size_t NN = position.size();
    const float m = simulation_parameters.m;

    for(size_t k=0; k<NN; ++k)
    {
        vec3& p = position[k];
        vec3& v = speed[k];
        const vec3& f = force[k];

        v = v + h*f/m;
        p = p + h*v;
    }
}



/************************************
*             CREATER               *
************************************/
void scene_exercise::createTreePos(){
    const float R=4.0; float y=-8;

    //tree_pos.push_back({0,y,0});
    tree_pos.push_back({R*(float)cos(3.14*0.25),y,0}); tree_pos.push_back({-R*(float)cos(3.14*0.25),y,0});

    for (int i=0;i<N_tree;i++) {
        y=y-7;
        tree_pos.push_back({R,y,0});
        tree_pos.push_back({-R,y,0});
    }

}

vcl::mesh create_skybox()
{
    const float a= 2;
    const vec3 p000 = {-a,-a,-a};
    const vec3 p001 = {-a,-a, a};
    const vec3 p010 = {-a, a,-a};
    const vec3 p011 = {-a, a, a};
    const vec3 p100 = { a,-a,-a};
    const vec3 p101 = { a,-a, a};
    const vec3 p110 = { a, a,-a};
    const vec3 p111 = { a, a, a};

    mesh skybox;

    skybox.position = {
        p000, p100, p110, p010,
        p010, p110, p111, p011,
        p100, p110, p111, p101,
        p000, p001, p010, p011,
        p001, p101, p111, p011,
        p000, p100, p101, p001
    };


    skybox.connectivity = {
        {0,1,2}, {0,2,3}, {4,5,6}, {4,6,7},
        {8,11,10}, {8,10,9}, {17,16,19}, {17,19,18},
        {23,22,21}, {23,21,20}, {13,12,14}, {13,14,15}
    };

    const float e = 1e-3f;
    const float u0 = 0.0f;
    const float u1 = 0.25f+e;
    const float u2 = 0.5f-e;
    const float u3 = 0.75f-e;
    const float u4 = 1.0f;
    const float v0 = 0.0f;
    const float v1 = 1.0f/3.0f+e;
    const float v2 = 2.0f/3.0f-e;
    const float v3 = 1.0f;
    skybox.texture_uv = {
        {u1,v1}, {u2,v1}, {u2,v2}, {u1,v2},
        {u1,v2}, {u2,v2}, {u2,v3}, {u1,v3},
        {u2,v1}, {u2,v2}, {u3,v2}, {u3,v1},
        {u1,v1}, {u0,v1}, {u1,v2}, {u0,v2},
        {u4,v1}, {u3,v1}, {u3,v2}, {u4,v2},
        {u1,v1}, {u2,v1}, {u2,v0}, {u1,v0}
    };


    return skybox;

}

/************************************
*              EXTRA                *
************************************/

vec3 rotateFrom(const vec3& p, const vec3& k, float theta)
{
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    vcl::vec3 rotated = (p * cos_theta) + (cross(k, p) * sin_theta) + (k * vcl::dot(k, p)) * (1 - cos_theta);

    return rotated;
}

bool onGround(vec3 p){
    const float x = p.x;
    const float y = p.y;
    const float norm= sqrt(pow(x,2)+pow(std::max(y+10.0,0.0),2));

    float d= std::max(norm-radious-4,0.0f)/1.5;
    d=1-exp(-pow(d,4));
    d=(d>1)?1:d;
    if(d!=0) return false;
    return true;
}

bool onGround2(vec3 p){
    const float x = p.x;
    const float y = p.y;
    const float norm= sqrt(pow(std::max(x-40,0.0f),2)+pow(y+45, 2));

    float d= std::max(norm-radious,0.0f)/1.5;
    d=1-exp(-pow(d,2));
    d=(d>1)?1:d;
    if(d!=0) return false;
    return true;

}

void add_pos(vec3 p, snake_tail& snake_tail ){
    std ::vector<vcl::vec3>& tail = snake_tail.tail;
    if( tail.size() < snake_tail.size_max )
        tail.push_back(p);
    else
    {
        for(size_t k=0; k<snake_tail.size_max-1; ++k)
            tail[k] = tail[k+1];
            tail[snake_tail.size_max-1] = p;
    }

}
void scene_exercise::setup_camera_path()
{
    camera_set.N=100;
    camera_set.keyframe_position = camera_path(camera_set.N, 10 , 20);
    camera_set.keyframe_time = create_keyTime(camera_set.N);

    // Set timer bounds
    //  To ease spline interpolation of a closed curve time \in [t_1,t_{N-2}]
    camera_set.animal_timer.t_min = camera_set.keyframe_time[1];
    camera_set.animal_timer.t_max = camera_set.keyframe_time[camera_set.keyframe_time.size()-2];
    camera_set.animal_timer.t = camera_set.animal_timer.t_min;

    // Prepare the visual elements
    camera_set.surface = vcl::mesh_primitive_sphere();
    camera_set.surface.uniform_parameter.color   = {0,0,1};
    camera_set.surface.uniform_parameter.scaling = 0.08f;

    camera_set.sphere = vcl::mesh_primitive_sphere();
    camera_set.sphere.uniform_parameter.color = {1,1,1};
    camera_set.sphere.uniform_parameter.scaling = 0.05f;

    camera_set.segment_drawer.init();

    camera_set.animal_timer.scale=1.5f;

}

void scene_exercise::update_camera(int i,scene_structure& scene){
    if(i==0){ //drapeau view
        scene.camera.translation = {16.470531, -10.785789, 9.624680};
        scene.camera.orientation = {0.519099 ,-0.155370 ,0.840474, 0.854714 ,0.094362, -0.510450 , 0.000000 ,0.983339 ,0.181780};
        scene.camera.scale = 91.8113;
    }

    if(i==1){ //snake view
        scene.camera.translation = {-12.4442, -12.0482, 3.20195};
        scene.camera.orientation = {0.637392 ,-0.317567, 0.702056 , 0.770540 ,0.262692, -0.580742 , 0.000000 , 0.911123, 0.412136 };
        scene.camera.scale = 48.9877;
    }

    if(i==2){ //fire view
        scene.camera.translation = {-12.4442, -12.0482, 3.20195};
        scene.camera.orientation = {0.695585, 0.130598, -0.706474, -0.718444, 0.126443, -0.683996,  0, 0.983339, 0.18178};
        scene.camera.scale = 17.6145;
        scene.camera.spherical_coordinates= {-0.989062, 0.528};

    }

}

std::vector<vec3> camera_path(unsigned int N, const float R, float height){
    std::vector<vec3> path;
    float angle=0, toAdd=2*M_PI/(N-1);
    for (int i=0; i<N+2 ; i++) {
        path.push_back({R*std::cos(angle),R*std::sin(angle),height});
        angle+=toAdd;
    }

    return path;
}


/************************************
*              gui and mouse        *
************************************/
void scene_exercise::set_gui(scene_structure& scene)
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
    ImGui::Checkbox("Skybox", &gui_scene.skybox);
    ImGui::Checkbox("Path", &gui_scene.path);

    ImGui::Separator();
    ImGui::Text("Camera");
    ImGui::Checkbox("camera own path", &gui_scene.camera_ownpath);
    if( ImGui::Button("flag wiew") ) update_camera(0, scene);
    if( ImGui::Button("snake wiew") ) update_camera(1, scene);
    if( ImGui::Button("sprite wiew") ) update_camera(2, scene);

    if( ImGui::Button("camera position") )
    {
    std::cout<<"camera position : {"<<scene.camera.camera_position().x<< ", "<<scene.camera.camera_position().y << ", "<<scene.camera.camera_position().z<<"}"<<std::endl;

    std::cout<<"camera matrix : {"<<scene.camera.orientation[0];
    for (int i = 0; i < 3*3; ++i) std::cout<<", "<<scene.camera.orientation[i];
    std::cout<<"}"<<std::endl;

    std::cout<<"scale : "<< scene.camera.scale<<std::endl;
    std::cout<<"spherical coord : {"<<scene.camera.spherical_coordinates.x << ", "<< scene.camera.spherical_coordinates.y << "}"<<std::endl<<std::endl;
    }


    ImGui::Separator();
    ImGui::Text("flag parameters");
    //drapeau--------------------------------
    float scale_min = 0.05f;
    float scale_max = 2.0f;
    ImGui::SliderScalar("drapeau Time scale", ImGuiDataType_Float, &flag_timer1.scale, &scale_min, &scale_max, "%.2f s");

    float stiffness_min = 1.0f;
    float stiffness_max = 600.0f;
    ImGui::SliderScalar("Stiffness",ImGuiDataType_Float, &user_parameters.K, &stiffness_min, &stiffness_max, "%.2f s");

    float mu_min = 0.0f;
    float mu_max = 0.1f;
    ImGui::SliderScalar("Damping",ImGuiDataType_Float, &user_parameters.mu, &mu_min, &mu_max, "%.3f s");


    float mass_min = 0.01f;
    float mass_max = 15.0f;
    ImGui::SliderScalar("Mass",ImGuiDataType_Float, &user_parameters.m, &mass_min, &mass_max, "%.2f s");

    float wind_min = 0.0f;
    float wind_max = 0.5f;
    ImGui::SliderScalar("Wind",ImGuiDataType_Float, &user_parameters.wind, &wind_min, &wind_max, "%.2f s");

    /***************/
    //fire
    ImGui::Separator();
    ImGui::Text("Fire parameters");
    ImGui::Checkbox("Fire", &gui_scene.enable_sprite);
    float firescale_min = 0.05f;
    float firescale_max = 2.0f;
    ImGui::SliderScalar("Time period", ImGuiDataType_Float, &gui_scene.fire_timer.periodic_event_time_step, &firescale_min, &firescale_max, "%.2f s");



}

void scene_exercise::mouse_click(scene_structure& scene, GLFWwindow* window, int , int action, int )
{
    // Mouse click is used to select a position of the control polygon
    // ******************************************************************** //

    // Window size
    int w=0;
    int h=0;
    glfwGetWindowSize(window, &w, &h);

    // Current cursor position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Convert pixel coordinates to relative screen coordinates between [-1,1]
    const float x = 2*float(xpos)/float(w)-1;
    const float y = 1-2*float(ypos)/float(h);

    // Check if shift key is pressed
    const bool key_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));

    if(action==GLFW_PRESS && key_shift)
    {
        // Create the 3D ray passing by the selected point on the screen
        const ray r = picking_ray(scene.camera, x,y);

        // Check if this ray intersects a position (represented by a birdSet.sphere)
        //  Loop over all positions and get the intersected position (the closest one in case of multiple intersection)
        for(animal_setting birdSet: birdSet){
        const size_t N = birdSet.keyframe_position.size();
        birdSet.picked_object = -1;
        float distance_min = 0.0f;
        for(size_t k=0; k<N; ++k)
        {
            const vec3 c = birdSet.keyframe_position[k];
            const picking_info info = ray_intersect_sphere(r, c, 0.1f);

            if( info.picking_valid ) // the ray intersects a birdSet.sphere
            {
                const float distance = norm(info.intersection-r.p); // get the closest intersection
                if( birdSet.picked_object==-1 || distance<distance_min ){
                    birdSet.picked_object = k;
                }
            }
        }
        }
    }

}

void scene_exercise::mouse_move(scene_structure& scene, GLFWwindow* window)
{
    // Mouse move is used to translate a position once selected
    // ******************************************************************** //

    // Window size
    int w=0;
    int h=0;
    glfwGetWindowSize(window, &w, &h);

    // Current cursor position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Convert pixel coordinates to relative screen coordinates between [-1,1]
    const float x = 2*float(xpos)/float(w)-1;
    const float y = 1-2*float(ypos)/float(h);

    // Check that the mouse is clicked (drag and drop)
    const bool mouse_click_left  = (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT )==GLFW_PRESS);
    const bool key_shift = (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT));

    for(animal_setting birdSet: birdSet){
    const size_t N = birdSet.keyframe_position.size();
    if(mouse_click_left && key_shift && birdSet.picked_object!=-1)
    {
        // Translate the selected object to the new pointed mouse position within the camera plane
        // ************************************************************************************** //

        // Get vector orthogonal to camera orientation
        const mat4 M = scene.camera.camera_matrix();
        const vec3 n = {M(0,2),M(1,2),M(2,2)};

        // Compute intersection between current ray and the plane orthogonal to the view direction and passing by the selected object
        const ray r = picking_ray(scene.camera, x,y);
        vec3& p0 = birdSet.keyframe_position[birdSet.picked_object];
        const picking_info info = ray_intersect_plane(r,n,p0);

        // translate the position
        p0 = info.intersection;

        // Make sure that duplicated positions are moved together
        int Ns = N;
        if(birdSet.picked_object==0 || birdSet.picked_object==Ns-3){
            birdSet.keyframe_position[0] = info.intersection;
            birdSet.keyframe_position[N-3] = info.intersection;
        }
        if(birdSet.picked_object==1 || birdSet.picked_object==Ns-2){
            birdSet.keyframe_position[1] = info.intersection;
            birdSet.keyframe_position[N-2] = info.intersection;
        }
        if(birdSet.picked_object==2 || birdSet.picked_object==Ns-1){
            birdSet.keyframe_position[2] = info.intersection;
            birdSet.keyframe_position[N-1] = info.intersection;
        }

    }}
}

#endif
