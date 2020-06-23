#ifdef VERT

layout(location = 0) in ivec2 pos;
layout(location = 1) in vec4 in_color;
layout(location = 2) in int in_uv;

layout (binding = 1) uniform u1{
    ivec2 res;
};

out vec4 rcolor;
out vec2 uv;

void main(){
    vec2 p = vec2(pos) / vec2(res);
    p = (p - 0.5) * 2.0;

    float u = in_uv & 1;
    float v = (in_uv & 2) >> 1;
    float z = (in_uv & (~4)) >> 3;

    uv = vec2(u, v); 

    rcolor = in_color;

    gl_Position = vec4(p, -z / 500.0, 1.0);
}

#endif

#ifdef FRAG

out vec4 color;

in vec2 uv;
in vec4 rcolor;

void main(){
    color = rcolor;
}

#endif

