#include <arh/gl.h>
#include <lib/glew.h>

uint Arh_Attrib_Create(Attribute* attribs){
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

    return vao;
}

void Arh_Buffer_Create(Buffer* buffer, uint type, void* data, uint vao){
    buffer->vao = vao;
    buffer->type = type;
    buffer->data = data;
    buffer->count = arrcount(data);
    buffer->stride = arrelem(data);

    glGenBuffers(1, &(buffer->id));
    glNamedBufferData(buffer->id, arrlen(buffer->data), buffer->data, GL_STATIC_DRAW);
}

void Arh_Buffer_Render(Buffer* buffer){
    glBindVertexArray(buffer->vao);
    glBindVertexBuffer(0, buffer->id, 0, buffer->stride);

    glDrawArrays(buffer->type, 0, buffer->count);
}

void Arh_Buffer_Update(Buffer* buffer, uint offset, uint size){
    glNamedBufferSubData(buffer->id, offset, size, buffer->data);
}

void Arh_Buffer_Delete(Buffer* buffer){
    glDeleteBuffers(1, &buffer->id);
}
