/**
* \file hl1_studiomodel_buffer.h
*/

#ifndef HLMDLVIEWER_HL1_STUDIOMODEL_BUFFER_H_
#define HLMDLVIEWER_HL1_STUDIOMODEL_BUFFER_H_

#include "mesh_buffer.h"
#include "gltexture.h"

namespace hl_mdlviewer {
namespace hl1 {

/** \brief A structure that holds all Studiomodel mesh buffers 
* and stride infos. */
struct StudioModelBuffer
{
    StudioModelBuffer() :
        meshes(),
        bones(),
        attachments(),
        hitboxes(),
        sequence_bbox(),
        buffer(),
        gltextures()
    {

    }

    void clear()
    {
        meshes.clear();
        bones.reset();
        attachments.reset();
        hitboxes.clear();
        sequence_bbox.clear();

        buffer.delete_buffer();

        for (auto& t : gltextures)
            t.delete_texture();
        gltextures.clear();
    }

    /** Mesh strides. */
    std::vector<MeshBufferStride> meshes;

    /** Bones stride. */
    MeshBufferStride bones;

    /** Attachments stride. */
    MeshBufferStride attachments;

    /** Hitbox strides. */
    std::vector<MeshBufferStride> hitboxes;

    /** The Sequence bbox strides.
    * (1) faces stride.
    * (2) outline stride.
    */
    std::vector<MeshBufferStride> sequence_bbox;

    /** \brief the Studiomodel mesh buffer. */
    MeshBuffer buffer;

    /** \brief the Studiomodel OpenGL textures. */
    std::vector<gltexture> gltextures;
};

}
}

#endif // HLMDLVIEWER_HL1_STUDIOMODEL_BUFFER_H_
