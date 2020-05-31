#ifdef VERT

layout(location = 0) in vec2 pos;

void main(){
    gl_Position = vec4(pos, 0, 1);
}

#endif

#ifdef FRAG

out vec4 color;

void main(){
    color = vec4(1, 1, 1, 1);
}

#endif

