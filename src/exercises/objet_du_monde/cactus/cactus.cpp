
#include "cactus.hpp"



vcl::mesh_drawable_hierarchy createCactus(){
    vcl::mesh_drawable_hierarchy cactus;
    vcl::mesh_drawable tronc= load_obj("data/cactus/tronc.obj","data/cactus/tronc.mtl");
    vcl::mesh_drawable epine= load_obj("data/cactus/epine.obj","data/cactus/epine.mtl");
    epine.uniform_parameter.color={0,0.2,0};
    cactus.add_element(tronc,"tronc","root");
    cactus.add_element(epine,"epine","tronc");
    return cactus;
}


