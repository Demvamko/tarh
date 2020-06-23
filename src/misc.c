#include <arhgl.h>
#include <misc.h>
#include <glew.h>

static uint shader;
static Buffer buffer;
static Attributes attr[] = {{ 2, GL_FLOAT }, { 0 }};

void InitCrosshair(){
    shader = CreateShader("./res/glsl/basic.glsl");
    buffer = CreateBuffer(sizeof(float) * 2, 1, GL_POINTS, (float[]) { 0.0f, 0.0f }, attr);
}

void RenderCrosshair(){
    glUseProgram(shader);
    RenderBuffer(&buffer);
}