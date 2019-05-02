
#include "objet_du_monde.hpp"

// visual representation of a surface


vcl::mesh create_grid(const gui_scene_structure& gui_scene)
{
    // Number of samples of the terrain is N x N
    const size_t N = 600;

    vcl::mesh terrain; // temporary terrain storage (CPU only)
    terrain.texture_uv.resize(N*N);
    terrain.position.resize(N*N);
    terrain.color.resize(N*N);

    // Fill terrain geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);


            // get gui parameters
            const float scaling = gui_scene.scaling;
            const int octave = gui_scene.octave;
            const float persistency = gui_scene.persistency;
            const float height = gui_scene.height;

            // Evaluate Perlin noise
            const float noise = vcl::perlin(scaling*u, scaling*v, octave, persistency);

            // 3D vertex coordinates
            const float x = 30*(u-0.5f);
            const float y = 30*(v-0.5f);

            float z = evalZ(x,y,height,noise);

            const float c = 0.3f+0.7f*noise;

            // Compute coordinates
            terrain.position[kv+N*ku] = {x,y,z};
            terrain.color[kv+N*ku]  = {c,c,c,1.0f};
            terrain.texture_uv[kv+N*ku] = {15*u, 15*v};
            //terrain.color[kv+N*ku]  = {1,1,1,0.0f};
        }
    }


    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    unsigned int Ns = N;
    for(size_t ku=0; ku<Ns-1; ++ku)
    {
        for(size_t kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + Ns*ku; // current vertex offset

            const vcl::index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const vcl::index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }

    return terrain;
}
float evalZ(float x, float y, float height, float noise){
    float distance= sqrt(x*x+y*y)/21;
    std::cout<< distance<< std::endl;
    if(x*x+y*y<4)
        return 0;
    else if(x*x+y*y<16){
        distance=-2;
        return height*noise*distance;}
    return height*noise;



}

