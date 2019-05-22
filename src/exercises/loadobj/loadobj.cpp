#include "loadobj.hpp"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace vcl;


//std::vector<unsigned int> decoupe(std::string &text);
std::vector<unsigned int> decoupe(std::string &text){
    std::string s = text;
    std::string delimiter = "/";

    size_t pos = 0;
    std::string token;

    std::vector<unsigned int> f;
    for (int i=0;i<3;i++) {
        pos = s.find(delimiter);
        token = s.substr(0, pos);

        if(token.compare("")!=0){
            std::istringstream sval(token);
            unsigned int val;
            sval>>val;
            f.push_back(val);}

        s.erase(0, pos + delimiter.length());
    }

    return f;
}

mesh_drawable load_obj(const char* objfile, const char* mtlfile)
{
    //--------- LOAD ALL TEXTURE AND MATERIALS ----------//
    mesh obj;
    std::map<std::string,std::map<std::string,vec3>>  Mtl=load_mtl(mtlfile);
    std::string MtlName;

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices; //tableau des indice

    //fichiers temporaire
    std::vector< vcl::vec2 > temp_uvs;
    std::vector< vcl::vec3 > temp_normals;


    std::ifstream in(objfile, std::ios::in);
    if (!in)
    {
       printf("Impossible to open the file !\n");
    }

    std::string line;
//    std::cout<< "start reading obj"<< std::endl;
//    unsigned int i=0;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {
            //std::cout<< "vertex read"<< std::endl;
            std::istringstream s (line.substr(2));
            vcl::vec3 v; s >> v.x; s >> v.y; s >> v.z;
//            std::cout<< "vertex read "<< v << std::endl;
            obj.position.push_back(v);
        }
        else if (line.substr(0,3) == "vn ")
        {
            //std::cout<< "vn read"<< std::endl;
            std::istringstream s (line.substr(3));
            vcl::vec3 vn; s >> vn.x; s >> vn.z; s >> vn.y;
//            std::cout<< "vn read "<< vn << std::endl;
            //obj.normal.push_back(vn);
        }
        else if (line.substr(0,3) == "vt ")
        {
            //std::cout<< "vt read"<< std::endl;
            std::istringstream s (line.substr(3));
            vcl::vec2 vt; s >> vt.x; s >> vt.y;
//            std::cout<< "vt read "<< vt <<std::endl;
            temp_uvs.push_back(vt);
        }
        else if (line.substr(0,7) == "usemtl ")
        {
//            std::cout<< "mtl read: "<<line.substr(7)<< std::endl;
            MtlName =line.substr(7);

//            std::cout<< "mtl Ka: "<<Mtl[MtlName]["Ka"]<< std::endl;
//            std::cout<< "mtl Kd: "<<Mtl[MtlName]["Kd"]<< std::endl;
//            std::cout<< "mtl Ks: "<<Mtl[MtlName]["Ks"]<< std::endl;


        }
        else if (line.substr(0,2) == "f ")
        {
//            std::cout<< "face "<< i<< " reading"<< std::endl;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            std::istringstream s (line.substr(2));
            std::string a[3];
            s >> a[0]; s >> a[1]; s >> a[2];
            std::vector<unsigned int> f;
            for (int i=0; i<3; i++) {

                f=decoupe(a[i]); //return

                vertexIndex[i]=f[0]-1;
//                std::cout<< "vertex"<< i << " pos : "<< vertexIndex[i] <<std::endl;
                if (f.size()==3){
                    uvIndex[i]=f[1]-1;
                    normalIndex[i]=f[2]-1;
                }
                if (f.size()==2){
                    normalIndex[i]=f[1];
                }
            }

//            std::cout<< "face creation"<< std::endl;
            const unsigned int v1_pos=vertexIndex[0]; const unsigned int v2_pos=vertexIndex[1]; const unsigned int v3_pos=vertexIndex[2];

//            std::cout<<"face "<< i<<" vertex "<<0 <<": "<< obj.position[v1_pos]<< std::endl;
//            std::cout<<"face "<< i<<" vertex "<<1 <<": "<< obj.position[v2_pos]<< std::endl;
//            std::cout<<"face "<< i<<" vertex "<<2 <<": "<< obj.position[v3_pos]<< std::endl;

            obj.connectivity.push_back({v1_pos,v2_pos,v3_pos});

//            std::cout<<"face "<< i<<" connectivity : "<<v1_pos<<";"<<v2_pos<<";"<<v3_pos<< std::endl;

            obj.color.push_back({Mtl[MtlName]["Kd"].x,Mtl[MtlName]["Kd"].y,Mtl[MtlName]["Kd"].z,0.0});


            if(f.size()==3){
            unsigned int t1_pos=uvIndex[0]; unsigned int t2_pos=uvIndex[1]; unsigned int t3_pos=uvIndex[2];
            obj.texture_uv.push_back(temp_uvs[t1_pos]);
//            std::cout<<"face "<< i<<" uv "<<i <<": "<< temp_uvs[t1_pos] << std::endl;
            obj.texture_uv.push_back(temp_uvs[t2_pos]);
//            std::cout<<"face "<< i<<" uv "<<i <<": "<< temp_uvs[t2_pos] << std::endl;
            obj.texture_uv.push_back(temp_uvs[t3_pos]);
//            std::cout<<"face "<< i<<" uv "<<i <<": "<< temp_uvs[t3_pos] << std::endl;
            }

//            unsigned int n1_pos=normalIndex[0]; unsigned int n2_pos=normalIndex[1]; unsigned int n3_pos=normalIndex[2];
//            obj.normal.push_back(temp_normals[n1_pos]);
//            std::cout<<"face "<< i<<" vn of vertex"<<0 <<": "<< temp_normals[n1_pos] << std::endl;
//            obj.normal.push_back(temp_normals[n2_pos]);
//            std::cout<<"face "<< i<<" vn of vertex"<<1 <<": "<< temp_normals[n2_pos] << std::endl;
//            obj.normal.push_back(temp_normals[n3_pos]);
//            std::cout<<"face "<< i<<" vn of vertex"<<2 <<": "<< temp_normals[n3_pos] << std::endl;

//            i++;

        }
        else if (line[0] == '#')
        {
            /* ignoring this line */
        }
        else
        {
            /* ignoring this line */
        }
    }

    mesh_drawable objDrawable=obj;
    objDrawable.uniform_parameter.shading.ambiant=Mtl[MtlName]["Ka"].x;
    objDrawable.uniform_parameter.shading.specular=Mtl[MtlName]["Ks"].x;
    objDrawable.texture_obj=load_text(mtlfile);

    return objDrawable;
}

mesh_drawable load_obj(const char* objfile)
{
    //--------- LOAD ALL TEXTURE AND MATERIALS ----------//
    mesh obj;

    std::vector< unsigned int > vertexIndices, uvIndices, normalIndices; //tableau des indice

    //fichiers temporaire
    std::vector< vcl::vec2 > temp_uvs;
    std::vector< vcl::vec3 > temp_normals;


    std::ifstream in(objfile, std::ios::in);
    if (!in)
    {
       printf("Impossible to open the file !\n");
    }

    std::string line;
//    std::cout<< "start reading obj"<< std::endl;
//    unsigned int i=0;
    while (getline(in, line))
    {
        if (line.substr(0,2) == "v ")
        {
            //std::cout<< "vertex read"<< std::endl;
            std::istringstream s (line.substr(2));
            vcl::vec3 v; s >> v.x; s >> v.y; s >> v.z;
//            std::cout<< "vertex read "<< v << std::endl;
            obj.position.push_back(v);
        }
        else if (line.substr(0,3) == "vn ")
        {
            //std::cout<< "vn read"<< std::endl;
            std::istringstream s (line.substr(3));
            vcl::vec3 vn; s >> vn.x; s >> vn.y; s >> vn.z;
//            std::cout<< "vn read "<< vn << std::endl;
            //obj.normal.push_back(vn);
        }
        else if (line.substr(0,3) == "vt ")
        {
            //std::cout<< "vt read"<< std::endl;
            std::istringstream s (line.substr(3));
            vcl::vec2 vt; s >> vt.x; s >> vt.y;
//            std::cout<< "vt read "<< vt <<std::endl;
            temp_uvs.push_back(vt);
        }
        else if (line.substr(0,2) == "f ")
        {
//            std::cout<< "face "<< i<< " reading"<< std::endl;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

            std::istringstream s (line.substr(2));
            std::string a[3];
            s >> a[0]; s >> a[1]; s >> a[2];
            std::vector<unsigned int> f;
            for (int i=0; i<3; i++) {

                f=decoupe(a[i]); //return

                vertexIndex[i]=f[0]-1;
//                std::cout<< "vertex"<< i << " pos : "<< vertexIndex[i] <<std::endl;
                if (f.size()==3){
                    uvIndex[i]=f[1]-1;
                    normalIndex[i]=f[2]-1;
                }
                if (f.size()==2){
                    normalIndex[i]=f[1];
                }
            }

//            std::cout<< "face creation"<< std::endl;
            const unsigned int v1_pos=vertexIndex[0]; const unsigned int v2_pos=vertexIndex[1]; const unsigned int v3_pos=vertexIndex[2];

//            std::cout<<"face "<< i<<" vertex "<<0 <<": "<< obj.position[v1_pos]<< std::endl;
//            std::cout<<"face "<< i<<" vertex "<<1 <<": "<< obj.position[v2_pos]<< std::endl;
//            std::cout<<"face "<< i<<" vertex "<<2 <<": "<< obj.position[v3_pos]<< std::endl;

            obj.connectivity.push_back({v1_pos,v2_pos,v3_pos});

//            std::cout<<"face "<< i<<" connectivity : "<<v1_pos<<";"<<v2_pos<<";"<<v3_pos<< std::endl;


            if(f.size()==3){
            unsigned int t1_pos=uvIndex[0]; unsigned int t2_pos=uvIndex[1]; unsigned int t3_pos=uvIndex[2];
            obj.texture_uv.push_back(temp_uvs[t1_pos]);
//            std::cout<<"face "<< i<<" uv "<<i <<": "<< temp_uvs[t1_pos] << std::endl;
            obj.texture_uv.push_back(temp_uvs[t2_pos]);
//            std::cout<<"face "<< i<<" uv "<<i <<": "<< temp_uvs[t2_pos] << std::endl;
            obj.texture_uv.push_back(temp_uvs[t3_pos]);
//            std::cout<<"face "<< i<<" uv "<<i <<": "<< temp_uvs[t3_pos] << std::endl;
            }

        }
        else if (line[0] == '#')
        {
            /* ignoring this line */
        }
        else
        {
            /* ignoring this line */
        }
    }

    mesh_drawable objDrawable=obj;

    return objDrawable;
}
std::map<std::string,std::map<std::string,vec3>>  load_mtl(const char* filename)
{
    //Le hashmap qui contient tous les "materials"
    std::map<std::string,std::map<std::string,vec3>> material;

    std::string currentName; //le nom du material d'on on recup les infos

    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
       printf("Impossible to open the file !\n");
    }

    std::string line;
    std::cout<< "start reading materials"<< std::endl;
    while (getline(in, line))
    {
        if (line.substr(0,7) == "newmtl ")
        {
            currentName = line.substr(7);
            std::map<std::string,vec3> newMtl;
            material[currentName]=newMtl;
            //std::cout<<"new material: " << currentName << std::endl;
        }

        if (line.substr(0,3) == "Ka ")
        {
            std::istringstream s (line.substr(3));
            vcl::vec3 Ka; s >> Ka.x; s >> Ka.y; s >> Ka.z;
            std::cout<< currentName <<" Ka read "<< Ka << std::endl;
            material[currentName]["Ka"]=Ka;
        }
        if (line.substr(0,3) == "Kd ")
        {
            std::istringstream s (line.substr(3));
            vcl::vec3 Kd; s >> Kd.x; s >> Kd.y; s >> Kd.z;
            std::cout<< currentName <<" Kd read "<< Kd << std::endl;
            material[currentName]["Kd"]=Kd;
        }
        if (line.substr(0,3) == "Ks ")
        {
            std::istringstream s (line.substr(3));
            vcl::vec3 Ks; s >> Ks.x; s >> Ks.y; s >> Ks.z;
            std::cout<< currentName << " Ks read "<< Ks << std::endl;
            material[currentName]["Ks"]=Ks;
        }
    }

    return material;
}

std::string load_text(const char* filename)
{
    std::string texture;

    std::string texture_pos="";

    std::ifstream in(filename, std::ios::in);
    if (!in)
    {
       printf("Impossible to open the file !\n");
    }

    std::string line;
//    std::cout<< "start reading textures"<< std::endl;
    while (getline(in, line))
    {
        if (line.substr(0,7) == "map_Kd ")
        {
            texture_pos = line.substr(7);
//            std::cout<< "texture read pos: "<< texture_pos << std::endl;
        }
    }

    return texture_pos;
}





