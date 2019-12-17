/**
* \file mesh_buffer.cpp
* \brief Implementation for the mesh buffer class.
*/

#include "pch.h"
#include "mesh_buffer.h"

namespace hl_mdlviewer {

MeshBuffer::MeshBuffer() :
    buffer_()
{
}

MeshBuffer::~MeshBuffer()
{
}

void MeshBuffer::initialize(
    const std::vector<glvertex>& vertices,
    const std::vector<unsigned int>& indices,
    GLenum usage)
{
    buffer_.initialize(vertices, indices, usage);
}

void MeshBuffer::delete_buffer()
{
    buffer_.delete_buffer();
}

void MeshBuffer::draw_arrays_unbinded(const GLenum mode)
{
    buffer_.draw_arrays_unbinded(mode);
}

void MeshBuffer::draw_arrays_unbinded(const GLenum mode, const MeshBufferStride& stride)
{
    buffer_.draw_arrays_unbinded(mode, stride.vertex_start_index, stride.num_vertices);
}

void MeshBuffer::draw_indexed_unbinded(const GLenum mode, const MeshBufferStride& stride)
{
    buffer_.draw_indexed_range_unbinded(
        mode,
        stride.indice_start_index,
        stride.num_indices);
}

void MeshBuffer::draw_indexed_unbinded(const GLenum mode)
{
    buffer_.draw_indexed_unbinded(mode);
}

void MeshBuffer::set_vertices(const MeshBufferStride& stride, const std::vector<glvertex>& vertices)
{
    buffer_.set_vertices(vertices, stride.vertex_start_index);
}

}
