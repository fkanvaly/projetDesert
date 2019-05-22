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


/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{

    //initialisation du terrain
    update_terrain();
    terrain_tex = texture_gpu( image_load_png("data/terrain/sabl.png") );
    raccord = create_underTerrain(gui_scene);
    raccord.uniform_parameter.color={0.9,0.7,0.6};
    raccord.uniform_parameter.shading.ambiant=0.5;

    //eau
    eau= create_eau(gui_scene);
    gui_scene.eau_time.t_min=0;
    gui_scene.eau_time.t_max=100;
    gui_scene.eau_time.scale=0.5f;
    gui_scene.eau_time.update();
    update_eau();
    gui_scene.eau_time.scale=0.1;

    eau_tex=texture_gpu( image_load_png("data/textures/water.png") );


    //Creation des objets********************************************************************************************************

    //statue
//    statue=load_obj("data/statue/statue.obj");
//    statue.uniform_parameter.color={0.56,0.40,0.16};
//    statue.uniform_parameter.shading.specular=0;
//    statue.uniform_parameter.shading.ambiant=0.8;
//    statue.uniform_parameter.translation={-24,20,10};
//    statue.uniform_parameter.scaling=0.04;
//    statue.uniform_parameter.rotation=rotation_from_axis_angle_mat3({0,1,0}, 3.14/3.5);

    //maison
    maison=load_obj("data/maison/maison.obj","data/maison/maison.mtl");
    maison.uniform_parameter.rotation=rotation_from_axis_angle_mat3({0,0,1}, -3.14f*0.5);
    maison.uniform_parameter.translation={-8,-20,0};
    maison.uniform_parameter.scaling=1.7;
    maison.uniform_parameter.shading.diffuse=1;
    maison.uniform_parameter.shading.specular=0;


    //Tree
    tree=createTree(); tree.scaling=0.08;
    createTreePos();

    tree2=createTree2();
    update_tree2_position();

    //Oiseau
    birds.resize(N_bird);
    birdSet.resize(N_bird);

    birdP_init.resize(N_bird);

    bird_R={8.0,4.0};

    for (int k=0; k<N_bird; ++k) {
        birds[k] = create_oiseau();
        birds[k].rotation("body")= rotation_from_axis_angle_mat3({1,0,0}, 3.14f*0.5);
        birds[k].translation("body")={0,0,2};
        birds[k].scaling=0.5f;

        birdSet[k]= init_birdSet(gui_scene, 8.0f);
        birdSet[k].N=20;

        birdP_init[k]={0,0,0};

        update_path(birdSet[k], gui_scene,bird_R[k]);
    }



    goat = create_goat();
    goatSet = init_goatSet({20,20,0},gui_scene);
    goat.scaling=0.2;

    //flower
    flower=createFlower(0.1f);
    update_flower_position();

    //cactus
    cactus=createCactus(); cactus.scaling=0.3;
    cactus.translation("tronc")={0,0,10};
    update_cactus_position();

    cac2=load_obj("data/cactus/cac2.obj");
    cac2.uniform_parameter.color={0.162093, 0.319995, 0.093024};
    cac2.uniform_parameter.shading.ambiant=1;
    cac2.uniform_parameter.shading.specular=0;
    cac2.uniform_parameter.scaling=1.5;

    //rock
    caillou=load_obj("data/arbre/caillou.obj");
    caillou.uniform_parameter.color={0.6 ,0.6, 0.6};
    caillouTex=texture_gpu(image_load_png("data/arbre/caillou.png"));
    update_rock_position();
    update_rock_size();

    //tente------------/
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
    //sprite=create_sprite();
    //spriteSet = init_sprite();
    //sauce
    sauce= mesh_primitive_disc(0.2);
    sauce.uniform_parameter.color={0.8, 0.2, 0.1};
    sauce.uniform_parameter.translation=pot_position +vec3(0,0,0.16);


    //arbremort
    arbremort=load_obj("data/arbre/arbre_mort.obj");
    arbremort.uniform_parameter.scaling=0.3;
    arbremort.uniform_parameter.color={1 ,1, 1};
    arbremortTex=texture_gpu(image_load_png("data/arbre/wood.png"));
    update_amort_position();

    //skybox
    skybox = create_skybox();
    skybox.uniform_parameter.shading = {1,0,0};
    skybox.uniform_parameter.rotation = rotation_from_axis_angle_mat3({1,0,0},-3.014f/2.0f);
    texture_skybox = texture_gpu(image_load_png("data/skybox/desertbox.png"));

    // Setup initial camera mode and position
    scene.camera.camera_type = camera_control_spherical_coordinates;
    scene.camera.scale = 10.0f;
    scene.camera.apply_rotation(0,0,0,1.2f);

}


/** This function is called at each frame of the animation loop.
    It is used to compute time-varying argument and perform data data drawing */

void scene_exercise::frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& )
{
    set_gui();

    glEnable( GL_POLYGON_OFFSET_FILL ); // avoids z-fighting when displaying wireframe

    displayTerrain(shaders,scene);

    displayTree(shaders,scene);

    displaySkybox(shaders,scene);

    displayRock(shaders,scene);

    displayTree2(shaders,scene);

    displayOiseau(oiseau, birdSet, shaders,scene);
    displayGoat(goat, goatSet, shaders,scene);


    //Dessin des objets simple
    // ////******a decommenter****////
    //statue.draw(shaders["mesh"], scene.camera);

    displayMaison(shaders,scene);
    tente.draw(shaders["mesh"], scene.camera);
    chameau.draw(shaders["mesh"], scene.camera);
    pot.draw(shaders["mesh"], scene.camera);
    sauce.draw(shaders["mesh"], scene.camera);
    displaySprite(shaders,scene);

    displayArbreMort(shaders,scene);


    raccord.draw(shaders["mesh"], scene.camera);


    displayCactus(shaders,scene);
    displayFlower(shaders,scene);
    displayEau(shaders,scene);




}

/************************************
*             UPDATER               *
************************************/
void scene_exercise::setup_sprite(vec3 p)
{
    sprite = vcl::mesh_drawable(vcl::mesh_primitive_quad({-1,1,0},{-1,-1,0},{1,-1,0},{1,1,0}));
    sprite.uniform_parameter.scaling = 5.0f;

    // The sprite texture .png file with alpha channel used as transparency parameter
    sprite.texture_pos = texture_gpu(image_load_png("data/sprite_smoke/smoke.png"));
    gui_scene.fire_timer.periodic_event_time_step=0.5;



    // Set initial position of several particles
    fire_particle particle;
    particle.p = p;
    particles.push_back(particle);
}

void scene_exercise::update_path(animal_setting& birdSet , gui_scene_structure& gui, const float R)
{
    birdSet.keyframe_position=bird_path(gui, birdSet.N, R, 5.0);
    birdSet.keyframe_time = create_keyTime(birdSet.N);

    birdSet.animal_timer.t_min = birdSet.keyframe_time[1];
    birdSet.animal_timer.t_max = birdSet.keyframe_time[birdSet.keyframe_time.size()-2];
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
            const float y = 45*(v-0.5f)-28;

            //const float z = h(x,y,gui_scene.eau_time.t)*height;
            const float z = height*noise-5.5;


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

void scene_exercise::update_cactus_position()
{

    for(size_t k=0; k<N_cactus; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const float choix=distrib(generator);
        const vec3 p = evaluate_terrain(u,v,gui_scene);
        cactus_position.push_back(p);
        choixCac.push_back(choix);
    }
}

void scene_exercise::update_amort_position()
{

    for(size_t k=0; k<N_amort; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v,gui_scene);
        amort_position.push_back(p);
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

void scene_exercise::update_tree2_position()
{

    for(size_t k=0; k<N_tree2; ++k)
    {
        const float u = 0.025f+0.95f*distrib(generator);
        const float v = 0.025f+0.95f*distrib(generator);
        const vec3 p = evaluate_terrain(u,v,gui_scene);
        tree2_position.push_back(p);
    }
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
    //std::cout<<"t : "<<t<<std::endl;
    //std::cout<<"->dt : "<<dt<<std::endl;

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

void scene_exercise::displayOiseau(vcl::mesh_drawable_hierarchy& oiseau, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene){
    Set.animal_timer.update();
    const float t = Set.animal_timer.t;



    // ********************************************* //
    // Compute interpolated position at time t
    // ********************************************* //
    const size_t idx = index_at_value(t, Set.keyframe_time);

    // Assume a closed curve trajectory
    const size_t N = Set.keyframe_time.size();



    // Linear interpolation
    // Linear interpolation
    const float t_1 = Set.keyframe_time[idx-1];
    const float t0 = Set.keyframe_time[idx];
    const float t1 = Set.keyframe_time[idx+1];
    const float t2 = Set.keyframe_time[idx+2];

    const vec3& p_1 = Set.keyframe_position[idx-1];
    const vec3& p0 = Set.keyframe_position[idx];
    const vec3& p1 = Set.keyframe_position[idx+1];
    const vec3& p2 = Set.keyframe_position[idx+2];


    //const vec3 p = linear_interpolation(t,t1,t2,p1,p2);
    const vec3 p = cardinal_spline_interpolation(t,t_1,t0,t1,t2,p_1,p0,p1,p2);
    Set.trajectory.add_point(p);

    //Draw movement
    //const float t = timer.t;

    oiseau.rotation("body") = rotation_from_axis_angle_mat3({1,0,0}, 0.1*std::sin(2*3.14f*t) );

    oiseau.rotation("arm_top_left") = rotation_from_axis_angle_mat3({0,0,1}, std::sin(2*3.14f*(t-0.4f)) );
    oiseau.rotation("arm_bottom_left") = rotation_from_axis_angle_mat3({0,0,1}, std::sin(2*3.14f*(t-0.6f)) );

    oiseau.rotation("arm_top_right") = rotation_from_axis_angle_mat3({0,0,-1}, std::sin(2*3.14f*(t-0.4f)) );
    oiseau.rotation("arm_bottom_right") = rotation_from_axis_angle_mat3({0,0,-1}, std::sin(2*3.14f*(t-0.6f)) );


    // Draw current position
    oiseau.translation("body") = p;

    //il faisait des rotations bizarre
    vec3 ro = p-birdP_init;
    if(p.x<0)
        oiseau.rotation("body") = rotation_between_vector_mat3({0,-1.0f,0},ro)*rotation_from_axis_angle_mat3({1.0f,0,0},3.14/2);
    if(p.x>0)
        oiseau.rotation("body") = rotation_between_vector_mat3({0,1.0f,0},ro)
                                   *rotation_from_axis_angle_mat3({1.0f,0,0},3.14/2)
                                    *rotation_from_axis_angle_mat3({-1,0,0},3.14)
                                    *rotation_from_axis_angle_mat3({0,0,1},3.14); //solution au bug

    birdP_init=p;
    oiseau.draw(shaders["mesh"],scene.camera);




    // Draw birdSet.sphere at each keyframe position
    for(size_t k=0; k<N; ++k)
    {
        const vec3& p_keyframe = Set.keyframe_position[k];
        Set.sphere.uniform_parameter.translation = p_keyframe;
        Set.sphere.draw(shaders["mesh"],scene.camera);
    }


    // Draw segments between each keyframe
    for(size_t k=0; k<Set.keyframe_position.size()-1; ++k)
    {
        const vec3& pa = Set.keyframe_position[k];
        const vec3& pb = Set.keyframe_position[k+1];

        Set.segment_drawer.uniform_parameter.p1 = pa;
        Set.segment_drawer.uniform_parameter.p2 = pb;
        Set.segment_drawer.draw(shaders["segment_im"], scene.camera);
    }

    // Draw moving point trajectory
    Set.trajectory.draw(shaders["curve"], scene.camera);

    // Draw selected oiseau in red
    if( Set.picked_object!=-1 )
    {
        const vec3& p_keyframe = Set.keyframe_position[Set.picked_object];
        Set.sphere.uniform_parameter.color = vec3(1,0,0);
        Set.sphere.uniform_parameter.scaling = 0.06f;
        Set.sphere.uniform_parameter.translation = p_keyframe;
        Set.sphere.draw(shaders["mesh"],scene.camera);
        Set.sphere.uniform_parameter.color = vec3(1,1,1);
        Set.sphere.uniform_parameter.scaling = 0.05f;
    }
}


void scene_exercise::displayGoat(vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene){
    Set.animal_timer.update();
    const float t = Set.animal_timer.t;



    // ********************************************* //
    // Compute interpolated position at time t
    // ********************************************* //
    const size_t idx = index_at_value(t, Set.keyframe_time);

    // Assume a closed curve trajectory
    const size_t N = Set.keyframe_time.size();



    // Linear interpolation
    // Linear interpolation
        const float t_1 = Set.keyframe_time[idx-1];
        const float t0 = Set.keyframe_time[idx];
        const float t1 = Set.keyframe_time[idx+1];
        const float t2 = Set.keyframe_time[idx+2];

        const vec3& p_1 = Set.keyframe_position[idx-1];
        const vec3& p0 = Set.keyframe_position[idx];
        const vec3& p1 = Set.keyframe_position[idx+1];
        const vec3& p2 = Set.keyframe_position[idx+2];


    //const vec3 p = linear_interpolation(t,t1,t2,p1,p2);
    const vec3 p = cardinal_spline_interpolation(t,t_1,t0,t1,t2,p_1,p0,p1,p2);
    Set.trajectory.add_point(p);

    //Draw movement
    //const float t = timer.t;

    //goat.rotation("body") = rotation_from_axis_angle_mat3({1,0,0}, 0.1*std::sin(2*3.14f*t) );


    // Draw current position
    goat.translation("body") = p;
    goat.rotation("body") = rotation_between_vector_mat3({0,-1.0f,0},p-goatP_init)*rotation_from_axis_angle_mat3({0,0,1},3.14/2);

    goatP_init=p;
    goat.draw(shaders["mesh"],scene.camera);




    // Draw birdSet.sphere at each keyframe position
    for(size_t k=0; k<N; ++k)
    {
        const vec3& p_keyframe = Set.keyframe_position[k];
        Set.sphere.uniform_parameter.translation = p_keyframe;
        Set.sphere.draw(shaders["mesh"],scene.camera);
    }


    // Draw segments between each keyframe
    for(size_t k=0; k<Set.keyframe_position.size()-1; ++k)
    {
        const vec3& pa = Set.keyframe_position[k];
        const vec3& pb = Set.keyframe_position[k+1];

        Set.segment_drawer.uniform_parameter.p1 = pa;
        Set.segment_drawer.uniform_parameter.p2 = pb;
        Set.segment_drawer.draw(shaders["segment_im"], scene.camera);
    }

    // Draw moving point trajectory
    Set.trajectory.draw(shaders["curve"], scene.camera);

    // Draw selected oiseau in red
    if( Set.picked_object!=-1 )
    {
        const vec3& p_keyframe = Set.keyframe_position[Set.picked_object];
        Set.sphere.uniform_parameter.color = vec3(1,0,0);
        Set.sphere.uniform_parameter.scaling = 0.06f;
        Set.sphere.uniform_parameter.translation = p_keyframe;
        Set.sphere.draw(shaders["mesh"],scene.camera);
        Set.sphere.uniform_parameter.color = vec3(1,1,1);
        Set.sphere.uniform_parameter.scaling = 0.05f;
    }
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

void scene_exercise::displayCactus(std::map<std::string,GLuint>& shaders, scene_structure& scene)
{
    if(!gui_scene.cactus)
        return ;

    const size_t N_grass = cactus_position.size();

    for(size_t k=0; k<N_grass; ++k)
    {
        const vec3& p = cactus_position[k];
        if(!onGround(p)){
            if(choixCac[k]>0.5){
            cactus.translation("tronc") = p + vec3{0,0,0.5};
            glPolygonOffset( 1.0, 1.0 );
            cactus.draw(shaders["mesh"], scene.camera);}
            else{
                cac2.uniform_parameter.translation=p;
                cac2.draw(shaders["mesh"], scene.camera);
            }
        }
    }


    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_grass; ++k)
        {
            const vec3& p = flower_position[k];
            if(!onGround(p)){
            cactus.translation("tronc") = p;
            glPolygonOffset( 1.0, 1.0 );
            cactus.draw(shaders["wireframe"], scene.camera);
            }
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

void scene_exercise::displayArbreMort(std::map<std::string, GLuint> &shaders, scene_structure &scene)
{

    const size_t N_amort = amort_position.size();

    for(size_t k=0; k<N_amort; ++k)
    {
        const vec3& p = amort_position[k];
        if(!onGround(p)){
            arbremort.uniform_parameter.translation = p + vec3{0,0,-0.2};

            glPolygonOffset( 1.0, 1.0 );
            // Display amort *************************************
            glPolygonOffset( 1.0, 1.0 );
            glBindTexture(GL_TEXTURE_2D, arbremortTex);

            arbremort.draw(shaders["mesh"], scene.camera);

            //  Avoids to use the previous texture for another object
            glBindTexture(GL_TEXTURE_2D, scene.texture_white);

        }
    }



}

void scene_exercise::displayTree2(std::map<std::string, GLuint> &shaders, scene_structure &scene)
{

    const size_t N_grass = tree2_position.size();

    for(size_t k=0; k<N_grass; ++k)
    {
        const vec3& p = tree2_position[k];
        if(!onGround(p)){
            tree2.translation("tronc")= p + vec3{0,0,0.05};
            glPolygonOffset( 1.0, 1.0 );
            tree2.draw(shaders["mesh"], scene.camera);
        }
    }


    if( gui_scene.wireframe ){
        for(size_t k=0; k<N_grass; ++k)
        {
            const vec3& p = tree2_position[k];
            if(!onGround(p)){
            tree2.translation("root")= p;
            glPolygonOffset( 1.0, 1.0 );
            tree2.draw(shaders["wireframe"], scene.camera);
            }
        }
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
        y=y-4;
        tree_pos.push_back({R,y,0});
        tree_pos.push_back({-R,y,0});
    }

}

vcl::mesh create_skybox()
{
    const vec3 p000 = {-1,-1,-1};
    const vec3 p001 = {-1,-1, 1};
    const vec3 p010 = {-1, 1,-1};
    const vec3 p011 = {-1, 1, 1};
    const vec3 p100 = { 1,-1,-1};
    const vec3 p101 = { 1,-1, 1};
    const vec3 p110 = { 1, 1,-1};
    const vec3 p111 = { 1, 1, 1};

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

void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);
    ImGui::Checkbox("Skybox", &gui_scene.skybox);

    //Goat
    ImGui::Separator();
    ImGui::Text("Goat parameters");
    const float goatTime_scale_min = 1.f;
    const float goatTime_scale_max = 3.f;
    ImGui::SliderFloat("Time scale", &goatSet.animal_timer.scale, goatTime_scale_min, goatTime_scale_max);


    //Eau
    ImGui::Separator();
    ImGui::Text("Water parameters");

    float eau_height_min = 0.01f;
    float eau_height_max = 2.0f;
    if( ImGui::SliderScalar("Height eau", ImGuiDataType_Float, &gui_scene.eau_height, &eau_height_min, &eau_height_max) )
        update_eau();
    float eau_scaling_min = 0.01f;
    float eau_scaling_max = 10.0f;
    if( ImGui::SliderScalar("(u,v) Scaling eau ", ImGuiDataType_Float, &gui_scene.eau_scaling, &eau_scaling_min, &eau_scaling_max) )
        update_eau();

    int octave_min = 1;
    int octave_max = 10;
    if( ImGui::SliderScalar("Octave eau", ImGuiDataType_S32, &gui_scene.eau_octave, &octave_min, &octave_max) )
        update_eau();

    float persistency_min = 0.1f;
    float persistency_max = 0.9f;
    if( ImGui::SliderScalar("Persistency eau", ImGuiDataType_Float, &gui_scene.eau_persistency, &persistency_min, &persistency_max) )
        update_eau();

    //FOR BIRD
    ImGui::Separator();
    ImGui::Text("Bird parameters");

    ImGui::SliderFloat("Time", &birdSet.animal_timer.t, birdSet.animal_timer.t_min, birdSet.animal_timer.t_max);

    const float time_scale_min = 0.1f;
    const float time_scale_max = 3.0f;
    ImGui::SliderFloat("Time scale2", &birdSet.animal_timer.scale, time_scale_min, time_scale_max);

    float path_height_min = 0.01f;
    float path_height_max = 8.0f;
    if( ImGui::SliderScalar("Height path", ImGuiDataType_Float, &gui_scene.path_height, &path_height_min, &path_height_max) )
        update_path(birdSet,gui_scene);

    float path_scaling_min = 0.01f;
    float path_scaling_max = 10.0f;
    if( ImGui::SliderScalar("(u,v) Scaling path ", ImGuiDataType_Float, &gui_scene.path_scaling, &path_scaling_min, &path_scaling_max) )
        update_path(birdSet,gui_scene);

    int Boctave_min = 1;
    int Boctave_max = 10;
    if( ImGui::SliderScalar("Octave path", ImGuiDataType_S32, &gui_scene.path_octave, &Boctave_min, &Boctave_max) )
        update_path(birdSet,gui_scene);

    float Bpersistency_min = 0.1f;
    float Bpersistency_max = 0.9f;
    if( ImGui::SliderScalar("Persistency path", ImGuiDataType_Float, &gui_scene.path_persistency, &Bpersistency_min, &Bpersistency_max) )
        update_path(birdSet,gui_scene);

    if( ImGui::Button("Print Keyframe") )
    {
        std::cout<<"birdSet.keyframe_position={";
        for(size_t k=0; k<birdSet.keyframe_position.size(); ++k)
        {
            const vec3& p = birdSet.keyframe_position[k];
            std::cout<< "{"<<p.x<<"f,"<<p.y<<"f,"<<p.z<<"f}";
            if(k<birdSet.keyframe_position.size()-1)
                std::cout<<", ";
        }
        std::cout<<"}"<<std::endl;
    }

    //fire
    ImGui::Separator();
    ImGui::Text("Fire parameters");
    ImGui::Checkbox("Fire", &gui_scene.enable_sprite);
    float firescale_min = 0.05f;
    float firescale_max = 2.0f;
    ImGui::SliderScalar("Time period", ImGuiDataType_Float, &gui_scene.fire_timer.periodic_event_time_step, &firescale_min, &firescale_max, "%.2f s");


    //for plants
    ImGui::Separator();
    ImGui::Text("Plants parameters");
    ImGui::Checkbox("Flower", &gui_scene.flower);
    ImGui::Checkbox("cactus", &gui_scene.cactus);
    ImGui::Checkbox("flower_Texture", &gui_scene.texture);
    ImGui::Checkbox("rock", &gui_scene.rock);

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

    }
}

#endif
