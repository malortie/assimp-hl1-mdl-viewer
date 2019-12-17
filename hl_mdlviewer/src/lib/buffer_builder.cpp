/**
* \file buffer_builder.cpp
* \brief Implementation for the buffer builder class.
*/

#include "pch.h"
#include "buffer_builder.h"

namespace hl_mdlviewer {

BufferBuilder::BufferBuilder()
{
}

void BufferBuilder::append(
    const std::vector<glvertex>& vertices, 
    const std::vector<unsigned int>& indices,
    const unsigned int primitive_restart_index,
    MeshBufferStride& stride)
{
    stride.vertex_start_index = static_cast<int>(vertices_.size());
    stride.indice_start_index = static_cast<int>(indices_.size());
    stride.num_vertices = static_cast<int>(vertices.size());
    stride.num_indices = static_cast<int>(indices.size());

    for (auto it = vertices.begin(); it != vertices.end(); ++it)
        vertices_.push_back(*it);

    for (auto it = indices.begin(); it != indices.end(); ++it)
    {
        if (*it == primitive_restart_index)
            indices_.push_back(*it);
        else
            indices_.push_back(stride.vertex_start_index + *it);
    }
}

void BufferBuilder::append_indices(
    const MeshBufferStride& previous_stride,
    const std::vector<unsigned int>& indices,
    const unsigned int primitive_restart_index,
    MeshBufferStride& stride)
{
    stride.vertex_start_index = previous_stride.vertex_start_index;
    stride.indice_start_index = static_cast<int>(indices_.size());
    stride.num_vertices = previous_stride.num_vertices;
    stride.num_indices = static_cast<int>(indices.size());

    for (auto it = indices.begin(); it != indices.end(); ++it)
    {
        if (*it == primitive_restart_index)
            indices_.push_back(*it);
        else // Reuse the vertices from the prototype entry.
            indices_.push_back(previous_stride.vertex_start_index + *it);
    }
}

void BufferBuilder::reserve(const size_t num_vertices, const size_t num_indices)
{
    vertices_.reserve(num_vertices);
    indices_.reserve(num_indices);
}

}
