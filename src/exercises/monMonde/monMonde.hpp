#pragma once

#include "../../exercises/base_exercise/base_exercise.hpp"
#include "../loadobj/loadobj.hpp"
#include "../objet_du_monde/objet_du_monde.hpp"

#include <string>

#ifdef MON_MONDE

struct user_parameters_structure
{
    float m;    // Global mass (to be divided by the number of particles)
    float K;    // Spring stiffness
    float mu;   // Damping
    float wind; // Wind magnitude;
};

struct simulation_parameters_structure
{
    float m;  // mass
    float L0; // spring rest length
};

struct snake_tail
{
    size_t size_max=500;
    std ::vector<vcl::vec3> tail;
};

struct scene_exercise : base_scene_exercise
{

    /** A part must define two functions that are called from the main function:
     * setup_data: called once to setup data before starting the animation loop
     * frame_draw: called at every displayed frame within the animation loop
     *
     * These two functions receive the following parameters
     * - shaders: A set of shaders.
     * - scene: Contains general common object to define the 3D scene. Contains in particular the camera.
     * - data: The part-specific data structure defined previously
     * - gui: The GUI structure allowing to create/display buttons to interact with the scene.
    */

    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void set_gui();

    //creater
    void createTreePos();

    //setup
    void setup_sprite(vcl::vec3 p);
    void setup_snake();

    //updater
    void update_terrain();
    void update_eau();
    void update_eau2();
    void update_flower_position();
    void update_cactus_position();
    void update_rock_position();
    void update_rock_size();
    void update_tree2_position();
    void update_amort_position();
    void update_path(gui_scene_structure& gui, const int k);

    //Displayer
    void display_drapeau(std::map<std::string,GLuint>& shaders, scene_structure& scene );
    void displayTerrain(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayMaison(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    void displayEau(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayEau2(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySousEau(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    void displayTree(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySkybox(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayOiseau(int k, vcl::mesh_drawable_hierarchy& oiseau, animal_setting& birdSet, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayFlower(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayCactus(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayRock(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayTree2(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySprite(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayArbreMort(std::map<std::string, GLuint> &shaders, scene_structure &scene);
    void displayGoat(int k, vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySnake(int k, vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    //GUI
    gui_scene_structure gui_scene;

    //INTERACTION----
    // Called every time the mouse is clicked
    void mouse_click(scene_structure& scene, GLFWwindow* window, int button, int action, int mods);
    // Called every time the mouse is moved
    void mouse_move(scene_structure& scene, GLFWwindow* window);


    // Mes objets*******************************************************************************************************//
    //statue
    vcl::mesh_drawable statue;
    vcl::mesh_drawable drone;
    // Tree
    const size_t N_tree = 5;
    vcl::mesh_drawable_hierarchy tree;
    std::vector<vcl::vec3> tree_pos;

    // Cactus----------------------------------------------------------------------------------------------------------// *
    const size_t N_cactus = 50;
    vcl::mesh_drawable_hierarchy cactus;
    vcl::mesh_drawable cac2;
    std::vector<vcl::vec3> cactus_position;
    std::vector<float> choixCac;

    // Weed----------------------------------------------------------------------------------------------------------// *
    const size_t N_tree2 = 10;
    vcl::mesh_drawable_hierarchy tree2;
    std::vector<vcl::vec3> tree2_position;

    //tente----------------------------------------------------------------------------------------------------------// *
    vcl::mesh_drawable_hierarchy tente;

    //pot
    vcl::mesh_drawable pot;
    vcl::vec3 pot_position;
    vcl::mesh_drawable sauce;
    //chameau
    vcl::mesh_drawable chameau;
    //feu
    std::vector<fire_particle> particles;
    std::vector<float> particles_t;
    GLuint sprite_texture;

    vcl::mesh_drawable sprite;
    animal_setting spriteSet;

    //drapeau
    vcl::mesh_drawable poteau;

    //maison
    vcl::mesh_drawable maison;
    vcl::mesh_drawable maison2;

    //nuage
    const size_t N_cloud = 15;
    float cloud_height = 15;
    vcl::mesh_drawable cloud;
    std::vector<vcl::vec3> cloud_position;


    //ANIMALS MOVING----------------------------------------------------------------------------------------------------------// *
    //oiseau
    const size_t N_bird=5;
    std::vector<vcl::mesh_drawable_hierarchy> birds;
    std::vector<animal_setting> birdSet;
    std::vector<float> bird_R;
    std::vector<float> bird_rotation;
    std::vector<vcl::vec3> bird_pos;
    std::vector<vcl::vec3> birdP_init;

    //goat
    const size_t N_goat=4;
    std::vector<vcl::mesh_drawable_hierarchy> goats;
    std::vector<animal_setting> goatSets;
    std::vector<vcl::vec3> goat_pos;
    std::vector<vcl::vec3> goatP_inits;

    //snake
    const size_t N_snake=4;
    std::vector<vcl::mesh_drawable_hierarchy> snakes;
    std::vector<animal_setting> snakeSets;
    std::vector<vcl::vec3> snake_pos;
    std::vector<vcl::vec3> snakeP_inits;
    GLuint texture_snake;
    std::vector<snake_tail> queue;
    vcl::mesh_drawable snake_sphere;


    //croc
    vcl::mesh_drawable_hierarchy croc;
    vcl::vec2 croc_xy = {0,-20};

    //croc
    vcl::mesh_drawable_hierarchy boat;
    vcl::vec2 boat_xy = {0,-40};

    //flower----------------------------------------------------------------------------------------------------------// *
    vcl::mesh_drawable flower;
    std::vector<vcl::vec3> flower_position;

    //arbremort----------------------------------------------------------------------------------------------------------// *
    const size_t N_amort = 20;
    vcl::mesh_drawable arbremort;
    std::vector<vcl::vec3> arbremort_position;
    GLuint arbremortTex;
    std::vector<vcl::vec3> amort_position;

    //caillou----------------------------------------------------------------------------------------------------------// *
    const size_t N_rock=50;
    std::vector<vcl::vec3> rock_position;
    std::vector<float> rock_size;
    vcl::mesh_drawable caillou;
    std::vector<vcl::vec3> caillou_position;
    GLuint caillouTex;

    //skybox----------------------------------------------------------------------------------------------------------//*
    vcl::mesh_drawable skybox;
    GLuint texture_skybox;

    //Le terrain-------------------------------------------------------------------------------------------------------//*
    GLuint terrain_tex;
    vcl::mesh_drawable terrain;
    vcl::mesh_drawable raccord;
    vcl::mesh_drawable underTerrain;

    //Eau-------------------------------------------------------------------------------------------------------//*
    GLuint eau_tex;
    vcl::mesh_drawable eau;
    vcl::mesh_drawable eau2;
    vcl::mesh_drawable sousEau;



    //drapeau
    // Particles parameters
    std::vector<vcl::vec3> position;
    std::vector<vcl::vec3> speed;
    std::vector<vcl::vec3> force;

    // Simulation parameters
    simulation_parameters_structure simulation_parameters; // parameters that user can control directly
    user_parameters_structure user_parameters;             // parameters adjusted with respect to mesh size (not controled directly by the user)

    // Cloth mesh elements
    vcl::mesh_drawable cloth;              // Visual model for the cloth
    std::vector<vcl::vec3> normals;        // Normal of the cloth used for rendering and wind force computation
    std::vector<vcl::index3> connectivity; // Connectivity of the triangular model

    // Store index and position of vertices constrained to have a fixed 3D position
    std::map<int,vcl::vec3> positional_constraints;

    // Textures
    GLuint texture_cloth;
    GLuint texture_wood;

    // Visual elements of the scene
    vcl::mesh_drawable sphere;

    // Gui parameters
    bool gui_display_wireframe=false;
    bool gui_display_texture=true;

    // Parameters used to control if the simulation runs when a numerical divergence is detected
    bool simulation_diverged; // Active when divergence is detected
    bool force_simulation;    // Force to run simulation even if divergence is detected

    vcl::timer_event flag_timer;
    vcl:: timer_interval flag_timer1;



    void initialize();
    void compute_forces(float h);
    void numerical_integration(float h);
    void hard_constraints();

};

#endif
