
#include "objet_du_monde.hpp"



vcl::mesh_drawable createCactus(){

    vcl::mesh_drawable cactus= load_obj("data/cactus/cactus.obj","data/cactus/cactus.mtl");

    return cactus;
}

