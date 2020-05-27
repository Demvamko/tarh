#ifdef VERT

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec2 ao;

out vec2 fuv;

layout (binding = 0) uniform u0{ 
    mat4x4 projection;
    mat4x4 view;
};

void main(){
    gl_Position = projection * view * vec4(pos, 1);
    fuv = uv;    
}

#endif

#ifdef FRAG

out vec4 color;
in vec2 fuv;

void main(){
    color = vec4(1, fuv.x, fuv.y, 1);
}

#endif

