#ifdef VERT

layout(location = 0) in ivec2 pos;
layout(location = 1) in int c;
layout(location = 2) in int in_uv;

layout (binding = 1) uniform u1{
    ivec2 res;
};

layout (binding = 3) uniform u3{
    float uvs[127 * 4];
};

out vec2 uv;

void main(){
    int idu = in_uv & 1;
    int idv = (in_uv & 2) >> 1;
    int z = (in_uv & ~2) >> 2; 
    
    float u = uvs[c * 4 + 0 + idu * 2];
    float v = uvs[c * 4 + 1 + idv * 2];

    uv = vec2(u, v);

    vec2 p = vec2(pos) / vec2(res);
    p = (p - 0.5) * 2.0;
    gl_Position = vec4(p, -z / 500.0, 1.0);
}

#endif

#ifdef FRAG

out vec4 color;

in vec2 uv;

layout (binding = 0) uniform sampler2D tex;

void main(){
    vec4 c = texture(tex, vec2(uv.x, uv.y));
 
    color = vec4(1, 1, 1, c.r);
}

#endif

