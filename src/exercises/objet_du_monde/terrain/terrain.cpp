
#include "terrain.hpp"

// visual representation of a surface


vcl::mesh create_terrain(const gui_scene_structure& gui_scene)
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
            const float x = 70*(u-0.5f);
            const float y = 100*(v-0.5f);


            //float radious = 8;
            float z = evaluate_terrain_z(u,v, gui_scene); //fait les deux aplanissement en meme temps


            // Compute coordinates
            terrain.position[kv+N*ku] = {x,y,z};
            terrain.color[kv+N*ku]  = {1,1,1,1.0f};
            terrain.texture_uv[kv+N*ku] = {15*u, 15*v};
            terrain.color[kv+N*ku]  = {1,1,1,0.0f};
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

float evaluate_terrain_z(float u, float v, const gui_scene_structure& gui_scene)
{
    // get gui parameters
    const float scaling = gui_scene.scaling;
    const int octave = gui_scene.octave;
    const float persistency = gui_scene.persistency;
    const float height = gui_scene.height;

    //Norme
    const float x = 70*(u-0.5f);
    const float y = 100*(v-0.5f);
    const float norm= sqrt(pow(x,2)+pow(std::max(y + 10.0,0.0),2));

    const std::vector<vcl::vec2> pi = {{0,0}, {0.5f,0.5f}, {0.2f,0.7f}, {0.8f,0.7f},{0.4f,0.8f},{0.6f,0.4f},{0.2f,0.7f}, {0.8f,0.7f},{0.4f,0.8f}};
    const std::vector<float> hi = {3.0f, -1.5f, 1.0f, 2.0f, 1.5f,1.0f,3.0f, -1.5f, 1.0f};
    const std::vector<float> sigma_i = {0.5f, 0.15f, 0.2f, 0.2f, 0.16f, 0.4f,0.2f, 0.2f, 0.16f};

    const size_t N = pi.size();
    float z = 0.0f;
    for(size_t k=0; k<N; ++k)
    {
        const float u0 = pi[k].x;
        const float v0 = pi[k].y;
        const float d2 = (u-u0)*(u-u0)+(v-v0)*(v-v0);
        z += hi[k] * std::exp( - d2/sigma_i[k]/sigma_i[k] );

        z += 0.2f*vcl::perlin(scaling*u, scaling*v, octave, persistency);

    }

    z=z*height;

    //on fait la deformation puis on applique le deuxieme aplanissement sur le resultat
    float d= std::max(norm-radious,0.0f)/1.5;
    d=1-exp(-pow(d,2));
    d=(d>1)?1:d;
    if(d!=0) return evaluate_terrain_z2(x,y, 1.2*z*d );

    return evaluate_terrain_z2(x,y,  -exp(-pow(0.5*pow(norm,2),2)/10)) ;

}

float evaluate_terrain_z2(float x, float y, float z)
{

    //Norme
    const float norm= sqrt(pow(std::max(x-40,0.0f),2)+pow(y+45, 2));
    float d= std::max(norm-radious,0.0f)/1.5;
    d=1-exp(-pow(d,2));
    d=(d>1)?1:d;
    if(d!=0) return 1.2*z*d;

    const float norm2= sqrt(pow(x,2)+pow(std::max(y+20.0,0.0),2));

    return -exp(-pow(0.5*pow(norm2,2),2)/10);
}



float evaluate_terrain2_z(float x, float y, const gui_scene_structure& gui_scene)
{
    //Norme
    const float u = (x/70)+0.5;
    const float v = (y/100)+0.5;
    return evaluate_terrain_z(u,v,gui_scene);

}

vcl::vec3 evaluate_terrain(float u, float v,const gui_scene_structure& gui_scene)
{
    const float x = 70*(u-0.5f);
    const float y = 100*(v-0.5f);
    const float z = evaluate_terrain_z(u,v,gui_scene);

    return {x,y,z};
}

size_t index_at_value(float t, const std::vector<float>& vt)
{
    const size_t N = vt.size();
    assert(vt.size()>=2);
    assert(t>=vt[0]);
    assert(t<vt[N-1]);

    size_t k=0;
    while( vt[k+1]<t )
        ++k;
    return k;
}

vcl::mesh_drawable create_underTerrain(const gui_scene_structure& gui_scene){
    // Number of samples of the terrain is N x N
    const size_t N = 600;

    vcl::mesh underTerrain; // temporary terrain storage (CPU only)

    // Fill terrain geometry
    float height = -5.0f;

    size_t kv=0;
    for(size_t ku=0; ku<N; ++ku)
    {
        const float u = ku/(N-1.0f);
        const float v = kv/(N-1.0f);
        vcl::vec3 p= evaluate_terrain(u,v,gui_scene);
        underTerrain.position.push_back( p );
        underTerrain.position.push_back({p.x,p.y, height});

    }

    size_t ku=N-1;
    for(size_t kv=1; kv<N; ++kv)
    {
        const float u = ku/(N-1.0f);
        const float v = kv/(N-1.0f);
        vcl::vec3 p= evaluate_terrain(u,v,gui_scene);
        underTerrain.position.push_back( p );
        underTerrain.position.push_back({p.x,p.y, height});
    }

    kv=N-1;
    for(int ku=N-2; ku>=0; --ku)
    {
        const float u = ku/(N-1.0f);
        const float v = kv/(N-1.0f);
        vcl::vec3 p= evaluate_terrain(u,v,gui_scene);
        underTerrain.position.push_back( p );
        underTerrain.position.push_back({p.x,p.y, height});
    }

    ku=0;
    for(int kv=N-2; kv>0; --kv)
    {
        const float u = ku/(N-1.0f);
        const float v = kv/(N-1.0f);
        vcl::vec3 p= evaluate_terrain(u,v,gui_scene);
        underTerrain.position.push_back( p );
        underTerrain.position.push_back({p.x,p.y, height});
    }

    const unsigned int Ns = 4*(N-1);
    for(unsigned int k=0; k<Ns; ++k)
    {
        const unsigned int u00 = 2*k;
        const unsigned int u01 = (2*k+1)%(2*Ns);
        const unsigned int u10 = (2*(k+1))%(2*Ns);
        const unsigned int u11 = (2*(k+1)+1) % (2*Ns);

        const vcl::index3 t1 = {u00, u10, u11};
        const vcl::index3 t2 = {u00, u11, u01};

        underTerrain.connectivity.push_back(t1);
        underTerrain.connectivity.push_back(t2);

    }
    vcl::mesh_drawable underTerrain_draw=underTerrain;
    return underTerrain_draw;

}
