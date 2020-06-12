#ifdef VERT

layout(location = 0) in int pos;
layout(location = 1) in int in_uv;
layout(location = 2) in int type;
layout(location = 3) in int in_ao;

out float ao;
out vec2 uv;

layout (binding = 0) uniform u0{ 
    mat4x4 projection;
    mat4x4 view;
};

layout (binding = 1) uniform u1{
    ivec2 cpos;
};

void main(){
    vec3 npos = vec3(
        pos & 0x001f, //0000000000011111
        (pos & 0x03E0) >> 5, //0000001111100000
        (pos & 0x7C00) >> 10  //0111110000000000
    );

    npos.x += cpos.x * 16;
    npos.z += cpos.y * 16;

    gl_Position = projection * view * vec4(npos, 1);
    ao = float(in_ao) / 16.0;

    float u = in_uv & 1;
    float v = (in_uv & 2) >> 1;
    int s = (in_uv & 0x1C) >> 2;

    int atlas_w = 16;
    float inv_atlas_w = 1.0 / 16.0;

    int idx = type * 6 + s;

    int cx = int(mod(idx, atlas_w));
    int cy = idx / atlas_w;

    vec2 bin_uv = vec2(u + float(cx), v + float(cy)) * inv_atlas_w;

    uv = bin_uv;
}

#endif

#ifdef FRAG

out vec4 color;
in float ao;
in vec2 uv;

layout (binding = 0) uniform sampler2D tex;

void main(){
    // color = vec4(1, fuv.x, fuv.y, 1);
    vec4 result = texture(tex, uv);

    result.rgb = result.rgb * ao;

    color = result;

}

#endif

