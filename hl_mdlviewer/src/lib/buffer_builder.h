/**
* \file buffer_builder.h
* \brief Declaration for the buffer builder class.
*/

#ifndef HLMDLVIEWER_BUFFER_BUILDER_H_
#define HLMDLVIEWER_BUFFER_BUILDER_H_

#include <vector>
#include "glvertex.h"
#include "mesh_buffer.h"

namespace hl_mdlviewer {

/** \brief A convinient way to combine multiple set of vertices and
* indices to form a single buffer. */
class BufferBuilder
{
public:
    BufferBuilder();

    /** \brief Append \p vertices and \p indices to the buffer.
    * \param[in] vertices A list of vertices to append to the buffer.
    * \param[in] indices A list of indices to append to the buffer.
    * \param[in] primitive_restart_index The value used in \p indices
    *            to identify indices used to restart primitive.
    * \param[out] stride The stride info.
    */
    void append(const std::vector<glvertex>& vertices, 
        const std::vector<unsigned int>& indices,
        const unsigned int primitive_restart_index,
        MeshBufferStride& stride);

    /** \brief Append \p indices to the buffer while reusing the vertices
    *          stride info from \p previous_stride.
    * \param[in] previous_stride A stride from which the vertex stride info 
    *            will be used.
    * \param[in] indices A list of indices to append to the buffer.
    * \param[in] primitive_restart_index The value used in \p indices
    *            to identify indices used to restart primitive.
    * \param[out] stride The stride info.
    */
    void append_indices(const MeshBufferStride& previous_stride,
        const std::vector<unsigned int>& indices,
        const unsigned int primitive_restart_index,
        MeshBufferStride& stride);

    void reserve(const size_t num_vertices, const size_t num_indices);

    const std::vector<glvertex>& get_vertices() const { return vertices_; }
    const std::vector<unsigned int>& get_indices() const { return indices_; }

private:

    /** \brief The resulting vertices. */
    std::vector<glvertex> vertices_;

    /** \brief The resulting indices. */
    std::vector<unsigned int> indices_;
};

}

#endif // HLMDLVIEWER_BUFFER_BUILDER_H_
