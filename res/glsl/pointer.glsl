#ifdef VERT

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec2 ao;

out vec2 fuv;
out vec3 p;

layout (binding = 0) uniform u0{ 
    mat4x4 projection;
    mat4x4 view;
};

layout (binding = 1) uniform u1{
    ivec2 cpos;
};

layout (binding = 2) uniform u2{
    vec3 ppos;
};

void main(){
    vec3 npos = pos + ppos;

    gl_Position = projection * view * vec4(npos, 1);
    fuv = uv;    
    p = pos;
}

#endif

#ifdef FRAG

out vec4 color;
in vec2 fuv;
in vec3 p;

void main(){
    // color = vec4(1, fuv.x, fuv.y, 1);
    float c = 0.1;

    if(
        (mod(p.x, 1.0) > 0.95 || mod(p.x, 1.0) < 0.05) &&
        (mod(p.y, 1.0) > 0.95 || mod(p.y, 1.0) < 0.05) ||
        (mod(p.z, 1.0) > 0.95 || mod(p.z, 1.0) < 0.05)
    )
        c = 0.7;
    
    color = vec4(c, 0, 0, 0.5);

}

#endif

