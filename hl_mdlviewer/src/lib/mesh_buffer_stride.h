/**
* \file mesh_buffer_stride.h
*/

#ifndef HLMDLVIEWER_MESH_BUFFER_STRIDE_H_
#define HLMDLVIEWER_MESH_BUFFER_STRIDE_H_

namespace hl_mdlviewer {

/** \brief A structure that contains the stride of a single mesh. */
struct MeshBufferStride
{
    MeshBufferStride() :
        vertex_start_index(-1),
        indice_start_index(-1),
        num_vertices(0),
        num_indices(0)
    {
    }

    void reset()
    {
        vertex_start_index = -1;
        indice_start_index = -1;
        num_vertices = 0;
        num_indices = 0;
    }

    /** The index for the first vertex of the mesh. */
    int vertex_start_index;

    /** The index for the first "indice" of the mesh. */
    int indice_start_index;

    /** The mesh vertex count. */
    int num_vertices;

    /** The mesh "indice" count. */
    int num_indices;
};

}

#endif // HLMDLVIEWER_MESH_BUFFER_STRIDE_H_
