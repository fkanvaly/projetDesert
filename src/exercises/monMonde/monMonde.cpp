
#include "monMonde.hpp"


#ifdef MON_MONDE

// Add vcl namespace within the current one - Allows to use function from vcl library without explicitely preceeding their name with vcl::
using namespace vcl;

//les fonctions
mesh create_grid(const gui_scene_structure& gui_scene);

/** This function is called before the beginning of the animation loop
    It is used to initialize all part-specific data */
void scene_exercise::setup_data(std::map<std::string,GLuint>& , scene_structure& scene, gui_structure& )
{
    //initialisation du terrain
    update_terrain();
    terrain_tex = texture_gpu( image_load_png("data/terrain/sabl.png") );

    //Creation des objets
    arbre=createArbre(); arbre.scaling=0.1;
    //cactus=createCactus(); cactus.uniform_parameter.scaling; cactus.uniform_parameter.translation={0,1,5};

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

    // Dessin des objets
    arbre.draw(shaders["mesh"], scene.camera);
    //cactus.draw(shaders["mesh"], scene.camera);

    if(gui_scene.wireframe){
        arbre.draw(shaders["wireframe"], scene.camera);
        terrain.draw(shaders["wireframe"], scene.camera);
        //cactus.draw(shaders["mesh"], scene.camera);
    }

}
void scene_exercise::update_terrain()
{
    // Clear memory in case of pre-existing terrain
    terrain.data_gpu.clear();

    // Create visual terrain surface
    terrain = create_grid(gui_scene);
    terrain.uniform_parameter.color = {0.83,0.69,0.43};
    terrain.uniform_parameter.shading.specular = 0.0f;
    terrain.uniform_parameter.shading.ambiant=1.0f;
}

void scene_exercise::displayTerrain(std::map<std::string,GLuint>& shaders, scene_structure& scene){
    //Dessiner le terrain


    // Display terrain *************************************
    glPolygonOffset( 1.0, 1.0 );
    glBindTexture(GL_TEXTURE_2D, terrain_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    terrain.draw(shaders["mesh"], scene.camera);

    //  Avoids to use the previous texture for another object
    glBindTexture(GL_TEXTURE_2D, scene.texture_white);
    //End terrain-------------------------------------------
}
void scene_exercise::set_gui()
{
    ImGui::Checkbox("Wireframe", &gui_scene.wireframe);

    ImGui::Separator();
    ImGui::Text("Perlin parameters");

    float height_min = 0.1f;
    float height_max = 2.0f;
    if( ImGui::SliderScalar("Height", ImGuiDataType_Float, &gui_scene.height, &height_min, &height_max) )
        update_terrain();

    float scaling_min = 0.1f;
    float scaling_max = 10.0f;
    if( ImGui::SliderScalar("(u,v) Scaling", ImGuiDataType_Float, &gui_scene.scaling, &scaling_min, &scaling_max) )
        update_terrain();

    int octave_min = 1;
    int octave_max = 10;
    if( ImGui::SliderScalar("Octave", ImGuiDataType_S32, &gui_scene.octave, &octave_min, &octave_max) )
        update_terrain();

    float persistency_min = 0.1f;
    float persistency_max = 0.9f;
    if( ImGui::SliderScalar("Persistency", ImGuiDataType_Float, &gui_scene.persistency, &persistency_min, &persistency_max) )
        update_terrain();
}



#endif
