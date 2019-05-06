
#include "tente.hpp"

vcl::mesh_drawable_hierarchy createTente(){
    vcl::mesh_drawable_hierarchy tente;

    vcl::mesh_drawable drap= load_obj("data/tente/tente.obj","data/tente/tente.mtl");

    vcl::mesh_drawable support= load_obj("data/tente/support.obj","data/tente/support.mtl");

    vcl::mesh_drawable tapis= load_obj("data/tente/tapis.obj","data/tente/tapis.mtl");
    tapis.uniform_parameter.color={0.56,0.40,0.16};

    vcl::mesh_drawable corde= load_obj("data/tente/corde.obj","data/tente/corde.mtl");

    vcl::mesh_drawable feu= load_obj("data/tente/feu.obj","data/tente/feu.mtl");
    feu.uniform_parameter.color={0.80,0.38,0.20};

    vcl::mesh_drawable caillou_feu= load_obj("data/tente/caillou_feu.obj","data/tente/caillou_feu.mtl");
    caillou_feu.uniform_parameter.color={0.8,0.70,0.80};

    tente.add_element(drap,"drap","root");
    tente.add_element(support,"support","tronc",{0.0,0.0,0.0});
    tente.add_element(tapis,"tapis","tronc",{0.0,0.0,0.0});
    tente.add_element(corde,"corde","tronc",{0.0,0.0,0.0});
    tente.add_element(feu,"feu","tronc",{0.0,0.0,0.0});
    tente.add_element(caillou_feu,"caillou_feu","tronc",{0.0,0.0,0.0});

    return tente;
}
