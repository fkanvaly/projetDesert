
#include "flower.hpp"



vcl::mesh_drawable createFlower(float size){

    GLuint texture_flower;
    vcl::mesh_drawable billboard_flower;
    billboard_flower = create_billboard_surface(size);
    billboard_flower.uniform_parameter.shading  = {1,0,0};
    texture_flower = texture_gpu(vcl::image_load_png("data/flower/flower.png"));
    billboard_flower.texture_pos=texture_flower;
    return billboard_flower;
}

vcl::mesh create_billboard_surface(float size)
{
    vcl::mesh billboard;
    billboard.position = {{-size,0,0}, {size,0,0}, {size,0.2f,0}, {-size,0.2f,0}};
    billboard.texture_uv = {{0,1}, {1,1}, {1,0}, {0,0}};
    billboard.connectivity = {{0,1,2}, {0,2,3}};

    return billboard;
}
