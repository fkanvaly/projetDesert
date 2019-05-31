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
    //main function
    void setup_data(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void frame_draw(std::map<std::string,GLuint>& shaders, scene_structure& scene, gui_structure& gui);
    void set_gui(scene_structure& scene);

    //creater
    void createTreePos();

    //setuper
    void setup_sprite(vcl::vec3 p);
    void setup_snake();
    void setup_camera_path();

    //updater
    void update_terrain();
    void update_eau();
    void update_eau2();
    void update_flower_position();
    void update_rock_position();
    void update_rock_size();
    void update_vegetation_type();
    void update_path(gui_scene_structure& gui, const int k);
    void update_boat(animal_setting& Set,vcl::vec2 p);
    void update_camera(int i,scene_structure& scene);

    //move camera
    void move_camera(scene_structure& scene , std::map<std::string,GLuint>& shaders);

    //Displayer
    void display_drapeau(std::map<std::string,GLuint>& shaders, scene_structure& scene );
    void displayTerrain(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayMaison(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayEau(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayEau2(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayTree(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySkybox(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayOiseau(int k, vcl::mesh_drawable_hierarchy& oiseau, animal_setting& birdSet, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayFlower(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayRock(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySprite(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayGoat(vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySnake(int k, vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayBoat(vcl::mesh_drawable_hierarchy& boat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayPluie(std::map<std::string, GLuint> &shaders, scene_structure &scene);

    //GUI
    gui_scene_structure gui_scene;

    //INTERACTION----
    // Called every time the mouse is clicked
    void mouse_click(scene_structure& scene, GLFWwindow* window, int button, int action, int mods);
    // Called every time the mouse is moved
    void mouse_move(scene_structure& scene, GLFWwindow* window);

    //for camera mouvment
    animal_setting camera_set;


    /************************************
    *          OBJETS STATIQUES         *
    ************************************/
    //Le terrain-----------------------------// *
    GLuint terrain_tex;
    vcl::mesh_drawable terrain;
    vcl::mesh_drawable raccord;
    vcl::mesh_drawable underTerrain;

    //flower-----------------------------// *
    vcl::mesh_drawable flower;
    std::vector<vcl::vec3> flower_position;

    //skybox-----------------------------// *
    vcl::mesh_drawable skybox;
    GLuint texture_skybox;

    //statue-----------------------------// *
    vcl::mesh_drawable statue;
    vcl::mesh_drawable drone;

    // Tree -----------------------------// *
    const size_t N_tree = 5;
    vcl::mesh_drawable_hierarchy tree;
    std::vector<vcl::vec3> tree_pos;

    // Cactus-----------------------------// *
    const size_t N_cactus = 50;
    vcl::mesh_drawable_hierarchy cactus;
    vcl::mesh_drawable cactus2;
    std::vector<float> choixCac;

    // Arbre mort-----------------------------// *
    const size_t N_veg_rock = 30;
    std::vector<bool> tree_type;
    std::vector<vcl::vec3> tree2_position;

    //tente-----------------------------// *
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

    //nuage-----------------------------// *
    const size_t N_cloud = 15;
    float cloud_height = 15;
    vcl::mesh_drawable cloud;
    std::vector<vcl::vec3> cloud_position;

    //caillou-----------------------------// *
    const size_t N_rock=50;
    std::vector<vcl::vec3> rock_position;
    std::vector<float> rock_size;
    vcl::mesh_drawable caillou;
    std::vector<vcl::vec3> caillou_position;
    GLuint caillouTex;

    //autres-----------------------------// *
    vcl::mesh_drawable poteau;
    vcl::mesh_drawable maison;
    vcl::mesh_drawable rock2;
    std::vector<float> choixRock;
    vcl::mesh_drawable rock3;
    vcl::mesh_drawable tour;
    vcl::mesh_drawable boat4;


    /************************************
    *          OBJETS ANIMES            *
    ************************************/

    // fumée-----------------------------// *
    vcl::mesh_drawable sprite;
    animal_setting spriteSet;

    //goute d'eau nuage-----------------------------// *
    std::list<particle_structure> goute_particules;
    vcl::mesh_drawable goute;
    vcl::timer_event goute_timer;
    std::vector<vcl::vec3> pluie_position;
    vcl::timer_event pluie_timer;
    bool cond = true;


    //oiseau-----------------------------// *
    const size_t N_bird=5;
    std::vector<vcl::mesh_drawable_hierarchy> birds;
    std::vector<animal_setting> birdSet;
    std::vector<float> bird_R;
    std::vector<float> bird_rotation;
    std::vector<vcl::vec3> bird_pos;

    //goat-----------------------------// *
    const size_t N_goat=4;
    std::vector<vcl::mesh_drawable_hierarchy> goats;
    std::vector<animal_setting> goatSets;
    std::vector<vcl::vec3> goat_pos;
    std::vector<vcl::vec3> goatP_inits;

    //snake-----------------------------// *
    const size_t N_snake=4;
    std::vector<vcl::mesh_drawable_hierarchy> snakes;
    std::vector<animal_setting> snakeSets;
    std::vector<vcl::vec3> snake_pos;
    std::vector<vcl::vec3> snakeP_inits;
    GLuint texture_snake;
    std::vector<snake_tail> queue;
    vcl::mesh_drawable snake_sphere;

    //croc-----------------------------// *
    vcl::mesh_drawable_hierarchy croc;
    vcl::vec2 croc_xy = {0,-20};

    //boat-----------------------------// *
    vcl::mesh_drawable_hierarchy boat;
    vcl::vec2 boat_xy = {0,-40};

    vcl::mesh_drawable_hierarchy boat2;
    animal_setting boat2Set;
    vcl::vec2 boat2_xy = {10,-60};

    vcl::mesh_drawable_hierarchy boat3;
    animal_setting boat3Set;
    vcl::vec2 boat3_xy = {-20,-60};


    //Eau-----------------------------// *
    GLuint eau_tex;
    vcl::mesh_drawable eau;
    vcl::mesh_drawable eau2;
    vcl::mesh_drawable sousEau;



    //drapeau-----------------------------// *
    // Particles parameters
    std::vector<vcl::vec3> position;
    std::vector<vcl::vec3> speed;
    std::vector<vcl::vec3> force;

    // Simulation parameters
    simulation_parameters_structure simulation_parameters;
    user_parameters_structure user_parameters;

    // Cloth mesh elements
    vcl::mesh_drawable cloth;
    std::vector<vcl::vec3> normals;
    std::vector<vcl::index3> connectivity;

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
    bool simulation_diverged;
    bool force_simulation;

    //flag timer
    vcl::timer_event flag_timer;
    vcl:: timer_interval flag_timer1;

    //flag function
    void initialize();
    void compute_forces(float h);
    void numerical_integration(float h);
    void hard_constraints();

};

#endif
