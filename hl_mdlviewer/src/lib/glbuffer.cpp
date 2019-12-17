/**
* \file glbuffer.cpp
* \brief Implementation for the OpenGL buffer class.
*/

#include "pch.h"
#include "glbuffer.h"

namespace hl_mdlviewer
{

glbuffer::glbuffer() : 
    vao_(0),
    vbo_(0), 
    ibo_(0), 
    num_indices_(0)
{
}

glbuffer::~glbuffer()
{
}

void glbuffer::initialize(
    const std::vector<glvertex>& vertices,
    const std::vector<unsigned int>& indices,
    const GLenum usage)
{
    num_vertices_ = static_cast<int>(vertices.size());
    num_indices_ = static_cast<int>(indices.size());

    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ibo_);

    glBindVertexArray(vao_);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(glvertex) * vertices.size(),
        vertices.data(),
        usage);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glvertex), (void*)offsetof(glvertex, position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glvertex), (void*)offsetof(glvertex, normal));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glvertex), (void*)offsetof(glvertex, uv));
    glVertexAttribIPointer(3, 1, GL_INT, sizeof(glvertex), (void*)offsetof(glvertex, boneid));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
        sizeof(unsigned int) * indices.size(),
        indices.data(),
        usage);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void glbuffer::delete_buffer()
{
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ibo_);
    vao_ = vbo_ = ibo_ = 0;
    num_vertices_ = num_indices_ = 0;
}

void glbuffer::draw_arrays(const GLenum mode, int first, int count)
{
    glBindVertexArray(vao_);
    glDrawArrays(mode, first, count);
    glBindVertexArray(0);
}

void glbuffer::draw_arrays(const GLenum mode)
{
    draw_arrays(mode, 0, num_vertices_);
}

void glbuffer::draw_arrays_unbinded(const GLenum mode, int first, int count)
{
    glDrawArrays(mode, first, count);
}

void glbuffer::draw_arrays_unbinded(const GLenum mode)
{
    draw_arrays_unbinded(mode, 0, num_vertices_);
}

void glbuffer::draw_indexed(const GLenum mode, int count)
{
    glBindVertexArray(vao_);
    glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void glbuffer::draw_indexed(const GLenum mode)
{
    draw_indexed(mode, num_indices_);
}

void glbuffer::draw_indexed_unbinded(const GLenum mode, int count)
{
    glBindVertexArray(vao_);
    glDrawElements(mode, count, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void glbuffer::draw_indexed_unbinded(const GLenum mode)
{
    draw_indexed_unbinded(mode, num_indices_);
}

void glbuffer::draw_indexed_range_unbinded(const GLenum mode, int start, int num_indices)
{
    void* indices[1] = { (void*)(start * sizeof(unsigned int)) };
    GLint count[1] = { num_indices };
    GLint drawcount = ARRAYSIZE(count);

    glMultiDrawElements(
        mode,
        count,
        GL_UNSIGNED_INT,
        indices,
        drawcount);
}

void glbuffer::set_vertices(const std::vector<glvertex>& vertices, const size_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    char* ptr = (char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr + offset * sizeof(glvertex), vertices.data(), vertices.size() * sizeof(glvertex));
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void glbuffer::set_vertices(const std::vector<glvertex>& vertices)
{
    set_vertices(vertices, 0);
}

}
