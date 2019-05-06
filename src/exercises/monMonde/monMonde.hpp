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

    //updater
    void update_flower_position();
    void update_cactus_position();
    void update_rock_position();
    void update_rock_size();
    void update_weed_position();
    void update_weed_size();

    //Displayer
    void displayTerrain(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayTree(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displaySkybox(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayOiseau(vcl::mesh_drawable_hierarchy& oiseau, bird_settings& birdSet, std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayFlower(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayCactus(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayRock(std::map<std::string,GLuint>& shaders, scene_structure& scene);
    void displayWeed(std::map<std::string,GLuint>& shaders, scene_structure& scene);

    //GUI
    gui_scene_structure gui_scene;

    //INTERACTION----
    // Called every time the mouse is clicked
    void mouse_click(scene_structure& scene, GLFWwindow* window, int button, int action, int mods);
    // Called every time the mouse is moved
    void mouse_move(scene_structure& scene, GLFWwindow* window);


    // Mes objets*******************************************************************************************************//
    // Tree
    const size_t N_tree = 8;
    vcl::mesh_drawable_hierarchy tree;
    std::vector<vcl::vec3> tree_pos;

    // Cactus
    const size_t N_cactus = 50;
    vcl::mesh_drawable_hierarchy cactus;
    std::vector<vcl::vec3> cactus_position;

    // Weed
    const size_t N_weed = 100;
    vcl::mesh_drawable weed;
    std::vector<vcl::vec3> weed_position;
    std::vector<float> weed_size;

    //water----------------------------------------------------------------------------------------------------------// *
    vcl::mesh_drawable eau;

    //water----------------------------------------------------------------------------------------------------------// *
    vcl::mesh_drawable_hierarchy tente;

    //rock----------------------------------------------------------------------------------------------------------// *
    const size_t N_rock=40;
    vcl::mesh_drawable rock;
    std::vector<vcl::vec3> rock_position;
    std::vector<float> rock_size;

    //oiseau----------------------------------------------------------------------------------------------------------// *
    vcl::mesh_drawable_hierarchy oiseau;
    bird_settings birdSet;

    //flower----------------------------------------------------------------------------------------------------------// *
    vcl::mesh_drawable flower;
    std::vector<vcl::vec3> flower_position;

    //skybox----------------------------------------------------------------------------------------------------------//*
    vcl::mesh_drawable skybox;
    GLuint texture_skybox;

    //Le terrain-------------------------------------------------------------------------------------------------------//*
    GLuint terrain_tex;
    vcl::mesh_drawable terrain;
    vcl::mesh_drawable raccord;
    vcl::mesh_drawable underTerrain;
    void update_terrain();


};

#endif
