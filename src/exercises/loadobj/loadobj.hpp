#pragma once

#include <string>
#include "../../exercises/base_exercise/base_exercise.hpp"

vcl::mesh_drawable load_obj(const char* objfile, const char* mtlfile);
std::string load_text(const char* filename);
std::map<std::string,std::map<std::string,vcl::vec3>>  load_mtl(const char* filename);
