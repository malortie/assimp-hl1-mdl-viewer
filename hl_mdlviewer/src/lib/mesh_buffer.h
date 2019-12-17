/**
* \file mesh_buffer.h
* \brief Declaration for the mesh buffer class.
*/

#ifndef HLMDLVIEWER_MESH_BUFFER_H_
#define HLMDLVIEWER_MESH_BUFFER_H_

#include "glbuffer.h"
#include "mesh_buffer_stride.h"

namespace hl_mdlviewer {

/** \brief A convinient way to draw separate meshes that share
* a single buffer. */
class MeshBuffer
{
public:
    MeshBuffer();
    ~MeshBuffer();

    void initialize(
        const std::vector<glvertex>& vertices,
        const std::vector<unsigned int>& indices,
        GLenum usage = GL_STATIC_DRAW);

    void delete_buffer();

    void draw_arrays_unbinded(const GLenum mode);
    void draw_arrays_unbinded(const GLenum mode, const MeshBufferStride& stride);

    void draw_indexed_unbinded(const GLenum mode, const MeshBufferStride& stride);
    void draw_indexed_unbinded(const GLenum mode);

    inline void bind() {
        buffer_.bind();
    }

    inline void unbind() {
        buffer_.unbind();
    }

    void set_vertices(const MeshBufferStride& stride, const std::vector<glvertex>& vertices);

private:
    glbuffer buffer_;
};

}

#endif // HLMDLVIEWER_MESHBUFFER_H_
