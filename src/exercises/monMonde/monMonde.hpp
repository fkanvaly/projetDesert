#pragma once

#include "../../exercises/base_exercise/base_exercise.hpp"
#include "../loadobj/loadobj.hpp"
#include "../objet_du_monde/objet_du_monde.hpp"

#include <string>

#ifdef MON_MONDE

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
    void setup_fire(vcl::vec3 p);

    //updater
    void update_terrain();
    void update_eau();
    void update_flower_position();
    void update_cactus_position();
    void update_rock_position();
    void update_rock_size();
    void update_tree2_position();
    void update_amort_position();

    //Displayer
    void displayTerrain(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayMaison(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayEau(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayTree(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySkybox(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayOiseau(vcl::mesh_drawable_hierarchy& oiseau, animal_setting& birdSet, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayFlower(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayCactus(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayRock(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayTree2(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySprite(vcl::mesh_drawable& sprite, animal_setting& spriteSet,std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayArbreMort(std::map<std::string, GLuint> &shaders, scene_structure &scene);
    void displayGoat(vcl::mesh_drawable_hierarchy& goat, animal_setting& Set, std::map<std::string,GLuint>& shaders, scene_structure& scene);

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
    const size_t N_tree = 10;
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
    //vcl::mesh_drawable sprite;
    GLuint sprite_texture;

    vcl::mesh_drawable sprite;
    animal_setting spriteSet;


    //maison
    vcl::mesh_drawable maison;
    vcl::mesh_drawable maison2;


    //ANIMALS MOVING----------------------------------------------------------------------------------------------------------// *
    //oiseau
    vcl::mesh_drawable_hierarchy oiseau;
    animal_setting birdSet;
    vcl::vec3 birdP_init={0,0,0};
    //goat
    vcl::mesh_drawable_hierarchy goat;
    animal_setting goatSet;
    vcl::vec3 goatP_init={0,0,0};

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






};

#endif
