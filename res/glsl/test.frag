#version 440 
out vec4 _ren_color;
in vec2 _ren_uv; 
in float _ren_fao; 
layout (binding = 0) uniform sampler2D tex; 

void main(){ 
    vec4 color = texture(tex, _ren_uv);
    color.rgb *= _ren_fao;
    _ren_color =  color;
}
