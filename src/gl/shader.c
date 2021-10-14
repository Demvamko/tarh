#include <arh/gl.h>
#include <gl/glew.h>
#include <stdio.h>

static int CompileFailed(uint shade){
    uint compiled = 0;
    glGetShaderiv(shade, GL_COMPILE_STATUS, &compiled);

    if(compiled)
        return 0;

    char err[2048];
    glGetShaderInfoLog(shade, 2048, 0, err);
    printf("%s\n", err);

    return 1;
}

static int LinkFailed(uint prog){
    uint linked = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    
    if(linked)
        return 0;

    char err[2048];
    glGetProgramInfoLog(prog, 2048, 0, err);
    printf("%s\n", err);

    return 1;
}

uint Arh_Shader_Create(char* svert, char* sfrag){
    uint prog = glCreateProgram();

    uint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &svert, (int[]){ arrlen(svert) });
    glCompileShader(vert);
    
    uint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &sfrag, (int[]){ arrlen(sfrag) });
    glCompileShader(frag);

    if(CompileFailed(vert) || CompileFailed(frag))
        return 0;

    glAttachShader(prog, vert);
    glAttachShader(prog, frag);
    glLinkProgram(prog);

    if(LinkFailed(prog))
        return 0;
    
    return prog;
}

void Arh_Shader_Bind(uint shader){
    glUseProgram(shader);
}