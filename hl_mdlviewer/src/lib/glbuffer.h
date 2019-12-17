/**
* \file glbuffer.h
* \brief Declaration for the OpenGL buffer class.
*/

#ifndef HLMDLVIEWER_GLBUFFER_H_
#define HLMDLVIEWER_GLBUFFER_H_

#include "glad.h"
#include "glvertex.h"

namespace hl_mdlviewer {

/** A class that wraps an OpenGL vertex array object. */
class glbuffer
{
public:
    glbuffer();
    ~glbuffer();

    inline const GLuint vertex_array_id() const { return vao_; }
    inline const GLuint vertex_buffer_id() const { return vbo_; }
    inline const GLuint index_buffer_id() const { return ibo_; }
    inline const int num_vertices() const { return num_vertices_; }
    inline const int num_indices() const { return num_indices_; }

    void initialize(
        const std::vector<glvertex>& vertices,
        const std::vector<unsigned int>& indices,
        const GLenum usage = GL_STATIC_DRAW);

    void delete_buffer();

    void draw_arrays(const GLenum mode, int first, int count);
    void draw_arrays(const GLenum mode);
    void draw_arrays_unbinded(const GLenum mode, int first, int count);
    void draw_arrays_unbinded(const GLenum mode);

    void draw_indexed(const GLenum mode, int count);
    void draw_indexed(const GLenum mode);
    void draw_indexed_range_unbinded(const GLenum mode, int start, int count);
    void draw_indexed_unbinded(const GLenum mode, int count);
    void draw_indexed_unbinded(const GLenum mode);

    void set_vertices(const std::vector<glvertex>& vertices, const size_t offset);
    void set_vertices(const std::vector<glvertex>& vertices);

    inline void bind() {
        glBindVertexArray(vao_);
    }

    inline void unbind() {
        glBindVertexArray(0);
    }

private:

    GLuint vao_, vbo_, ibo_;
    int num_vertices_;
    int num_indices_;
};

}

#endif // HLMDLVIEWER_GLBUFFER_H_
