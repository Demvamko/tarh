#include <gl/gl.h>
#include <lib/glew.h>

uint Arh_Shader_Create(int* rvert, int* rfrag){
    uint prog = glCreateProgram();

    char* source = Arh_GetResource(rvert[0]);
    int len = rvert[1] - rvert[0];

    uint vert = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vert, 1, &source, &len);
    glCompileShader(vert);

    if(CompileFailed(vert))
        return 0;
    
    source = Arh_GetResource(rfrag[0]);
    len = rfrag[1] - rfrag[0];    

    uint frag = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(frag, 1, &source, &len);
    glCompileShader(frag);

    if(CompileFailed(frag))
        return 0;

    glAttachShader(prog, vert);
    glAttachShader(prog, frag);

    glCheckError();

    glLinkProgram(prog);

    if(LinkFailed(prog))
        return 0;
    
    return prog;
}

static int CompileFailed(uint shade){
    GLint compiled = 0;
    glGetShaderiv(shade, GL_COMPILE_STATUS, &compiled);

    if(compiled)
        return 0;

    char err[2048];
    glGetShaderInfoLog(shade, 2048, 0, err);
    printf("%s\n", err);

    return 1;
}

static int LinkFailed(uint prog){
    GLint linked = 0;
    glGetProgramiv(prog, GL_LINK_STATUS, &linked);
    
    if(linked)
        return 0;

    char err[2048];
    glGetProgramInfoLog(prog, 2048, 0, err);
    printf("%s\n", err);

    return 1;
}