
#include "eau.hpp"

// visual representation of a surface
vcl::mesh create_eau(const gui_scene_structure& gui_scene)
{
    // Number of samples of the eau is N x N
    const size_t N = 40;

    vcl::mesh eau; // temporary eau storage (CPU only)
    eau.texture_uv.resize(N*N);
    eau.position.resize(N*N);
    eau.color.resize(N*N);



    // Fill eau geometry
    for(size_t ku=0; ku<N; ++ku)
    {
        for(size_t kv=0; kv<N; ++kv)
        {
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

            // get gui parameters
            //const float scaling = gui_scene.eau_scaling;
            const float scaling = gui_scene.eau_scaling*sin(gui_scene.eau_time.t);
            const int octave = gui_scene.eau_octave;
            const float persistency = gui_scene.eau_persistency;
            const float height = gui_scene.eau_height;

            // Evaluate Perlin noise
            const float noise = vcl::perlin(scaling*u, scaling*v, octave, persistency);

            const float x = 5*(u-0.5f);
            const float y = 45*(v-0.5f)-28;

            //const float z = h(x,y,gui_scene.eau_time.t)*height;
            const float z = height*noise-5.5;


            eau.position[kv+N*ku] = {x,y,z};
            eau.texture_uv[kv+N*ku] = {15*u, 15*v};
            eau.color[kv+N*ku]  = {1,1,1,0.0f};
        }
    }

    unsigned int Ns = N;
    for(size_t ku=0; ku<Ns-1; ++ku)
    {
        for(size_t kv=0; kv<Ns-1; ++kv)
        {
            const unsigned int idx = kv + Ns*ku; // current vertex offset

            const vcl::index3 triangle_1 = {idx, idx+1+Ns, idx+1};
            const vcl::index3 triangle_2 = {idx, idx+Ns, idx+1+Ns};

            eau.connectivity.push_back(triangle_1);
            eau.connectivity.push_back(triangle_2);
        }
    }

    return eau;

}

float h(const float x, const float y, const float t)
{
    const std::vector<float> kx = {3.0f, -1.5f, 1.0f, 2.0f};
    const std::vector<float> A = {2.0f, -7.0f, 1.0f, 2.0f};
    const std::vector<float> kz = { 2.0f,  -1.5f, 1.0f, 3.0f};
    const std::vector<float> w = {0.5f, 2.0f, 1.5f,1.0f,};

    float output=0;
    for (int i=0; i< kx.size(); ++i) {
        output+= A[i]*cos(kx[i]*x + kz[i]*y - w[i]*t);
    }

    return output;

}



