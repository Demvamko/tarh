#include <gl/buff.h>
#include <lib/glew.h>

uint Arh_Attr_Create(Attribute* attribs){
    uint vao = 0;

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    for(int i = 0; attribs[i].type; i++){
        Attribute att = attribs[i];

        glEnableVertexAttribArray(i);
        
        if(att.as_int)
            glVertexAttribIFormat(i, att.size, att.type, att.offset);
        else
            glVertexAttribFormat(i, att.size, att.type, att.normalize, att.offset);
        
        glVertexAttribBinding(i, 0);
    }
}

void Arh_Buff_Create(Buffer* buffer, uint type, void* data, uint vao){
    buffer->vao = vao;
    buffer->type = type;
    buffer->data = data;

    glGenBuffers(1, &(buffer->vbo));
    glBindBuffer(GL_ARRAY_BUFFER, buffer->vbo);
    glBufferData(GL_ARRAY_BUFFER, arrlen(buffer->data), buffer->data, GL_STATIC_DRAW);
}

void Arh_Buff_Render(Buffer* buffer){
    glBindVertexArray(buffer->vao);
    glBindVertexBuffer(0, buffer->vbo, 0, arrelem(buffer->data));

    glDrawArrays(buffer->type, 0, arrcount(buffer->data));
}

void Arh_Buff_Update(Buffer* buffer, uint offset, uint size){
    glNamedBufferSubData(buffer->vbo, offset, size, buffer->data);
}

void Arh_Buff_Delete(Buffer* buffer){
    glDeleteBuffers(1, &buffer->vbo);
}
