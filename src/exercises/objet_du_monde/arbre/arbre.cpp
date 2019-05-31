
#include "arbre.hpp"



vcl::mesh_drawable_hierarchy createTree(){
    vcl::mesh_drawable_hierarchy arbre;

    vcl::mesh_drawable tronc= load_obj("data/arbre/tronc.obj","data/arbre/tronc.mtl");
    vcl::mesh_drawable feuille= load_obj("data/arbre/feuille.obj","data/arbre/feuille.mtl");

    tronc.uniform_parameter.scaling=0.01;
    feuille.uniform_parameter.scaling=0.01;

    arbre.add_element(tronc,"tronc","root");
    arbre.add_element(feuille,"feuille","tronc",{1.2,-4,18});

    arbre.scaling=0.5;
    return arbre;
}

vcl::mesh_drawable_hierarchy createTree2(){
    vcl::mesh_drawable_hierarchy arbre;

    vcl::mesh_drawable tronc= load_obj("data/arbre/tronc2.obj","data/arbre/tronc2.mtl");
    tronc.uniform_parameter.shading.specular=0;
    vcl::mesh_drawable feuille= load_obj("data/arbre/feuille2.obj","data/arbre/feuille2.mtl");
    feuille.uniform_parameter.shading.specular=0;
    arbre.add_element(tronc,"tronc","root");
    arbre.add_element(feuille,"feuille","tronc",{0,0,0});

    arbre.scaling=2.5;
    return arbre;
}
